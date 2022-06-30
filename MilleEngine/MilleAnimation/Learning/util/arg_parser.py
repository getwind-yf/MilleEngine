import re as RE

class ArgParser(object):
	global_parser = None

	def __init__(self):
		self._table = dict()
		return 

	def clear(self):
		self._table.clear()
		return 

	def load_args(self, arg_strs):
		succ = True
		vals = []
		curr_key = ''

		for str in arg_strs:
			if not (self._is_comment(str)):
				is_key = self._is_key(str)
				if(is_key):
					if(curr_key!=''):
						if(curr_key not in self._table):
							self._table[curr_key] = vals 
					vals = []
					curr_key = str[2::]
				else: 
					vals.append(str) 

		if(curr_key != ''): 
			if(curr_key not in self._table):
				self._table[curr_key] = vals 

			vals = []

		return succ 

	def load_file(self, filename): 
		succ = False 
		with open(filename, 'r') as file: 
			lines = RE.split(r'[\n\r]+', file.read())
			file.close() 

			arg_strs = [] 
			for line in lines:
				if(len(line) >0 and not self._is_comment(line)): 
					arg_strs += line.split() 
			succ = self.load_args(arg_strs)
		return succ 

	def has_key(self, key): 
		return key in self._table

	def parse_string(self, key, default =''): 
		str = default
		if self.has_key(key):
			str = self._table[key][0]
		return str

