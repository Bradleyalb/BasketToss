import websocket

# Connect to WebSocket server
ws = websocket.WebSocket()
ws.connect("ws://10.0.0.153")
print("Connected to WebSocket server")

# Ask the user for some input and transmit it

# Wait for server to respond and print it
while True:
	result = ws.recv()
	print("Received: " + result)

# Gracefully close WebSocket connection
ws.close()