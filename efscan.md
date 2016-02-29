# Introduction #

This tool scan a given path, adding all files to sqlite3 database. Scans can be compared against one another, showing differences in the file system over time. File hashes can be compared to those provided by [NSRL](http://www.nsrl.nist.gov/Downloads.htm), in order to gain some understanding of what files are valid on a freshly scanned system.


---


# Build Instructions #
## Windows ##
Windows build environment is Windows 7, 64 bit edition.
### Requires ###
  * MinGW
### Instructions ###
Run sqlite3.bat, and then debug.bat


## Ubuntu ##
Ubuntu build environment is Ubuntu 9.10, 64 bit edition.
### Requires ###
  * gcc
  * libsqlite3-dev
  * Both can be obtained through `apt-get install build-essential libsqlite3-dev`
### Instructions ###
Run debug.sh