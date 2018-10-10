import random

from samplegen.scene.Light import Light
from samplegen.shotgen.lightgen.LightGen import LightGen


class RandomLightGen(LightGen):
    def __init__(self, range_side: (int, int), range_lift: (int, int), range_forward: (int, int),
                 rgb: (float, float, float) = (1.0, 0.9, 0.95), n_light_range=(3, 6)):
        self.n_light_range = n_light_range
        self.rgb = rgb
        self.range_forward = range_forward
        self.range_lift = range_lift
        self.range_side = range_side

    def gen_light(self) -> Light:
        position = (float(random.randint(self.range_side[0], self.range_side[1])),
                    float(random.randint(self.range_lift[0], self.range_lift[1])),
                    float(random.randint(self.range_forward[0], self.range_forward[1])))

        return Light(position, self.rgb)

    def gen_lights(self):
        n = random.randint(self.n_light_range[0], self.n_light_range[1])
        return [self.gen_light() for _ in range(n)]
