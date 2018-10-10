import glob

import numpy as np

from modelzoo.backend.visuals.plots.BaseHist import BaseHist
from samplegen.setanalysis.SetAnalyzer import SetAnalyzer
from utils.imageprocessing.Backend import imread, brightness
from utils.timing import tic, toc
from utils.workdir import cd_work

cd_work()
tic()
path = 'resource/ext/samples/bebop20k/'
img_shape = (180, 315)
set_analyzer = SetAnalyzer(img_shape, path)
toc()
set_analyzer.show_summary()
files = list(sorted(glob.glob(path + '*.jpg')))
# files = files[:20]
# #images = [brightness(imread(img, 'bgr'),0.5) for img in files]
# images = [imread(img,'bgr') for img in files]
#
# means = np.zeros((len(images), 3))
# for i, img in enumerate(images):
#     means[i] = np.mean(img.array.reshape(-1, 3))
# mean = np.mean(means, -1)
# BaseHist(mean, n_bins=20, x_label='Gray Value').show()
