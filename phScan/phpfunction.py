import php
import phpsource

function_name_seperators = [' ', '\r', '\n', '\t', '(']

class phpFunctionError (Exception) :
	def __init__ (self, value) :
		self.value = value
	def __str__ (self) :
		return repr(self.value)


class PhpFunction :


	#raw_source = ''
	
	# source must begin with a function, doesn't matter what comes after
	# basepath is required for includes that may be contained within function
	def __init__ (self, source, basepath) :
	
		self.name = ''
		self.arguments = []
		self.source = phpsource.PhpSource()
		self.total_size = 0
	
		if source[0] != 'function' :
			raise phpFunctionError('PhpFunction given source that did not start with function')
		
		# find the name of the function
		self.name = source[1]
		
		# find the first ( and ) enclosing the arguments
		arguments_begin_index = source.index('(')
		arguments_end_index = source.index(')')
		
		# get arguments
		self.arguments = []
		for a in source[arguments_begin_index:arguments_end_index] :
			if a != ',' :
				self.arguments.append(a)
		
		# find the first {
		opening_brace_index = source.index('{')
		
		# now get all text in the brace block
		source, closing_brace_index = php.brace_block(source[opening_brace_index:], return_index=True)
		
		self.source.from_source(source[1:], basepath)
		self.source.parse(in_php=True)
		
		self.total_size = opening_brace_index + closing_brace_index + 1
		#self.raw_source = text[:opening_brace_index + closing_brace_index + 1]
		
	def debug_output (self) :
		print 'name: ' + name
		print 'arguments: ' + arguments
		print 'source: ' + source
