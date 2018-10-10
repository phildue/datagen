from samplegen.scene.Camera import Camera
from samplegen.scene.Light import Light
from samplegen.shotgen.engine3d.View import View
from utils.labels.Pose import Pose


class Scene:
    def __init__(self, cam: Camera = Camera(1000,1000), objects: [(View, Pose)] = None, lights: [Light] = None):
        self.lights = lights
        self.cam = cam
        self.objects = objects
