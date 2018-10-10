from samplegen.scene.Gate import Gate
from utils.labels.GateCorners import GateCorners


class GateTall(Gate):
    def __init__(self, gate_corners: GateCorners = GateCorners(center=(0, 0, 0),
                                                               top_left=(-0.70, 0.67, 0),
                                                               top_right=(0.72, 0.67, 0),
                                                               bottom_right=(0.72, -0.67, 0),
                                                               bottom_left=(-0.70, -0.67, 0))):
        self.gate_corners = gate_corners

    @property
    def corners(self) -> GateCorners:
        return self.gate_corners
