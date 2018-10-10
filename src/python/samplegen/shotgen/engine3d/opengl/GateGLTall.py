from samplegen.scene.Gate import Gate
from samplegen.scene.GateTall import GateTall
from samplegen.shotgen.engine3d.opengl.OpenGlView import OpenGlView


class GateGLTall(OpenGlView):
    def __init__(self, model: Gate = GateTall(), path: str = "resource/gates/", filename: str = "gate2016.obj"):
        super().__init__(model, path, filename)
