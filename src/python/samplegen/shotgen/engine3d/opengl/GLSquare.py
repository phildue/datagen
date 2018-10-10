from samplegen.scene.Gate import Gate
from samplegen.scene.GateLarge import GateLarge
from samplegen.scene.GateTall import GateTall
from samplegen.shotgen.engine3d.opengl.OpenGlView import OpenGlView


class GLSquare(OpenGlView):
    def __init__(self, model: Gate = GateTall(), path: str = "resource/gates/", filename: str = "simple_square.obj"):
        super().__init__(model, path, filename)
