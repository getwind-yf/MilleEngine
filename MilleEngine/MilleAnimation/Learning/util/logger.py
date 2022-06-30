
import util.mpi_util as MPIUtil 

import os.path as osp, shutil, time, atexit, os, subprocess 

class Logger:
	def print(str):
		if (MPIUtil.is_root_proc()):
			print(str) 
		return 

	def __init__(self):
		self.output_file = None
		self.first_row = True
		self.log_headers = []
		self.log_current_row = {}
		self._dump_str_template = ""
		return

	def reset(self):
		self.first_row = True
		self.log_headers = []
		self.log_current_row = {}
		if self.output_file is not None:
			self.output_file = open(output_file, 'w')
		return 

	def configure_output_file(self, filename=None):
		return
