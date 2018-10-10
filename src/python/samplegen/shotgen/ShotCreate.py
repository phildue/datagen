import numpy as np

from samplegen.shotgen.ShotGen import ShotGen
from samplegen.shotgen.engine3d.PointProjector import PointProjector
from samplegen.shotgen.engine3d.SceneEngine import SceneEngine
from samplegen.shotgen.lightgen.LightGen import LightGen
from samplegen.shotgen.positiongen.PositionGen import PositionGen
from utils.labels.ImgLabel import ImgLabel


class ShotCreate(ShotGen):
    def __init__(self,
                 position_gen: PositionGen,
                 light_gen: LightGen,
                 scene_engine: SceneEngine,
                 gate_path="../resource/gates/",
                 shot_path="../resource/shots/",
                 projector: PointProjector = None, perc_empty=0.05):
        self.perc_empty = perc_empty
        if projector is None:
            projector = PointProjector(scene_engine.cam, scene_engine.graphic_engine.width,
                                       scene_engine.graphic_engine.height,
                                       near_clipping_plane=scene_engine.graphic_engine.near_clipping_pane)

        self.projector = projector
        self.gate_path = gate_path
        self.shot_path = shot_path
        self.position_gen = position_gen
        self.scene_engine = scene_engine
        self.light_gen = light_gen

    def get_shots(self, n_positions=10):
        shots = []
        labels = []
        n_empty = 0
        n_empty_max = int(np.ceil(n_positions * self.perc_empty))
        while len(shots) < n_positions:
            self.scene_engine.cam.pose = self.position_gen.gen_pos().to_scene_unit
            obj_labels = self.projector.create_labels(self.scene_engine.scene.objects)

            if len(obj_labels) is 0:
                if n_empty >= n_empty_max:
                    continue
                else:
                    n_empty += 1

            self.scene_engine.lights = self.light_gen.gen_lights()
            self.scene_engine.refresh(1)
            shots.append(self.scene_engine.graphic_engine.screen_shot())
            labels.append(ImgLabel(obj_labels))

        return shots, labels
