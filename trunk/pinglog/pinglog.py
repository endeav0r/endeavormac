import ping
import socket
import sqlite3
import sys
import time


if len(sys.argv) != 3 :
	print 'usage: ' + sys.argv[0] + ' <host> <log_filename>'
	sys.exit()

HOSTNAME = sys.argv[1]
FILENAME = sys.argv[2]

PINGS_PER_INTERVAL = 5
SECONDS_PER_INTERVAL = 60
TIMEOUT = 2

def do_ping (host, cursor) :

	pings = []
	timeouts = 0
	
	for i in range (5) :
		
		try :
			delay = ping.do_one(host, TIMEOUT)
			if delay == None :
				timeouts += 1
			else :
				pings.append(delay)
				
		except socket.gaierror, e :
			return "error " + str(e)
		

	delay = 0
	
	if len(pings) > 0 :

		for p in pings :
			delay += p
	
		delay /= len(pings)
		
	delay = delay * 1000
	delay = int(delay)
		
	cursor.execute("INSERT INTO pinglog (time, delay, sent, received) VALUES (?, ?, ?, ?)", (int(time.time()), int(delay), PINGS_PER_INTERVAL, PINGS_PER_INTERVAL - timeouts))
	
	return delay


conn = sqlite3.connect(FILENAME)
c = conn.cursor()

c.execute("CREATE TABLE IF NOT EXISTS pinglog (time INTEGER, delay INTEGER, sent INTEGER, received INTEGER)")

while True :
	try :
		loop_time = time.time()
		print do_ping (HOSTNAME, c)
		conn.commit()
		time.sleep(SECONDS_PER_INTERVAL - (time.time() - loop_time))
	except KeyboardInterrupt :
		print 'closed'
		break

conn.close()
