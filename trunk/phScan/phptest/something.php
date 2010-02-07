<?php

include 'functions.php';

function print_something ($something) {
	echo $something;
}

print 'in the middle of two functions\n';

function print_something_else ($something_else)
{
	echo $something_else;
}

function print_another_thing($another_thing) {
	echo $another_thing;
}

for ($i = 0; $i < 10; $i ++)
{
	echo 'hello';
}

print_something($_GET['something']);
print_something_else($something);
	
?>
