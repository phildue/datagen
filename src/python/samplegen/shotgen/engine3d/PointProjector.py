import numpy as np
from numpy import matlib

from samplegen.scene.Camera import Camera
from utils.labels.GateCorners import GateCorners
from utils.labels.GateLabel import GateLabel
from utils.labels.Pose import Pose


class PointProjector:
    def __init__(self, cam: Camera, width, height, near_clipping_plane=0.1, min_area=0.5):

        self.min_area = min_area
        self.near_clipping_plane = near_clipping_plane
        self.cam = cam
        self.height = height
        self.width = width

        self.top = self.height / 2
        self.bottom = -self.top
        self.right = self.width / 2
        self.left = -self.right

    def create_labels(self, objects: [(GateLabel, Pose)]):
        obj_labels = []
        for obj, obj_position in objects:

            corners3d_hom = self.__to3dcoords(obj.model.corners, obj_position)
            relative_camera_pose = self.get_relative_pose(self.cam.pose, obj_position)

            corners2d_hom = self.cam.project(corners3d_hom.T)

            if self.__points_behind_camera(corners2d_hom) > 0 or \
                    not self._visible(corners2d_hom):
                continue

            gate_corners = self.__create_corner_object(corners2d_hom)
            obj_labels.append(GateLabel(pose=relative_camera_pose.to_meters, gate_corners=gate_corners,
                                        class_name='gate'))
        return obj_labels

    @staticmethod
    def __points_behind_camera(points):
        return points[(points[:, 2] < 0)].shape[0]

    def _visible(self, points):
        x_min = np.min(points[:, 0])
        y_min = np.min(points[:, 1])
        x_max = np.max(points[:, 0])
        y_max = np.max(points[:, 1])

        if points[(points[:, 0] > self.right) | (points[:, 0] < self.left) | (points[:, 1] > self.top) | (
                    points[:, 1] < self.bottom)].shape[0] >= 5:
            return False

        x_min_visible = max(x_min, self.left)
        y_min_visible = max(y_min, self.bottom)
        x_max_visible = min(x_max, self.right)
        y_max_visible = min(y_max, self.top)

        area_total = (x_max - x_min) * (y_max - y_min)
        area_visible = (x_max_visible - x_min_visible) * (y_max_visible - y_min_visible)

        return area_visible / area_total > self.min_area

    def __create_corner_object(self, corners):
        """
        Creates an object of type gate corners
        (1) Transform coordinate space from gate center origin to lower left corner origin
        (2) Assign matrix values to corners
        :param corners: projected corners as matrix in coordinate system of scene
        :return: if corners are inside the image: object of type gate corners, in coordinate system of image
                None otherwise
        """
        corners += matlib.repmat(np.array(
            [[0.5 * self.width, 0.5 * self.height, 0]]),
            5, 1)
        corners = np.round(corners).astype(int)

        return GateCorners(center=(corners[0, 0], corners[0, 1]),
                           top_left=(corners[1, 0], corners[1, 1]),
                           top_right=(corners[2, 0], corners[2, 1]),
                           bottom_right=(corners[3, 0], corners[3, 1]),
                           bottom_left=(corners[4, 0], corners[4, 1]))

    @staticmethod
    def __to3dcoords(corners: GateCorners, position: Pose):
        corners_mat = corners.mat
        corners_mat = np.hstack([corners_mat, np.ones(shape=(corners_mat.shape[0], 1))])
        transmat = position.transfmat
        transmat[2] = -transmat[
            2]  # positive direction towards view, for some reason this is only required when moving an object

        return np.matmul(transmat, corners_mat.T)

    @staticmethod
    def get_relative_pose(pos_cam: Pose, pos_obj: Pose):
        translation = pos_cam.transvec - pos_obj.transvec
        translation /= Pose.METER_2_SCENE_UNIT
        rotation = np.matmul(pos_obj.rotmat.T, pos_cam.rotmat)
        yaw = np.arctan2(-rotation[2, 0], np.sqrt(rotation[2, 1] ** 2 + rotation[2, 2] ** 2))
        roll = np.arctan2(rotation[2, 1], rotation[2, 2])
        pitch = np.arctan2(rotation[1, 0], rotation[0, 0])
        pose_rel = Pose(yaw=yaw, roll=roll, pitch=pitch)
        pose_rel.transvec = translation

        return pose_rel
