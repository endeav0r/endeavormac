import phpfile
import php

something = phpfile.PhpFile('phptest/something.php')

print something.source

for function in something.functions :
	print function
	print something.functions[function].source
