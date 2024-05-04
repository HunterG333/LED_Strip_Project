import serial
import http.server
import socketserver
import json
import time

# Initialize serial
ser = serial.Serial('COM5', 1200)

# Define the minimum time interval between command executions (in seconds)
MIN_COMMAND_INTERVAL = 1  # Adjust this value as needed

last_command_time = 0

class CommandHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        # Handle GET requests
        if self.path == '/':
            # Redirect users to the index page of LEDHomepage
            self.send_response(301)  # 301 Moved Permanently
            self.send_header('Location', '/LEDHomepage/index.html')
            self.end_headers()
        else:
            # Serve files normally for other URLs
            super().do_GET()
        
    def do_POST(self):
        global last_command_time
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        data = json.loads(post_data.decode('utf-8'))
        command = data.get('command', '')
        print("Received command:", command)
        
        # Throttle command execution
        current_time = time.time()
        if current_time - last_command_time >= MIN_COMMAND_INTERVAL:
            # Execute the command received from the server
            ser.flush()
            ser.write(command.encode("utf-8"))
            
            last_command_time = current_time
            # Send response back to the client
            self.send_response(200)
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write("Command executed successfully".encode('utf-8'))
        else:
            # Send response indicating throttling
            self.send_response(429)  # 429 Too Many Requests
            self.send_header('Content-type', 'text/plain')
            self.end_headers()
            self.wfile.write("Command throttled. Too many requests.".encode('utf-8'))

def startServer():
    # Set up the HTTP server
    try:
        with socketserver.TCPServer(("192.168.4.46", 8000), CommandHandler) as httpd:
            print("Python server started successfully.")
            httpd.serve_forever()
    except Exception as e:
        print("Error starting server:", e)

# Call the function to start the server
startServer()
