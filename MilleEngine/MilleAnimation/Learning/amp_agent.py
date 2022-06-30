
import numpy as np
import tensorflow as tf 

from env.env import Env 
import learning.tf_util 


class AMPAgent(PPOAgent):
	
	NAME = "AMP"

	def __init__(self, id, world, json_data):
		super().__init__(id, world, json_data)

        self._disc_reward_mean = 0.0
        self._disc_reward_std = 0.0
        self._reward_min = np.inf
        self._reward_max = -np.inf
		self._build_disc_replay_buffer()
		return 

	def __str__(self):
		info_str = super().__str__()
		info_str = info_str[:-2]+ ',\n "AMPObsDim": "{:d}"'.format(self._get_amp_obs_size()) + info_str[-2:]
		return info_str
		pass

	def _load_params(self):
		pass

