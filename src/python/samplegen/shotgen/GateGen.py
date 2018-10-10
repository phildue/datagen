import random

import numpy as np

from samplegen.shotgen.engine3d.opengl.OpenGlView import OpenGlView
from utils.labels.Pose import Pose


class GateGen:
    def __init__(self, gates: [OpenGlView],
                 n_gate_range=(0, 10),
                 forw_gate_range=(-10, 10),
                 side_gate_range=(-5, 5),
                 yaw_range=np.linspace(0, 2 * np.pi, 180),
                 min_gate_dist=(2, 2)):
        self.gates = gates
        self.yaw_range = yaw_range

        self.forw_gate_range = list(np.linspace(forw_gate_range[0], forw_gate_range[1],
                                                (forw_gate_range[1] - forw_gate_range[0]) / min_gate_dist[1]))
        self.side_gate_range = list(np.linspace(side_gate_range[0], side_gate_range[1],
                                                (side_gate_range[1] - side_gate_range[0]) / min_gate_dist[0]))
        self.n_gate_range = n_gate_range

    def generate(self):
        n_gates = random.randint(self.n_gate_range[0], self.n_gate_range[1])

        gates = []
        forw_dists = random.sample(self.forw_gate_range, n_gates)
        side_dists = random.sample(self.side_gate_range, n_gates)

        for i in range(n_gates):
            gate = random.choice(self.gates)
            position = Pose(north=forw_dists[i], east=side_dists[i], yaw=random.choice(self.yaw_range))
            gates.append((gate, position))

        return gates
