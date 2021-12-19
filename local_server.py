from http.server import BaseHTTPRequestHandler, HTTPServer
import socket
import xml.etree.ElementTree as ET
import copy

data_to_parse = 'data.xml'

ship_values = {'second_value': int(), 'left_motor': int(), 'right_motor': int(), 'led': bool()}
list_of_ship_values = list()

xml_tree = ET.parse(data_to_parse)

root = xml_tree.getroot()

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

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.connect(("8.8.8.8", 80))
hostName = str(s.getsockname()[0])
serverPort = 8080

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path.endswith('/left_motor'):
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.end_headers()
            for el in list_of_ship_values:
                self.wfile.write(bytes("{};".format(el['left_motor']), "utf-8"))
        if self.path.endswith('/right_motor'):
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.end_headers()
            for el in list_of_ship_values:
                self.wfile.write(bytes("{};".format(el['right_motor']), "utf-8"))
        if self.path.endswith('/led'):
            self.send_response(200)
            self.send_header("Content-type", "text/html")
            self.end_headers()
            for el in list_of_ship_values:
                self.wfile.write(bytes("{};".format(el['led']), "utf-8"))

if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")
