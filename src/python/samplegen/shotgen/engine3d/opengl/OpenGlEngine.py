"""
Chapter 4.4, Programming Computer Vision with Python, Jan Erik Solem
"""
import copy

import numpy as np
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import glutInit, sys

from samplegen.scene.Light import Light
from samplegen.scene.Scene import Scene
from utils.imageprocessing.Backend import parse_screenshot


class OpenGlEngine:
    METER_2_SCENE_UNIT = 1.167

    def __init__(self, scene: Scene, width=1000, height=747, near_clipping_pane=0.1, far_clipping_pane=70.0):
        self.near_clipping_pane = near_clipping_pane
        self.far_clipping_pane = far_clipping_pane
        self.scene = scene
        self.width = width
        self.height = height
        self.light_sources = [GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7]
        self.light_state = [False, False, False, False, False, False, False, False]
        self.lights = self.scene.lights

    def init(self):
        glutInit(sys.argv)
        self.__init_projection(self.scene.cam.calibration_mat)
        glDepthFunc(GL_LESS)
        glEnable(GL_DEPTH_TEST)
        glEnable(GL_LIGHTING)
        glEnable(GL_NORMALIZE)
        glEnable(GL_COLOR_MATERIAL)

        light_ambient = [0.5, 0.5, 0.5, 1.0]
        light_diffuse = [0.5, 0.5, 0.5, 1.0]
        light_specular = [0.8, 0.8, 0.95, 1.0]
        light_position = [0, 1000.0, -1700.0, 0.0]

        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse)
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular)
        glLightfv(GL_LIGHT0, GL_POSITION, light_position)
        glEnable(GL_LIGHT0)

        light_ambient = [0.0, 0.0, 0.0, 1.0]
        light_diffuse = [0.7, 0.7, 0.7, 1.0]
        light_specular = [0.7, 0.7, 1.4, 1.0]
        light_position = [-10, 0.0, -10.0, 0.0]

        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient)
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse)
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular)
        glLightfv(GL_LIGHT1, GL_POSITION, light_position)
        # glEnable(GL_LIGHT1)

        light_ambient = [0.0, 0.0, 0.0, 1.0]
        light_diffuse = [0.7, 0.7, 0.7, 1.0]
        light_specular = [0.7, 0.7, 1.4, 1.0]
        light_position = [0, 10.0, -10.0, 0.0]

        glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient)
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse)
        glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular)
        glLightfv(GL_LIGHT2, GL_POSITION, light_position)
        # glEnable(GL_LIGHT2)

        light_ambient = [0.0, 0.0, 0.0, 1.0]
        light_diffuse = [0.7, 0.7, 0.7, 1.0]
        light_specular = [0.7, 0.7, 1.4, 1.0]
        light_position = [0, -10.0, -10.0, 0.0]

        glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient)
        glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse)
        glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular)
        glLightfv(GL_LIGHT3, GL_POSITION, light_position)
        # glEnable(GL_LIGHT3)

        light_ambient = [0.0, 0.0, 0.0, 1.0]
        light_diffuse = [0.7, 0.7, 0.7, 1.0]
        light_specular = [0.7, 0.7, 1.4, 1.0]
        light_position = [0, -10.0, -10.0, 0.0]

        glLightfv(GL_LIGHT4, GL_AMBIENT, light_ambient)
        glLightfv(GL_LIGHT4, GL_DIFFUSE, light_diffuse)
        glLightfv(GL_LIGHT4, GL_SPECULAR, light_specular)
        glLightfv(GL_LIGHT4, GL_POSITION, light_position)
        # glEnable(GL_LIGHT4)

    def __init_projection(self, calibration_mat: np.array):
        """ Set view from a camera calibration matrix. """
        # Choose matrix to modify GL_PROJECTION corresponds to calibration mat of pinhole camera
        glMatrixMode(GL_PROJECTION)
        # Starting with identity matrix
        # glLoadIdentity()
        fx = calibration_mat[0, 0]
        fy = calibration_mat[1, 1]
        # calculate angle of view
        fovy = 2 * np.arctan(0.5 * self.height / fy) * 180 / np.pi
        aspect = (self.width * fy) / (self.height * fx)
        # set perspective
        gluPerspective(fovy, aspect, self.near_clipping_pane, self.far_clipping_pane)
        glViewport(0, 0, self.width, self.height)

    def __update_modelview(self):
        """ Set the model view matrix from camera pose. """

        glMatrixMode(GL_MODELVIEW)
        glLoadIdentity()

        # We move the camera by moving the world in the opposite direction
        # We deduct how to move the world by the camera position
        camera_pose = self.scene.cam.pose
        world_pose = copy.deepcopy(camera_pose)
        # world_pose.yaw = np.pi/2 - camera_pose.yaw
        # world_pose.pitch = np.pi/2 - camera_pose.pitch
        # world_pose.roll = np.pi/2 - camera_pose.roll

        M = world_pose.transfmat
        M[:3, :] = -M[:3, :]

        # fit to open gl coordinate system
        M[0, :] = -M[0, :]
        M[1, :] = -M[1, :]
        # transpose and flatten to get column order
        M = M.T
        m = M.flatten()
        # replace model view with the new matrix
        glLoadMatrixf(m)

    def __draw(self):
        for o, p in self.scene.objects:
            o.draw(p)

    def screen_shot(self):
        glReadBuffer(GL_FRONT)
        pixels = (GLubyte * (3 * self.width * self.height))(0)
        glReadPixels(0, 0, self.width, self.height, GL_RGB, GL_UNSIGNED_BYTE, pixels)
        return parse_screenshot(pixels, self.width, self.height)

    def refresh(self):

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        self.__update_modelview()
        self.__draw()

    def toggle_light(self, n: int):
        if not self.light_state[n]:
            glEnable(self.light_sources[n])
            self.light_state[n] = True
        else:
            glDisable(self.light_sources[n])
            self.light_state[n] = False

    def refresh_lights(self):
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse)
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular)
        glLightf(GL_LIGHT0, GL_POSITION, light_position)
        glEnable(GL_LIGHT0)

    @property
    def lights(self):
        return self.scene.lights

    @lights.setter
    def lights(self, lights: [Light]):
        pass
        # for i, l in enumerate(self.light_sources):
        #     glDisable(l)
        #     self.light_state[i] = False
        #
        # if lights is not None:
        #     self.scene.lights = lights
        #     for i, l in enumerate(self.scene.lights, start=0):
        #         light_ambient = [0.0, 0.0, 0.0, 1.0]
        #         light_diffuse = [l.rgb[0], l.rgb[1], l.rgb[2], 1.0]
        #         light_specular = [l.rgb[0], l.rgb[1], l.rgb[2], 1.0]
        #         light_position = [l.pos[0], l.pos[1], l.pos[2], 0.0]
        #
        #         glLightfv(self.light_sources[i], GL_AMBIENT, light_ambient)
        #         glLightfv(self.light_sources[i], GL_DIFFUSE, light_diffuse)
        #         glLightfv(self.light_sources[i], GL_SPECULAR, light_specular)
        #         glLightf(self.light_sources[i], GL_POSITION, light_position)
        #         glEnable(self.light_sources[i])
        #         self.light_state[i] = True
