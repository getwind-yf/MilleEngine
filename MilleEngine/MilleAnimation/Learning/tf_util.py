
import tensorflow as tf 
import numpy as np 
import os 

xavier_initializer = tf.contrib.layers.xavier_initializer() 

def disable_gpu():
	os.environ["CUDA_VISIBLE_DEVICES"] = '-1'
	return 

def var_shape(x):
	out = [k.value for k in x.get_shape()]
	assert all(isinstance(a, int) for a in out), "shape function assumes that shape is fully known"
	return out 

def intprod(x):
	return int(np.prod(x)) 

