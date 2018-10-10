import numpy as np

from samplegen.scene.Scene import Scene
from samplegen.setanalysis.SetAnalyzer import SetAnalyzer
from samplegen.shotgen.GateGen import GateGen
from samplegen.shotgen.ShotCreate import ShotCreate
from samplegen.shotgen.engine3d.SceneEngine import SceneEngine
from samplegen.shotgen.engine3d.opengl.GLSquare import GLSquare
from samplegen.shotgen.engine3d.opengl.GateGLLarge import GateGLThickLarge
from samplegen.shotgen.engine3d.opengl.GateGLTall import GateGLTall
from samplegen.shotgen.lightgen.ConstantLightGen import ConstantLightGen
from samplegen.shotgen.lightgen.RandomLightGen import RandomLightGen
from samplegen.shotgen.positiongen.RandomPositionGen import RandomPositionGen
from utils.fileaccess.labelparser.DatasetParser import DatasetParser
from utils.fileaccess.utils import create_dirs
from utils.timing import tic, toc
from utils.workdir import cd_work

cd_work()

name = "test"
shot_path = "resource/shots/" + name + "/"

N = 0
n_positions = 100 - N * 100
n_batches = 10 - N
cam_range_side = (-1, 1)
cam_range_forward = (0, 5)
cam_range_lift = (-0.5, 1.5)
cam_range_pitch = (-0.1, 0.1)
cam_range_roll = (-0.5, 0.5)
cam_range_yaw = (-np.pi, np.pi)

n_light_range = (4, 6)
n_gate_range = (2, 3)

gate_pos_range_z = (-5, 5)
gate_pos_range_x = (-4, 4)

width, height = (640, 640)

gate_gen = GateGen(gates=[GateGLTall(), GateGLThickLarge(), GLSquare(), GLSquare()], n_gate_range=n_gate_range,
                   forw_gate_range=gate_pos_range_z
                   , side_gate_range=gate_pos_range_x, min_gate_dist=(2, 2))

position_gen = RandomPositionGen(range_dist_side=cam_range_side,
                                 range_dist_forward=cam_range_forward,
                                 range_lift=cam_range_lift,
                                 range_pitch=cam_range_pitch,
                                 range_roll=cam_range_roll,
                                 range_yaw=cam_range_yaw)

light_gen = ConstantLightGen((0, 0, 0))

scene = Scene()
scene_engine = SceneEngine(scene, width=width, height=height)
shot_creator = ShotCreate(position_gen, light_gen, scene_engine, perc_empty=0.1)

create_dirs([shot_path])
set_writer = DatasetParser.get_parser(shot_path, image_format='bmp', label_format='xml', start_idx=N * 100,color_format='bgr')

for i in range(n_batches):
    tic()
    scene.objects = gate_gen.generate()
    shots, labels = shot_creator.get_shots(int(n_positions / n_batches))

    set_writer.write(shots, labels)

    toc("Batch: {0:d}/{2:d}, {1:d} shots generated after ".format(i + 1, len(shots), n_batches))

scene_engine.stop()

set_analyzer = SetAnalyzer((height, width), shot_path)
set_analyzer.show_summary()
