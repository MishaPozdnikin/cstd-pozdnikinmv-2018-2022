import serial
import time
import sys
import xml.etree.ElementTree as ET
import xml.dom.minidom
import datetime

file = 'data.xml'

COMPORT = sys.argv[1]
time_to_read = int(sys.argv[2])

arduino = serial.Serial(port="COM" + COMPORT, baudrate=9600, timeout=.1)
arduino.readline()
arduino.readline()

root = ET.Element('time_to_values')
tree = ET.ElementTree(root)

session_time = ET.Element('session_time')
now = datetime.datetime.now()
session_time.text = str(now)[ : str(now).find('.')]
root.append(session_time)

i = 0
while i < time_to_read:
    i = i + 1
    line = arduino.readline()
    line = str(line).replace('b\'', '').replace('\\n', '').replace('\\r', '').replace('\'', '')
    if not line:
        i = i - 1
        continue
    line = line.split(' ')
    print(line[0] + " " + line[1] + " " + line[2])
    second = ET.Element('second')
    second.set('value', str(i))

    left_motor = ET.Element('left_motor')
    left_motor.text = line[0]
    right_motor = ET.Element('right_motor')
    right_motor.text = line[1]
    led = ET.Element('LED')
    led.text = line[2]
    
    second.append(left_motor)
    second.append(right_motor)
    second.append(led)

    root.append(second)
    time.sleep(1)

prettyxml = xml.dom.minidom.parseString(ET.tostring(root))
pretty_xml_as_string = prettyxml.toprettyxml()

with open(file, "w") as f:
    f.write(pretty_xml_as_string)
    f.close()
