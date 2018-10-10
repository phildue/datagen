from abc import ABC, abstractmethod

from utils.labels.GateCorners import GateCorners


class Gate(ABC):
    @property
    @abstractmethod
    def corners(self) -> GateCorners:
        pass
