from samplegen.scene.Gate import Gate
from utils.labels.GateCorners import GateCorners


class GateOpen(Gate):
    def __init__(self, gate_corners: GateCorners = GateCorners(center=(-0.1, -0.1, 0),
                                                               top_left=(-0.8, 0.6, 0),
                                                               top_right=(0.6, 0.6, 0),
                                                               bottom_right=(0.6, -0.8, 0),
                                                               bottom_left=(-0.8, -0.8, 0))):
        self.gate_corners = gate_corners

    @property
    def corners(self) -> GateCorners:
        return self.gate_corners
