import numpy as np
from sklearn.cluster import KMeans

from utils.BoundingBox import BoundingBox
from utils.fileaccess.labelparser.DatasetParser import DatasetParser


class SetAnalyzer:
    def __init__(self, img_shape, path):
        self.img_shape = img_shape
        if isinstance(path,list):
            self.labels = []
            for p in path:
                self.label_reader = DatasetParser.get_parser(p, 'xml', color_format='bgr')
                self.labels.extend(self.label_reader.read()[1])
        else:
            self.label_reader = DatasetParser.get_parser(path, 'xml', color_format='bgr')
            _, self.labels = self.label_reader.read()

    def get_label_map(self):

        y_limit = self.img_shape[0]
        label_sum = np.zeros(self.img_shape)
        for l in self.labels:
            label_map = np.zeros(self.img_shape)
            for o in l.objects:
                y_max = y_limit - o.y_min
                y_min = y_limit - o.y_max
                label_map[int(min((y_max, y_min))):int(max((y_max, y_min))), int(o.x_min): int(o.x_max)] = 1
            label_sum += label_map

        label_scaled = label_sum / np.max(label_sum)

        return Heatmap(label_scaled)

    def mean_n_objects(self):
        n_objects = 0
        for l in self.labels:
            n_objects += len(l.objects)
        return n_objects / len(self.labels)

    def area_distribution(self):
        box_dims = self.get_box_dims()
        area = box_dims[:, 0] * box_dims[:, 1]
        boxplot = BoxPlot(x_data=area, y_label='area', x_label='', title='Distribution of Bounding Box Areas')
        return boxplot

    def area_distribution_hist(self):
        box_dims = self.get_box_dims()
        print(len(box_dims))
        area = box_dims[:, 0] * box_dims[:, 1]
        boxplot = BaseHist(y_data=area, x_label='area', y_label='', title='Histogram of Bounding Box Areas',n_bins=10)
        return boxplot

    def get_box_dims(self):
        wh = []
        for label in self.labels:
            h, w = self.img_shape
            boxes = BoundingBox.from_label(label)
            for b in boxes:
                box_dim = np.array([b.w, b.h]) / np.array([w, h])
                box_dim = np.expand_dims(box_dim, 0)
                wh.append(box_dim)
        box_dims = np.concatenate(wh, 0)
        return box_dims

    def kmeans_anchors(self, n_boxes=5):
        box_dims = self.get_box_dims()
        # TODO move this to backend
        kmeans = KMeans(n_clusters=n_boxes).fit(box_dims)
        centers = kmeans.cluster_centers_
        scatter = BaseMultiPlot(x_data=[box_dims[:, 0], centers[:, 0]], y_data=[box_dims[:, 1], centers[:, 1]],
                                line_style=['bx', 'ro'],
                                x_label='width', y_label='height', y_lim=(0, 10))
        return scatter, kmeans

    def show_summary(self):
        heat_map = self.get_label_map()
        box_dims, _ = self.kmeans_anchors()
        mean_samples = self.mean_n_objects()
        area_distribution = self.area_distribution()
        print("Mean samples: ", mean_samples)
        box_dims.show(False)
        heat_map.show(False)
        area_distribution.show(True)
