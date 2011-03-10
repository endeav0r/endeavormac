import base64
import sys

if len(sys.argv) != 3 :
    print 'usage: python ' + sys.argv[0] + ' <input_filename> <output_filename>'
    sys.exit(-1)

fh = open(sys.argv[1], 'r')
input_raw = fh.read()
fh.close()


ascii_hex = ''

# go through and parse out all non-ascii HEX chars
for c in input_raw :
    c = c.upper()
    if c in '0123456789ABCDEF' :
        ascii_hex += c
    

print ascii_hex
binary = base64.b16decode(ascii_hex)
print base64.b16encode(binary)
print len(binary)

fh = open(sys.argv[2], 'wb')
fh.write(binary)
fh.close()