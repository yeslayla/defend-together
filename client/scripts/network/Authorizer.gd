extends Node

signal auth_connected
signal auth_disconnected

var client : StreamPeerTCP = null
var server_hostname : String = "127.0.0.1"
var server_port = 7778

func _ready():
	client = StreamPeerTCP.new()
	client.set_no_delay(true)
	set_process(false)
	
func auth_connect(host=server_hostname, port=server_port):
	
	# Connect if not connected
	if !client.is_connected_to_host():
		server_hostname = host
		server_port = port

		# Connect Socket & Create Stream
		client.connect_to_host(server_hostname, port)
		
		# Start listening
		set_process(true)
		
		# Validate intial connection
		if client.is_connected_to_host():
			client.put_string("Hey there daddy!")
			emit_signal("auth_connected")
			return true
		else:
			# Timeout implemented in `process` loop
			print("Waiting for host connection...")
			return false
	else:
		print("Client is already connected to server!")
		return false

func auth_disconnect():
	client.disconnect_from_host()
	set_process(false) # Disable listening loop
	print_debug("Disconnected from host.")
	emit_signal("auth_disconnected")

var count = 0
func _process(delta):

	if client.get_available_bytes() > 0:
		print(client.get_available_bytes())
		
		print(client.get_string(client.get_available_bytes()))


	# Await for client connection
	if client.get_status()==1:
		count= count+delta
	if count>1: # if it took more than 1s to connect, error
		print_debug("Failed connect, disconnecting...")
		auth_disconnect() #interrupts connection to nothing
