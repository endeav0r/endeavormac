# Introduction #

The MARC is used at the United States Military Academy. Instead of continually converting instructions to assembly by hand, wouldn't it be great if we all just had a disassembler?

Source is available in svn. win32 binary available in downloads.

# Basic Usage #
mardism only accept instructions in binary. If you don't have a file full of binary instructions, you need to create one.

Create a text file called **instructions.txt** and place this inside:
```
0c92
4f00
4bfb
5328
```

[ascii\_hex\_to\_bin.py](http://code.google.com/p/endeavormac/source/browse/trunk/mardism/ascii_hex_to_bin.py) has been provided to quickly convert this file to binary instructions.

```
python ascii_hex_to_bin.py instructions.bin
```

Now give this file to mardism

```
mardism instructions.bin
```

This will give you:

```
0000:   0C92    SUBX  R1, R1, R1
0001:   4F00    SETHI R1, 0x0000
0002:   4BFB    ADDI  R1, -0x0005
0003:   5328    ADDI  R2, 0x0028
```

You can also specify an offset for the first instruction as such:

```
mardism instructions.bin 0080
```

Which will give you
```
0080:   0C92    SUBX  R1, R1, R1
0081:   4F00    SETHI R1, 0x0000
0082:   4BFB    ADDI  R1, -0x0005
0083:   5328    ADDI  R2, 0x0028
```

# Negative Numbers #
mardism attempts to detect negative numbers and display them as such.