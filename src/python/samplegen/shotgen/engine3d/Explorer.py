import pygame

from samplegen.shotgen.engine3d.PointProjector import PointProjector
from samplegen.shotgen.engine3d.SceneEngine import SceneEngine
from samplegen.shotgen.positiongen.PositionGen import PositionGen
from utils.fileaccess.labelparser.DatasetParser import DatasetParser
from utils.imageprocessing.Imageprocessing import show
from utils.labels.ImgLabel import ImgLabel


class Explorer:
    def __init__(self, scene_engine: SceneEngine, projector: PointProjector = None, move_rate=0.1,
                 position_gen: PositionGen = None, recorder: DatasetParser = None,
                 ):

        self.recorder = recorder
        self.trajectory = []
        self.position_gen = position_gen
        if projector is None:
            projector = PointProjector(scene_engine.cam, scene_engine.graphic_engine.width,
                                       scene_engine.graphic_engine.height,
                                       near_clipping_plane=scene_engine.graphic_engine.near_clipping_pane)
        self.projector = projector

        self.running = True
        self.scene_engine = scene_engine
        self.move_rate = move_rate

    def event_loop(self):
        while self.running:
            self.scene_engine.refresh(20)
            events = pygame.event.get()
            for event in events:
                if event.type == pygame.KEYDOWN:
                    position = self.scene_engine.cam.pose
                    print("Cam Position:")
                    print(self.scene_engine.cam.pose.to_meters)

                    if self.recorder is not None:
                        img = self.scene_engine.screen_shot()
                        obj_labels = self.projector.create_labels(self.scene_engine.scene.objects)
                        label = ImgLabel(obj_labels)
                        self.recorder.write([img], [label])
                        self.trajectory.append(position.to_meters)

                    if event.key == pygame.K_LEFT:
                        position.yaw -= self.move_rate
                    if event.key == pygame.K_RIGHT:
                        position.yaw += self.move_rate
                    if event.key == pygame.K_UP:
                        position.pitch += self.move_rate
                    if event.key == pygame.K_DOWN:
                        position.pitch -= self.move_rate
                    if event.key == pygame.K_q:
                        position.roll -= self.move_rate
                    if event.key == pygame.K_e:
                        position.roll += self.move_rate
                    if event.key == pygame.K_w:
                        position.north += self.move_rate
                    if event.key == pygame.K_s:
                        position.north -= self.move_rate
                    if event.key == pygame.K_a:
                        position.east -= self.move_rate
                    if event.key == pygame.K_d:
                        position.east += self.move_rate
                    if event.key == pygame.K_r:
                        position.up += self.move_rate
                    if event.key == pygame.K_f:
                        position.up -= self.move_rate
                    if event.key == pygame.K_KP5:
                        position.roll = 0
                        position.pitch = 0
                        position.yaw = 0
                        position.east = -10
                        position.lift = 0
                        position.east = 0
                    if event.key == pygame.K_SPACE:
                        img = self.scene_engine.screen_shot()
                        obj_labels = self.projector.create_labels(self.scene_engine.scene.objects)
                        show(img, labels=ImgLabel(obj_labels))
                    if event.key == pygame.K_HASH:
                        img = self.scene_engine.screen_shot()
                        show(img)
                    if event.key == pygame.K_PLUS:
                        if self.position_gen is not None:
                            position = self.position_gen.gen_pos().to_scene_unit
                    if event.key == pygame.K_0:
                        self.scene_engine.graphic_engine.toggle_light(0)
                    if event.key == pygame.K_1:
                        self.scene_engine.graphic_engine.toggle_light(1)
                    if event.key == pygame.K_2:
                        self.scene_engine.graphic_engine.toggle_light(2)
                    if event.key == pygame.K_3:
                        self.scene_engine.graphic_engine.toggle_light(3)
                    if event.key == pygame.K_4:
                        self.scene_engine.graphic_engine.toggle_light(4)
                    if event.key == pygame.K_5:
                        self.scene_engine.graphic_engine.toggle_light(5)
                    if event.key == pygame.K_6:
                        self.scene_engine.graphic_engine.toggle_light(6)
                    if event.key == pygame.K_7:
                        self.scene_engine.graphic_engine.toggle_light(7)
                    if event.key == pygame.K_ESCAPE:
                        self.scene_engine.stop()

                    self.scene_engine.cam.pose = position
