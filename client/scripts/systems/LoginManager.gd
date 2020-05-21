extends Node

var auth

func _ready():
	$"/root/MusicManager".play_music("wizards")
	$Button.connect("button_down", self, "_on_button_press")
	$"/root/NetworkManager".connect("error_occured", self, "_on_error")
	$"/root/NetworkManager".connect("logged_in", self, "_on_login")
	auth = $"/root/Authorizer"
	auth.connect("auth_connected", self, "_on_auth_connection")
	auth.connect("auth_disconnected", self, "_on_auth_disconnection")
	

func _on_error():
	$ErrorDialog/ErrorLabel.text = $"/root/NetworkManager".error_info
	$ErrorDialog.popup_centered()

func _on_button_press():
	auth.auth_connect()
	
func _on_auth_connection():
	$Button.disabled = true
	
func _on_auth_disconnection():
	$Button.disabled = false

func _on_button_press_OLD():
	if($"/root/NetworkManager".connected):
		$"/root/NetworkManager".disconnect_from_server()
	else:
		$"/root/NetworkManager".connect_as_user($LineEdit.text)

func _on_login():
	get_tree().change_scene("res://scenes/game.scn")
