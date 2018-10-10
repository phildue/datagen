from samplegen.shotgen.positiongen.PositionGen import PositionGen
from utils.labels.Pose import Pose


class TrajectoryGen(PositionGen):
    def __init__(self, waypoints: [Pose]):
        # TODO waypoint to trajectory calc
        self.trajectory = waypoints
        self.iterator = iter(self.trajectory)

    def gen_pos(self):
        try:
            p = next(self.iterator)
        except StopIteration:
            self.iterator = iter(self.trajectory)
            p = next(self.iterator)

        return p
