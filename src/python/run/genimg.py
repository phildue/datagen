import numpy as np

from samplegen.imggen.RandomImgGen import RandomImgGen
from samplegen.setanalysis.SetAnalyzer import SetAnalyzer
from samplegen.shotgen.ShotLoad import ShotLoad
from utils.fileaccess.labelparser.DatasetParser import DatasetParser
from utils.fileaccess.utils import create_dirs
from utils.timing import tic, toc, tuc
from utils.workdir import cd_work

cd_work()

background_path = ["resource/backgrounds/cyberzoo_empty/"]
# background_path = ["resource/backgrounds/google-fence-gate-industry/"]

# background_path = "samplegen/resource/backgrounds/single"
# background_path = "samplegen/resource/backgrounds/single/"
# sample_path = "resource/samples/single_background_test/"
sample_path = "resource/ext/samples/test/"
shot_path = "resource/shots/test/"

n_backgrounds = 100
batch_size = 10
output_shape = (160, 315)
n_batches = int(np.ceil(n_backgrounds / batch_size))
create_dirs([sample_path])
tic()

shot_loader = ShotLoad(shot_path, img_format='bmp')
set_writer = DatasetParser.get_parser(sample_path, image_format='jpg', label_format='xml', start_idx=20000,color_format='bgr')

augmenter = None  # RandomEnsemble(augmenters=[
# (1.0, TransformResize((160, 315))),
# (1.0, TransformDistort(BarrelDistortion.from_file('resource/distortion_model_est.pkl'), crop=0.1))])
generator = RandomImgGen(background_path,
                         output_shape=output_shape,
                         preprocessor=augmenter)
for i in range(n_batches):
    tic()
    shots, shot_labels = shot_loader.get_shots(n_shots=250)
    samples, labels = generator.generate(shots, shot_labels, n_backgrounds=batch_size)

    set_writer.write(samples, labels)
    toc("Batch: {1:d}/{2:d} - {0:d}/{3:d} samples generated ".format(
        int(len(samples) * (i + 1)), i, n_batches, int(n_batches * batch_size)))
    tuc("Total time: ")

toc("Finished. Total run time ")

SetAnalyzer(output_shape, shot_path).show_summary()
