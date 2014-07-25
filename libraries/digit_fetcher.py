from StringIO import StringIO
from urllib2 import urlopen, URLError
import os
from PIL import Image

width = 28
height = 28

digit_counts = {}

for digit in range(10):
    file_name = 'data{}'.format(digit)
    digit_counts[digit] = 0
    digit_folder = '{}/'.format(digit)
    if not os.path.exists(digit_folder):
        os.mkdir(digit_folder)

    print 'Extracting digits from {}'.format(file_name)
    with open(file_name, 'rb') as digit_file:
        num_files = 1000
        interval = num_files / 10
        for i in range(num_files):
            if i % interval == 0:
                print '  extracting image file {} of {} ({:.2%})'.format(i, num_files, i * 1.0 / num_files)
            digit_buffer = digit_file.read(width * height)
            digit_image = Image.frombytes('L', (width, height), digit_buffer)
            digit_image.save('{}/{}.png'.format(digit, digit_counts[digit]))
            digit_counts[digit] += 1
