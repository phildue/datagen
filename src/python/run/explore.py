import numpy as np

from samplegen.scene.Camera import Camera
from samplegen.scene.Scene import Scene
from samplegen.scene.Square import Square
from samplegen.shotgen.GateGen import GateGen
from samplegen.shotgen.engine3d.Explorer import Explorer
from samplegen.shotgen.engine3d.SceneEngine import SceneEngine
from samplegen.shotgen.engine3d.opengl.GLSquare import GLSquare
from samplegen.shotgen.engine3d.opengl.GateGLLarge import GateGLThickLarge
from samplegen.shotgen.engine3d.opengl.GateGLOpen import GateGLOpen
from samplegen.shotgen.engine3d.opengl.GateGLTall import GateGLTall
from samplegen.shotgen.engine3d.opengl.Person import Person
from samplegen.shotgen.positiongen.RandomPositionGen import RandomPositionGen
from utils.fileaccess.utils import create_dirs
from utils.labels.Pose import Pose
from utils.workdir import cd_work

cd_work()

cam = Camera(focal_length_x=1000, focal_length_y=1000, init_pose=Pose(north=5))

gate_path = "resource/gates/"
gate_file = "gate250.obj"

gate_1 = (GateGLOpen(), Pose().to_scene_unit)
gate_2 = (GateGLThickLarge(), Pose().to_scene_unit)
gate_3 = (GLSquare(), Pose().to_scene_unit)
person = (Person(), Pose().to_scene_unit)
gate_generator = GateGen(gates=[GateGLThickLarge(), GateGLTall()], n_gate_range=(3, 8))

gates = [gate_3]
# gates = gate_generator.generate()
scene_engine = SceneEngine(Scene(cam, objects=person))

cam_range_side = (-1, 1)
cam_range_forward = (0, 10)
cam_range_lift = (-0.5, 1.0)
cam_range_pitch = (-np.pi / 4, np.pi / 4)
cam_range_roll = (-np.pi / 4, np.pi / 4)
cam_range_yaw = (-np.pi, np.pi)

n_gate_range = (2, 4)
gate_pos_range = (0, 10)
position_gen = RandomPositionGen(range_dist_side=cam_range_side,
                                 range_dist_forward=cam_range_forward,
                                 range_lift=cam_range_lift,
                                 range_pitch=cam_range_pitch,
                                 range_roll=cam_range_roll,
                                 range_yaw=cam_range_yaw)

shot_path = 'samplegen/resource/shots/stream_recorded/'
create_dirs([shot_path])
recorder = None  # SetFileParser(shot_path, img_format='bmp', label_format='pkl', start_idx=0)
explorer = Explorer(scene_engine, position_gen=position_gen, recorder=recorder)
explorer.event_loop()
# save(explorer.trajectory, 'recorded_trajectroy.pkl', shot_path)
# 3.341|Side-dist:-0.221|Lift:0.883|
# Roll:-0.434|Pitch:-0.264|Yaw:-0.334|
