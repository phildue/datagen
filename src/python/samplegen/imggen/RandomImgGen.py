import glob
import random
from scipy import signal

import numpy as np

from samplegen.imggen.ImgGen import ImgGen
from utils.imageprocessing.Backend import imread, replace_background, blur, noisy, convert_color, COLOR_BGR2YUV, resize
from utils.imageprocessing.Image import Image
from utils.imageprocessing.transform.ImgTransform import ImgTransform
from utils.labels.ImgLabel import ImgLabel


class RandomImgGen(ImgGen):
    def __init__(self, background_path="../resource/backgrounds",
                 output_shape=(416, 416),
                 preprocessor: ImgTransform = None, background_color=(0, 0, 0), postprocessor: ImgTransform = None,
                 merge_kernels=None):
        self.merge_kernels = merge_kernels
        self.postprocessor = postprocessor
        self.background_color = background_color
        self.output_shape = output_shape
        paths = background_path if isinstance(background_path, list) else [background_path]
        self.files = [f for folder in [glob.glob(p + "/*.jpg") for p in paths] for f in folder]
        self.preprocessor = preprocessor

    def generate(self, shots: [Image], labels: [ImgLabel], n_backgrounds=10) -> (
            [Image], [ImgLabel]):
        labels_created = []
        samples = []
        for j in range(n_backgrounds):
            background = imread(random.choice(self.files), 'bgr')
            i = random.randint(0, len(shots) - 1)
            shot = shots[i]
            label = labels[i]

            shot, label = resize(shot, background.shape[:2], label=label)

            if self.preprocessor:
                shot, label = self.preprocessor.transform(shot, label)

            img = self.merge(shot, background, kernels=self.merge_kernels)
            img, label = resize(img, shape=self.output_shape, label=label)

            if self.postprocessor:
                shot, label = self.preprocessor.transform(shot, label)

            samples.append(img)
            labels_created.append(label)

        return samples, labels_created

    def merge(self, shot: Image, background: Image, kernels=[]):

        new_img = background.copy()
        idx_fg = shot.array != self.background_color
        new_img.array[idx_fg] = shot.array[idx_fg]

        if not np.any(idx_fg) or not kernels:
            return new_img
        else:
            new_img = self.convolve(idx_fg, new_img, kernels)

        return new_img

    def convolve(self, idx_fg, new_img, kernels):
        new_img = new_img.copy()
        h, w = new_img.shape[:2]

        idx_y = np.mgrid[:h, :w][0][idx_fg[:, :, 0]]
        idx_x = np.mgrid[:h, :w][1][idx_fg[:, :, 0]]

        for kernel in kernels:

            k = kernel / np.sum(kernel)
            k_offset = int(np.floor((k.shape[0] - 1) / 2))
            padded = np.zeros((h + 2 * k_offset, w + 2 * k_offset, 3))

            # padding
            padded[:k_offset] = padded[k_offset:2 * k_offset]
            padded[-k_offset:] = padded[-2 * k_offset:-k_offset]

            padded[:, :k_offset] = padded[:, k_offset:2 * k_offset]
            padded[:, -k_offset:] = padded[:, -2 * k_offset:-k_offset]

            padded[k_offset:-k_offset, k_offset:-k_offset] = new_img.array.copy()
            kernel_stretched = np.reshape(k, (-1,))
            for idx in range(len(idx_x)):
                x, y = idx_x[idx], idx_y[idx]
                patch = padded[y + 1 - k_offset:y + 1 + k_offset + 1, x + 1 - k_offset:x + 1 + k_offset + 1]
                try:
                    new_img.array[y, x, 0] = kernel_stretched.T.dot(patch[:, :, 0].reshape((-1,)))
                    new_img.array[y, x, 1] = kernel_stretched.T.dot(patch[:, :, 1].reshape((-1,)))
                    new_img.array[y, x, 2] = kernel_stretched.T.dot(patch[:, :, 2].reshape((-1,)))
                except ValueError:
                    pass
                    # FIXME whats causing this?
                    # print(patch)
        return new_img
