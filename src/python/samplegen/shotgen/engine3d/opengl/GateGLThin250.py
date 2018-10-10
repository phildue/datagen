from samplegen.scene.Gate import Gate
from samplegen.scene.GateThin250 import Gate250
from samplegen.shotgen.engine3d.opengl.OpenGlView import OpenGlView


class GateGLThin250(OpenGlView):
    def __init__(self, model: Gate = Gate250(), path: str = "resource/gates/", filename: str = "gate250.obj"):
        super().__init__(model, path, filename)
