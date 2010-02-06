import Image, ImageDraw, ImageFont
import time as time_module
import sqlite3
import sys

WIDTH    = 720
HEIGHT   = 240
TIMEZONE = -5
MAXDELAY = 500

OUTDIR = 'graphs/'


if len(sys.argv) < 3 :
	print 'usage: ' + sys.argv[0] + ' <pinglog_db> <days_to_graph>'
	print '<days_to_graph> = 0 for all days, 1 for current day, 2 for today and yesterday, etc...'
	sys.exit()
else :
	DB_FILENAME = sys.argv[1]
	DAYS_TO_GRAPH = int(sys.argv[2])
	


def get_day_second_pixel (second) :
	
	global WIDTH, TIMEZONE
	
	second_width = float(WIDTH) / float(60 * 60 * 24)
	pixel = second_width * (float(second) % (60.0 * 60.0 * 24.0))
	return int(pixel)



def get_delay_height (delay) :

	global HEIGHT, MAXDELAY
	
	ms_height = float(HEIGHT) / float(MAXDELAY)
	return int(round(ms_height * float(delay)))
	


def get_delay_color (delay) :

	if delay > MAXDELAY :
		return (0xff, 0, 0)
	elif delay > MAXDELAY / 2 :
		green = float(MAXDELAY) - float(delay)
		green /= float(MAXDELAY / 2)
		green *= 256.0
		green = int(green)
		return (0xff, green, 0)
	else :
		red = float(delay) / float(MAXDELAY / 2)
		red *= 256.0
		red = int(red)
		return (red, 0xff, 0)
		
		
		
def average_list (l) :
	if len(l) == 0 :
		return None
	summation = 0
	for i in l :
		summation += i
	return summation / len(l)	
		
		
# data is a list, where each element is [time, delay]
# start_delay will start the day at n delay, if there is no initial data
def chart_day (data, filename, start_delay=0) :

	global WIDTH, HEIGHT, TIMEZONE, MAXDELAY
	
	print 'graphing ' + filename

	im = Image.new("RGB", (WIDTH, HEIGHT))
	draw = ImageDraw.Draw(im)
	draw.rectangle(((0,0), (WIDTH - 1, HEIGHT - 1)), fill=(255,255,255))



	last_hour = 0

	# line_width of one second
	line_width = float(WIDTH) / float(60 * 60 * 24)
	
	# set up each line as an empty list
	lines = []
	for i in range(WIDTH) :
		lines.append([])
		
	# add the data to the lines
	for i in range(len(data)) :
		time, delay = data[i]
		lines[get_day_second_pixel(time)].append(delay)
		
	
	
	last_delay = start_delay
	current_hour = 0
	
	# for each line
	for i in range(len(lines)) :
		
		# average the delay
		delay = average_list(lines[i])
		if delay == None :
			for line in lines[i:] :
				if len(line) > 0 :
					delay = last_delay
			if delay == None :
				delay = 0
			else :
				delay = last_delay
		else :
			last_delay = delay
			
			
		# draw the line
		draw.line(((i, HEIGHT), (i, HEIGHT - get_delay_height(delay))), fill=get_delay_color(delay))
		# draw a black dot at the top of the line to help show the height of the graph
		#draw.point((i, HEIGHT), fill=(0,0,0))
		#draw.rectangle(((i, HEIGHT - 1), (right, HEIGHT - delay - 1)), fill=(red, green, 0))
			

	for i in range (24) :
		draw.line(((get_day_second_pixel(i * 60 * 60), 0), (get_day_second_pixel(i * 60 * 60), HEIGHT - 1)), fill=(0,0,0))
		draw.text((get_day_second_pixel(i * 60 * 60) + 2, HEIGHT - 12), str(i), fill=(0,0,0))


	for i in range(MAXDELAY / 100) :
		if i == 0 :
			continue
		y = HEIGHT - get_delay_height((i) * 100)
		draw.line(((0, y), (WIDTH - 1, y)), fill=(0,0,0))
		draw.text((2, y - 12), str(i * 100) + 'ms', fill=(0,0,0))

	im.save(filename)
	
	
conn = sqlite3.connect(DB_FILENAME)
c = conn.cursor()

c.execute("SELECT time FROM pinglog ORDER BY time DESC LIMIT 0, 1")
last_db_time = c.fetchone()[0]

today_start_seconds = int(time_module.time()) - int(time_module.time()) % (60 * 60 * 24) - (TIMEZONE * 60 * 60)

for day_to_graph in range(DAYS_TO_GRAPH) :
	start_time = today_start_seconds - (60 * 60 * 24) * day_to_graph
	end_time = today_start_seconds + (60 * 60 * 24) * (day_to_graph + 1)
	c.execute("SELECT time, delay FROM pinglog WHERE time >= ? AND time <= ?", (start_time, end_time))
	data = []
	for row in c :
		data.append((int(row[0]) + (TIMEZONE * 60 * 60), int(row[1])))
	filename = time_module.strftime('%d%b%y', time_module.gmtime(today_start_seconds))
	filename.lower()
	filename += '.png'
	chart_day(data, OUTDIR + filename)
