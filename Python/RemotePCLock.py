from http.server import BaseHTTPRequestHandler, HTTPServer
import time
import socket
import ctypes

#ctypes.windll.user32.LockWorkStation()

#hostName = "localhost"
#serverPort = 8080
hostName = socket.gethostbyname(socket.gethostname())
serverPort = 1234

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        self.wfile.write(bytes("<html><head><title>Lock</title></head>", "utf-8"))
        #self.wfile.write(bytes("<p>Request: %s</p>" % self.path, "utf-8"))
        self.wfile.write(bytes("Lock unsafe PCs remotely.<br><br>", "utf-8"))
        if self.path == "/lock":
            print('lock triggered')
            ctypes.windll.user32.LockWorkStation()        
  
        self.wfile.write(bytes("<body>", "utf-8"))
        self.wfile.write(bytes("<p><a href='/lock'>Lock PC</a></p>", "utf-8"))
        self.wfile.write(bytes("</body></html>", "utf-8"))
    #def do_POST(self):
        #test
           

if __name__ == "__main__":
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))
    #print("PC IP is: ", socket.gethostbyname(socket.gethostname()))

    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")
