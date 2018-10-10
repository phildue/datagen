from samplegen.scene.Gate import Gate
from samplegen.scene.GateOpen import GateOpen
from samplegen.shotgen.engine3d.opengl.OpenGlView import OpenGlView


class GateGLOpen(OpenGlView):
    def __init__(self, model: Gate = GateOpen(), path: str = "resource/gates/", filename: str = "open_gate.obj"):
        super().__init__(model, path, filename)
