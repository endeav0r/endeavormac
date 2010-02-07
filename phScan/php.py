'''
	General purpose functions
'''


import re

all_whitespace = [' ', '\n', '\t', '\r']
line_whitepsace = [' ', '\t']

function_argument_seperators = [' ', '\r', '\n', '\t', ',']

control_structures = ['if', 'for', 'while']

class phpError (Exception) :
	def __init__ (self, value) :
		self.value = value
	def __str__ (self) :
		return repr(self.value)


# like str.split, except seperators is a list of seperators
# inclusive seperators will be seperated, but included in the result
def split(text, seperators, inclusive_seperators = []) :
	expression = '|'.join(seperators)
	result = re.split(expression, text)
	if len(inclusive_seperators) > 0 :
		# exponentially raise those inefficiences! (spelling?) ... ?
		result_i = 0
		while result_i < len(result) :
			for seperator in inclusive_seperators :
				while result[result_i].count(seperator) > 0 and len(result[result_i]) > 1 :
					r = result[:result_i]
					r.append(result[result_i][:result[result_i].index(seperator)])
					r.append(seperator)
					r.append(result[result_i][result[result_i].index(seperator) + 1:])
					r += result[result_i + 1:]
					result = r
			result_i += 1
	i = 0
	while i < len(result) :
		if result[i] == '' :
			del result[i]
		else :
			i += 1
	return result


'''
	Takes a tuple of whitespace possibilites and finds the next word from the
	beginning of text completely seperated by the whitespace (or EOF)
'''
def next_word (text, whitespace) :
	word = ''
	# 0 = searching for whitespace
	# 1 = searching for non-whitespace (beginning of next_word)
	# 2 = searching for whitespace (end of next_word)
	stage = 0
	# find the first piece of whitespace
	for i in range(len(text)) :
		if text[i] in whitespace :
			if stage == 0 :
				stage = 1
			elif stage == 2 :
				break
		else :
			if stage == 1 :
				word += text[i]
				stage = 2
			elif stage == 2 :
				word += text[i]
	return word



'''
	Given a list, where { and } are in seperated indices, returns all the 
	elements of the list within the braces, including the braces themselves
	identical to brace_block_text...
'''
def brace_block (source, return_index=False) :
	if source[0] != '{' :
		raise phpError('brace_block did not start with {')
	
	brace_count = 1
	for i in range(len(source[1:])) :
		if source[i + 1] == '}' :
			brace_count -= 1
			if brace_count == 0 :
				if return_index :
					return source[:i + 1], i + 1
				else :
					return source[:i + 1]
		elif source[i + 1] == '{' :
			brace_count += 1
	raise phpError('brace_block did not find closing brace')
					

'''
	Given a string with an opening brace as the first character, returns all
	text to respective closing brace, including opening and closing brace
'''
def brace_block_text (text, return_index=False) :
	if text[0] != '{' :
		raise phpError('brace_block_text did not start with {')
		
	brace_count = 1
	for i in range(len(text[1:])) :
		if text[i + 1] == '}' :
			brace_count -= 1
			if brace_count == 0 :
				if return_index :
					return text[:i + 1], i + 1
				else :
					return text[:i + 1]
		elif text[i + 1] == '{' :
			brace_count += 1
	raise phpError('brace_block_text did not find closing brace')



'''
	splits arguments into a list containing arguments name
	($one, $two, $three) returns ['$one', '$two', '$three']
	first character must be (
'''
def split_arguments (arguments_text) :
	if arguments_text[0] != '(' :
		raise phpError ('split_arguments first character was not (')
	if arguments_text.count(')') == 0 :
		raise phpError ('split_arguments text did not include a )')

	arguments_end_index = arguments_text.index(')')
	text = arguments_text[1:arguments_end_index - 2]
	arguments = split(text, function_argument_seperators)
	for i in range(len(arguments)) :
		if arguments[i] != '$' :
			del arguments[i]
		else :
			arguments[i].strip()
	return arguments
	
	
'''
	We story code in several seperated pieces, in a list. This formats a list
	of code in something easier for the user to read.
'''
def format_source (source) :
	text = ''
	indentation = 0
	
	for_loop = False
	in_string_literal = False
	
	for i in range(len(source)) :
	
		if source[i] == ';' or source[i] == '(' or source[i] == ')' :
			if text[-1] == ' ' :
				text = text[:-1]
		
		if source[i] == 'for' :
			for_loop = True
		
		# {
		if source[i] == '{' :
			indentation += 1
			text += '{\n' + ('\t' * indentation)
			for_loop = False
		# }
		elif source[i] == '}' :
			indentation -= 1
			if text[-1] == '\t' :
				text = text[:-1]
			text += '}\n' + ('\t' * indentation)
		# =
		elif source[i] == '=' :
			if text[-1] != ' ' :
				text += ' '
			text += '= '
		# ;
		elif source[i] == ';' :
			if not for_loop :
				text += ';\n' + ('\t' * indentation)
			else :
				text += '; '
		# (
		elif source[i] == '(' :
			text += source[i]
		# )
		elif source[i] == ')' :
			text += source[i] + ' '
		# ' "
		elif source[i] == '\'' or source[i] == '"' :
			if text[-1] == '\\' :
				text += source[i]
			else :
				if in_string_literal :
					text = text[:-1] + source[i] + ' '
					in_string_literal = False
				else :
					in_string_literal = True
					text += source[i]
		# <?php ?> php?>
		elif source[i] == '<?php' or source[i] == '?>' or source[i] == 'php?>' :
			text += source[i] + '\n' + ('\t' * indentation)
		# \
		elif source[i] == '\\' and source[i+1] in 'ntr' :
			if text[-1] == ' ' :
				text = text[:-1]
			text += '\\'
		else :
			text += source[i] + ' '
	return text
