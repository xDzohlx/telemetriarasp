import os
import sys
import BaseHTTPServer
import serial
import socket

ip = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
ip.connect(("192.168.100.42",80))
direccion = ip.getsockname()[0]
ip.close()
print(direccion)
arduino = serial.Serial('/dwev/ttyACM0',9600)

class RequestHandler (BaseHTTPServer.BaseHTTPRequestHandler):
    def do_GET (self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(self._get_status())
        return
    def _get_status (self):
        return "<head><meta http-equiv='refresh' content = '5'</head>Datos gps \n" + line + 
def main (args):
    httpd = BaseHTTPServer.HTTPServer((direccion, 8000), RequestHandler)
    httpd.serve_forever()

if __name__ == "__main__":
    sys.exit(main(sys.argv))
