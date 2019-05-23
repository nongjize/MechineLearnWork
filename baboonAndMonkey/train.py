import tensorflow as tf 
import numpy as np
from tensorflow.contrib.layers.python.layers import batch_norm
 
''' 1.数据集准备'''
# 取出数据集
filename_queue1 = tf.train.string_input_producer(["20190224_train.tfrecords"]) #读入流中
reader1 = tf.TFRecordReader()
_, serialized_example1 = reader1.read(filename_queue1)   #返回文件名和文件
features1 = tf.parse_single_example(serialized_example1,
                                   features={
                                       'label': tf.FixedLenFeature([], tf.int64),
                                       'img_raw' : tf.FixedLenFeature([], tf.string),
                                   })  #取出包含image和label的feature对象
image1 = tf.decode_raw(features1['img_raw'], tf.uint8)
image1 = tf.reshape(image1, [24, 24, 3])
label1 = tf.cast(features1['label'], tf.int32)
 
# 取出训练集 一定要使用shuffle_batch打乱顺序，否则训练过程中会出现精度0，1之间交替的情况
image_batch, label_batch = tf.train.shuffle_batch([image1, label1],batch_size = 128,capacity=2000,min_after_dequeue=1000)
 
filename_queue2 = tf.train.string_input_producer(["20190224_test.tfrecords"]) #读入流中
reader2 = tf.TFRecordReader()
_, serialized_example2 = reader2.read(filename_queue2)   #返回文件名和文件
features2 = tf.parse_single_example(serialized_example2,
                                   features={
                                       'label': tf.FixedLenFeature([], tf.int64),
                                       'img_raw' : tf.FixedLenFeature([], tf.string),
                                   })  #取出包含image和label的feature对象
image2 = tf.decode_raw(features2['img_raw'], tf.uint8)
image2 = tf.reshape(image2, [24, 24, 3])
label2 = tf.cast(features2['label'], tf.int32)
 
# 取出测试集 
images_test, labels_test = tf.train.shuffle_batch([image2, label2],batch_size = 512,capacity=2000,min_after_dequeue=1000)
 
''' 2.网络搭建 '''
def weight_variable(shape):
  initial = tf.truncated_normal(shape, stddev=0.1)
  return tf.Variable(initial)
 
def bias_variable(shape):
  initial = tf.constant(0.1, shape=shape)
  return tf.Variable(initial)
  
def conv2d(x, W):
  return tf.nn.conv2d(x, W, strides=[1, 1, 1, 1], padding='SAME')
 
def max_pool_2x2(x):
  return tf.nn.max_pool(x, ksize=[1, 2, 2, 1],
                        strides=[1, 2, 2, 1], padding='SAME')  
                        
def avg_pool_6x6(x):
  return tf.nn.avg_pool(x, ksize=[1, 6, 6, 1],
                        strides=[1, 6, 6, 1], padding='SAME')
                        
def batch_norm_layer(value,train = None, name = 'batch_norm'): 
  if train is not None:       
      return batch_norm(value, decay = 0.9,updates_collections=None, is_training = True)
  else:
      return batch_norm(value, decay = 0.9,updates_collections=None, is_training = False)
 
# 定义占位符
x = tf.placeholder(tf.float32, [None, 24, 24, 3]) # 输入为128*128*3
y = tf.placeholder(tf.float32, [None, 2]) # 2类
train = tf.placeholder(tf.float32)
 
# 定义网络结构
W_conv1 = weight_variable([5, 5, 3, 32])
b_conv1 = bias_variable([32])
 
x_image = tf.reshape(x, [-1,24,24,3])
 
h_conv1 = tf.nn.relu(batch_norm_layer((conv2d(x_image, W_conv1) + b_conv1),train))
h_pool1 = max_pool_2x2(h_conv1)
 
W_conv2 = weight_variable([5, 5, 32, 32])
b_conv2 = bias_variable([32])
 
h_conv2 = tf.nn.relu(batch_norm_layer((conv2d(h_pool1, W_conv2) + b_conv2),train))
h_pool2 = max_pool_2x2(h_conv2)
 
W_conv3 = weight_variable([5, 5, 32, 2])
b_conv3 = bias_variable([2])
h_conv3 = tf.nn.relu(conv2d(h_pool2, W_conv3) + b_conv3)
 
nt_hpool3=avg_pool_6x6(h_conv3)#2
nt_hpool3_flat = tf.reshape(nt_hpool3, [-1, 2])
 
y_conv = tf.contrib.layers.fully_connected(nt_hpool3_flat,2,activation_fn=tf.nn.softmax)
 
# 定义交叉熵
cross_entropy = -tf.reduce_sum(y * tf.log(y_conv))
#cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(labels=y, logits=y_conv))
 
#加入学习率退化
global_step = tf.Variable(0, trainable=False)
decaylearning_rate = tf.train.exponential_decay(0.04, global_step,1000, 0.9)
 
#定义优化器
train_step = tf.train.AdamOptimizer(decaylearning_rate).minimize(cross_entropy,global_step=global_step)
#train_step = tf.train.AdamOptimizer(1e-4).minimize(cross_entropy)
 
correct_prediction = tf.equal(tf.argmax(y_conv,1), tf.argmax(y,1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, "float"))
 
''' 3.开始训练'''
sess = tf.Session()
sess.run(tf.global_variables_initializer())
tf.train.start_queue_runners(sess=sess)
for i in range(15000):
	image_bth, label_b = sess.run([image_batch, label_batch])
 
	label_bth = np.eye(2, dtype=float)[label_b] #one hot
	#print(label_bth)
 
	train_step.run(feed_dict={x:image_bth, y: label_bth, train:1}, session=sess)
 
	if i % 200 == 0:
		train_accuracy = accuracy.eval(feed_dict={x:image_bth, y: label_bth}, session=sess)
		print( "step %d, training accuracy %g"%(i, train_accuracy))
image_bth, label_b = sess.run([images_test, labels_test])
label_bth = np.eye(2,dtype=float)[label_b]
print ("finished！ test accuracy %g"%accuracy.eval(feed_dict={
     x:image_bth, y: label_bth},session=sess))
