import random

from samplegen.shotgen.positiongen.PositionGen import PositionGen
from utils.labels.Pose import Pose


class RandomPositionGen(PositionGen):
    def __init__(self, range_dist_side: (float, float), range_dist_forward: (float, float),
                 range_lift: (float, float), range_roll: (float, float), range_pitch: (float, float),
                 range_yaw: (float, float)):
        self.range_pitch = range_pitch
        self.range_roll = range_roll
        self.range_forward = range_dist_forward
        self.range_dist_side = range_dist_side
        self.range_lift = range_lift
        self.range_yaw = range_yaw

    def gen_pos(self) -> Pose:
        return Pose(east=random.uniform(self.range_dist_side[0], self.range_dist_side[1]),
                    north=random.uniform(self.range_forward[0], self.range_forward[1]),
                    up=random.uniform(self.range_lift[0], self.range_lift[1]),
                    roll=random.uniform(self.range_roll[0], self.range_roll[1]),
                    pitch=random.uniform(self.range_pitch[0], self.range_pitch[1]),
                    yaw=random.uniform(self.range_yaw[0], self.range_yaw[1]))
