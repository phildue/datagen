from samplegen.scene.Gate import Gate
from samplegen.scene.GateThin250 import Gate250
from samplegen.shotgen.engine3d.opengl.OpenGlView import OpenGlView


class Person(OpenGlView):
    def __init__(self, model: Gate = Gate250(), path: str = "resource/Old_man/", filename: str = "muro.obj"):
        super().__init__(model, path, filename)
