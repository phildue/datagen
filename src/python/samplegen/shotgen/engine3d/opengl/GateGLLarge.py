from samplegen.scene.Gate import Gate
from samplegen.scene.GateLarge import GateLarge
from samplegen.shotgen.engine3d.opengl.OpenGlView import OpenGlView


class GateGLThickLarge(OpenGlView):
    def __init__(self, model: Gate = GateLarge(), path: str = "resource/gates/", filename: str = "gate2016_large.obj"):
        super().__init__(model, path, filename)
