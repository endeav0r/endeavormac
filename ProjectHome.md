Any code that I develop and wish to release under an opensource license.

Unless otherwise stated, code is GPLv3. Some code (libraries) comes from other people (polarssl, sqlite, pinglog/ping.py off the top of my head), and that code is under the license found in the header files for those files.

Some of my other code is also over at http://code.google.com/p/rainbowsandpwnies/

**project\_name** _(code quality)_ description

code\_quality = (bad, meh, ok, good) // worst <==> best

## C ##
  * **bbs** _(bad)_ random, early C project. unimportant. non-functional
  * **e\_types** _(good)_ generic (void `*`) linked list and AA tree implementations I will use when I don't feel like rewriting boilerplater code. In-place merge sort on linked list, can save to disk, can cast between list and tree... I'll also modify this for specific needs.
  * **efscan** _(bad)_ early C project. Adds filesystem info to a sqlite db. Not secure, don't use.
  * **end** _(ok)_ small scripting language developed for fun.
  * **end2** _(good)_ immediate successor to end. a better, small scripting language.
  * **md4** _(ok)_ Implementation of the MD4 algorithm from the RFC for understanding. Only works on small messages.
  * **md5** _(bad)_ Implementation of the MD5 algorithm from RFC. Ugly.
  * **poker** _(ok)_ My poker hand evaluator.

## Python ##
  * **phScan** _(bad)_ Program to do some basic analysis of php scripts.
  * **pinglog** _(meh)_ Pings a server on a regular basis, saving ping times to a sqlite db. You can then create colored graphs from the latency to track average latency times over a day.

## C++ ##
  * **tanks2** _(meh)_ A 2d tanks game where tank AI is programmed in LUA.