import mysql.connector
import xml.etree.ElementTree as ET
import copy

mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  password="qwe123qwe",
  database="ship"
)

mycursor = mydb.cursor()

data_to_parse = 'data.xml'

ship_values = {'second_value': int(), 'left_motor': int(), 'right_motor': int(), 'led': bool()}
list_of_ship_values = list()

xml_tree = ET.parse(data_to_parse)

root = xml_tree.getroot()

session_time = root.find('session_time').text

for elem in root.findall('second'):
    second_value = elem.attrib['value']
    left_motor = elem.find('left_motor').text
    right_motor = elem.find('right_motor').text
    LED = elem.find('LED').text
    ship_values['second_value'] = second_value
    ship_values['left_motor'] = left_motor
    ship_values['right_motor'] = right_motor
    ship_values['led'] = LED
    list_of_ship_values.append(copy.deepcopy(ship_values))

create_table = '''
create table if not exists ship_state (
	session_id datetime,
	second_value int, 
    left_motor_value int,
    right_motor_value int,
    led_value int
);'''

mycursor.execute(create_table)

for el in list_of_ship_values:
    insert = "INSERT INTO ship_state (session_id, second_value, left_motor_value, right_motor_value, led_value) VALUES (%s, %s, %s, %s, %s)"
    val = (session_time, el['second_value'], el['left_motor'], el['right_motor'], el['led'])
    mycursor.execute(insert, val)
    
mydb.commit()

print(len(list_of_ship_values), "records inserted.")

print(mydb)
