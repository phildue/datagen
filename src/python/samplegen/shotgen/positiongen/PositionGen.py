from abc import abstractmethod, ABC


class PositionGen(ABC):
    @abstractmethod
    def gen_pos(self):
        pass
