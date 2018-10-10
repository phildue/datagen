from samplegen.scene.Gate import Gate
from utils.labels.GateCorners import GateCorners


class Gate250(Gate):
    def __init__(self, gate_corners: GateCorners = GateCorners(center=(0, 0, 0),
                                                               top_left=(-0.71, 0.69, 0),
                                                               top_right=(0.71, 0.69, 0),
                                                               bottom_right=(0.71, -0.69, 0),
                                                               bottom_left=(-0.71, -0.69, 0))):
        self.gate_corners = gate_corners

    @property
    def corners(self) -> GateCorners:
        return self.gate_corners
