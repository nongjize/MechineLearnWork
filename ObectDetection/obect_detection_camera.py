import numpy as np  
import os  
import six.moves.urllib as urllib  
import sys  
import tarfile  
import tensorflow as tf  
import zipfile  
import matplotlib  
import cv2
matplotlib.use('Agg')
from collections import defaultdict  
from io import StringIO  
from matplotlib import pyplot as plt  
from PIL import Image  
from object_detection.utils import label_map_util  
from object_detection.utils import visualization_utils as vis_util
from absl import flags


flags.DEFINE_string('model', None, 'The model to use')
FLAGS = flags.FLAGS

##################### Load a (frozen) Tensorflow model into memory.
def main(unused_argv):
    cap = cv2.VideoCapture(0)  # 打开摄像头
    #PATH_TO_CKPT = 'frozen_inference_graph_9298.pb'FLAGS.model_dir
    PATH_TO_CKPT = FLAGS.model
    PATH_TO_LABELS = 'object-detection-sg.pbtxt'
    NUM_CLASSES = 1
    print('Loading model...')
    detection_graph = tf.Graph()
    with detection_graph.as_default():
        od_graph_def = tf.GraphDef()
        with tf.gfile.GFile(PATH_TO_CKPT, 'rb') as fid:
            serialized_graph = fid.read()
            od_graph_def.ParseFromString(serialized_graph)
            tf.import_graph_def(od_graph_def, name='')

    ##################### Loading label map
    print('Loading label map...')
    label_map = label_map_util.load_labelmap(PATH_TO_LABELS)
    categories = label_map_util.convert_label_map_to_categories(label_map, max_num_classes=NUM_CLASSES, use_display_name=True)
    category_index = label_map_util.create_category_index(categories)
    print('Detecting...')
    with detection_graph.as_default():
        with tf.Session(graph=detection_graph) as sess:
            while True:
                ret, image_np = cap.read()           #从摄像头中获取每一帧图像
                image_np_expanded = np.expand_dims(image_np, axis=0)
                image_tensor = detection_graph.get_tensor_by_name('image_tensor:0')
                boxes = detection_graph.get_tensor_by_name('detection_boxes:0')
                scores = detection_graph.get_tensor_by_name('detection_scores:0')
                classes = detection_graph.get_tensor_by_name('detection_classes:0')
                num_detections = detection_graph.get_tensor_by_name('num_detections:0')
                # Actual detection.
                (boxes, scores, classes, num_detections) = sess.run([boxes, scores, classes, num_detections],feed_dict={image_tensor: image_np_expanded})
                # Print the results of a detection.
                #print(scores)
                #print(classes)
                #print(category_index)
                vis_util.visualize_boxes_and_labels_on_image_array(
                    image_np,
                    np.squeeze(boxes),
                    np.squeeze(classes).astype(np.int32),
                    np.squeeze(scores),
                    category_index,
                    use_normalized_coordinates=True,
                    line_thickness=8)

                cv2.imshow('object detection', cv2.resize(image_np, (800, 600)))
                #cv2.waitKey(0)
                if cv2.waitKey(25) & 0xFF == ord('q'):
                    cv2.destroyAllWindows()
                    break


if __name__ == '__main__':
    tf.app.run()