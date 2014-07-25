from StringIO import StringIO
from urllib2 import urlopen, URLError
import os
from PIL import Image

base_url = "http://skyservice.pha.jhu.edu/dr1/ImgCutout/getjpeg.aspx"
width = 128
height = 128
galaxy_data_file_name = "galaxy_data.csv"
num_lines = sum(1 for line in open(galaxy_data_file_name))
interval = 0
if num_lines < 100:
    interval = num_lines / 10
elif num_lines < 1000:
    interval = num_lines / 100
elif num_lines < 10000:
    interval = num_lines / 1000
else:
    interval = num_lines / 10000
interval = 1
pct = interval * 1.0 / num_lines
galaxy_data_file = open(galaxy_data_file_name)
header_line = galaxy_data_file.readline()
paths = [ 'spiral', 'elliptical' ]
[ os.mkdir(p) for p in paths if not os.path.exists(p) ]
registry_file = open('registry.txt', 'w')
spiral_count = 0
elliptical_count = 0
line_num = 0
for line in galaxy_data_file.readlines():
    line_num += 1
    if line_num % interval == 0:
        print "processing line {} of {} ({:.2%})".format(line_num, num_lines, line_num * 1.0 / num_lines)
    columns = line.strip().split(',')
    ra = columns[1]
    dec = columns[2]
    is_spiral = bool(columns[13])
    is_elliptical = bool(columns[14])
    url = base_url + '?'
    args = {
        'width' : width,
        'height' : height,
        'ra' : ra,
        'dec' : dec
    }
    url = base_url + '?' + '&'.join([ '%s=%s' % pair for pair in args.items() ])
    try:
        galaxy_image = Image.open(StringIO(urlopen(url).read()))
    except URLError, err:
        print "couldn't fetch image" + str(err)
        continue
    image_file_name = ''
    if is_spiral:
        image_file_name = "spiral/%00000d.jpg" % spiral_count
        spiral_count += 1
    if is_elliptical:
        image_file_name = "elliptical/%00000d.jpg" % elliptical_count
        elliptical_count += 1
    galaxy_image.save(image_file_name)
    print >> registry_file, "%s:  ra=%s, dec=%s" % (image_file_name, ra, dec)

    if line_num >= 1000:
        break
