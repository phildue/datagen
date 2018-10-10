from samplegen.scene.Light import Light
from samplegen.shotgen.lightgen.LightGen import LightGen


class ConstantLightGen(LightGen):
    def __init__(self, lights: [Light]):
        self.lights = lights

    def gen_lights(self):
        return self.lights
