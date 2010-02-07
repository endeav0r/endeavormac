'''
	For loading of php files. Create PhpFile object for the entry point to your
	php script you want to analyze, and manipulate it from here
'''

import os

import php
import phpfunction
import phpsource


class PhpFile :
	
	raw_source = False
	source = False
	'''
		PhpSource has a functions variable too, that holds a PhpFunction
		object. This holds a pointer to an element in a PhpSource's function
		where the key corresponds to the function name, and the value points
		to the PhpFunction object.
		This, of course, implies all functions are in the global scope
	'''
	functions = {}
	
	def __init__ (self, filename) :
		self.source = phpsource.PhpSource()
		self.source.from_filename(filename)
		self.source.parse()
		self.functions = self.source.get_functions()
	
	# find statements with the variable in the script
	def trace_variable (self, variable_name) :
		statements = self.source.find_variable(variable_name)
