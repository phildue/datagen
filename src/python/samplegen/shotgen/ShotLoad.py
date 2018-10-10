import glob
import random

from samplegen.shotgen.ShotGen import ShotGen
from utils.fileaccess.labelparser.DatasetParser import DatasetParser
from utils.imageprocessing.Backend import imread


class ShotLoad(ShotGen):
    def __init__(self, shot_path: str, img_format='jpg', random=True, label_format='xml'):
        self.label_format = label_format
        self.random = random
        self.img_format = img_format
        self.path = shot_path

    def get_shots(self, n_shots=10):
        shots = []
        labels = []
        files = list(sorted(glob.glob(self.path + "*." + self.img_format)))
        if n_shots > len(files):
            print("Not enough shots available on hard drive, getting: " + str(len(files)))

        for i, f in enumerate(files):
            if i >= n_shots: break
            if self.random:
                f = random.choice(files)

            shots.append(imread(f, 'bgr'))
            labels.append(DatasetParser.read_label(self.label_format, f.replace(self.img_format, self.label_format)))
        return shots, labels
