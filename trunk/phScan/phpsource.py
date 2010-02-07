'''
	Basic parsing of php
	The next real work horse after php.py
'''
import os

import php
import phpfunction

algebraic_operators = ['+', '-', '/', '%']
statement_seperators = ['=', ',', '(', ')', '{', '}', '->', '\\', '\'', '"', ';']
string_literals = ["'", '"']

class PhpSource :

	def __init__ (self) :

		# basepath is the path from which include statements will try to include
		# things.
		self.basepath = ''
		'''
			The entire text is broken up and placed into source
			Normally we refer to text broken into pieces as source
		'''
		self.source = []
		'''
			Holds PhpFunction objects for all objects that can be found in
			self.source. The PhpFunction objects for included files will be
			found in the PhpSource objects in self.includes
		'''
		self.functions = []
		'''
			A list of statements we keep track of, in the order they appear in
			self.source
		'''
		self.statements = []
		'''
			When we come across an include statement, we create a PhpSource object, give
			it the filename, call parse and save it in includes. The key corresponds to
			the filename, value points to the PhpSource object
			Actually, this is taken care of in do_includes, and things don't happen in the
			order described (well maybe not)
			++Actually just see do_includes()
		'''
		self.includes = {}
		
	def __repr__ (self) :
		return php.format_source(self.source)
	
	# basepath is the base path of where the original file is
	# it must be included because any include statements we encounter are going to
	# include from that basepath
	def from_raw_text (self, text, basepath=os.getcwd()) :
		self.basepath = basepath
		self.source = php.split(text, php.all_whitespace, statement_seperators)
		
	def from_source (self, source, basepath=os.getcwd()) :
		self.source = source
		self.basepath = basepath
	
	
	def from_filename (self, filename) :
		print 'from filename ' + filename
		fh = open(filename, 'r')
		text = fh.read()
		fh.close()
		self.from_raw_text(text, os.path.split(os.path.abspath(filename))[0])
	
	
	'''
		in_php determines whether we begin inside a <?php tag.
		including a file? in_php = False.
		parsing contents of a file? in_php = True.
	'''
	def parse (self, in_php = False) :
	
		# for now, there are certain elements we want to skip
		# we add them here
		skip = []

		statement_source = []
		in_string_literal = False
		item_i = 0
		while item_i < len(self.source) :
			if item_i in skip :
				continue
		
			#print str(in_php) + ' ' + self.source[item_i]
			
			# <?php
			if self.source[item_i] == '<?php' :
				in_php = True
				item_i += 1
				continue
			
			if in_php :
				if self.source[item_i] == 'php?>' or self.source[item_i] == '?>' :
					in_php = False
				
				# string literal
				if self.source[item_i] == '\'' or self.source[item_i] == '"' :
					if self.source[item_i - 1] != '\\' :
						if in_string_literal :
							in_string_literal = False
						else :
							in_string_literal = True
				if in_string_literal :
					statement_source.append(self.source[item_i])
					item_i += 1
					continue
					
				
				# include
				if self.source[item_i] == 'include' :
					statement_source.append('include');
				
				# function
				elif self.source[item_i] == 'function' :
					self.functions.append(phpfunction.PhpFunction(self.source[item_i:], self.basepath))
					item_i += self.functions[-1].total_size
					print 'function loaded ' + self.functions[-1].name
					continue
					
				# for/while loop
				# for now we're just going to eliminate the loops
				elif self.source[item_i] == 'for' or self.source[item_i] == 'while' :
					opening_brace = self.source[item_i:].index('{')
					tmp, closing_brace = php.brace_block(self.source[item_i + opening_brace:], return_index=True)
					skip.append(self.source[item_i + opening_brace + closing_brace])
					item_i = item_i + opening_brace
					
				# end of a statement
				elif self.source[item_i] == ';' :
					statement_source.append(';')
					statement_source = self.statement_associative_arrays(statement_source)
					self.statements.append(statement_source)
					statement_source = []
					
				else :
					statement_source.append(self.source[item_i])
					
		
			item_i += 1
		self.do_includes()
		
	
	def do_includes (self) :
		for statement in self.statements :
			if statement[0] == 'include' :
				# we make a really crappy assumption that we have something along the lines
				# of: include 'filename.php'; and we are only going to include files of that
				# format
				if statement[1] in string_literals and statement[3] in string_literals :
					self.includes[statement[2]] = PhpSource()
					self.includes[statement[2]].from_filename(self.basepath + '/' + statement[2])
					self.includes[statement[2]].parse()
	
			
	'''
		because associative arrays can have string literals, they will be 
		seperated over multiple list elements. we want them as one list element.
		this checks a statement for an associative array, and if one is found,
		rewrites the statement so the associative array is in one element.
		returns a correct statement in respect to associative arrays
	'''
	def statement_associative_arrays (self, source) :
		i = 0
		while i < len(source) :
			if source[i][0] == '$' and source[i][-1] == '[' :
				end_of_associative_array = source[i:].index(']')
				associative_array = ''.join(source[i:i+end_of_associative_array + 1])
				source = source[:i] + [associative_array] + source[i + end_of_associative_array + 1:]
			i += 1
		return source
		
	
	
	'''
		PhpFile calls this to help fill its functions dict
	'''
	def get_functions (self) :
		functions = {}
		for function in self.functions :
			functions[function.name] = function
		for include in self.includes :
			functions.update(self.includes[include].get_functions())
		return functions
	
	
	
	
	'''
		returns all instances of the variable name in this php source's statements
	'''
	def find_variable (self, variable_name) :
		statements = self.source.statements
		variable_statements = []
		for statement_i in range(len(statements)) :
			if variable_name in statements[statement_i] :
				variable_statements.append(statements[statement_i])
		return variable_statements
