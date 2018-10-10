import pygame

from samplegen.scene.Light import Light
from samplegen.scene.Scene import Scene
from samplegen.shotgen.engine3d.opengl.OpenGlEngine import OpenGlEngine


class SceneEngine:
    def __init__(self, scene: Scene, title="Sample Generator", width=1000, height=747,
                 graphic_engine: OpenGlEngine = None):
        self.running = True
        if graphic_engine is None:
            graphic_engine = OpenGlEngine(scene, width, height)
        self.graphic_engine = graphic_engine

        pygame.init()
        pygame.display.set_mode((width, height), pygame.OPENGL | pygame.DOUBLEBUF)
        pygame.display.set_caption(title)
        self.graphic_engine.init()
        self.MOVE_RATE = 0.1
        print("Initial Cam Position:")
        print(self.cam.pose.to_meters)

    def refresh(self, ms):
        self.graphic_engine.refresh()
        pygame.display.flip()
        pygame.time.wait(ms)

    def stop(self):
        pygame.quit()
        self.running = False

    @property
    def cam(self):
        return self.graphic_engine.scene.cam

    @property
    def scene(self):
        return self.graphic_engine.scene

    @property
    def lights(self):
        return self.graphic_engine.lights

    @lights.setter
    def lights(self, lights: [Light]):
        self.graphic_engine.lights = lights

    def screen_shot(self):
        return self.graphic_engine.screen_shot()
