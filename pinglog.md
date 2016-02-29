# Introduction #

First, pinglog would not be possible without ping.py. I did not write this code. ping.py is released under the GPL2 license by Matthew Dixon Cowles

Two scripts are included.
  * **pinglog.py** Pings a server several times, averags the delay, stores it in a database, and then sleeps for a set amount of time.
  * **graph.py** Reads from the database created by pinglog.py and creates nice, colorful graphs that even your neighbor's cousin's aunt can understand.


# Requirements #

  * **Python 2.6** Not tested in python 3.x, and probably won't work out of the box
  * **Python Imaging Library** Package is _python-imaging_ in Ubuntu.