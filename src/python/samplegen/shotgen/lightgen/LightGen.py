from abc import ABC, abstractmethod

from samplegen.scene.Light import Light


class LightGen(ABC):
    @abstractmethod
    def gen_lights(self) -> [Light]:
        pass
