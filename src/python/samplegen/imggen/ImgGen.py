from abc import ABC, abstractmethod

from utils.imageprocessing.Image import Image
from utils.labels.ImgLabel import ImgLabel


class ImgGen(ABC):
    @abstractmethod
    def generate(self, shots: [Image], labels: [ImgLabel], n_backgrounds=10) -> (
            [Image], [ImgLabel]):
        pass
