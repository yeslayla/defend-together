extends Node


func _ready():
	$"/root/MusicManager".play_music("wizards")
	$Button.connect("button_down", self, "_on_button_press")
	$"/root/NetworkManager".connect("error_occured", self, "_on_error")
	$"/root/NetworkManager".connect("logged_in", self, "_on_login")

func _on_error():
	$ErrorDialog/ErrorLabel.text = $"/root/NetworkManager".error_info
	$ErrorDialog.popup_centered()

func _on_button_press():
	if($"/root/NetworkManager".connected):
		$"/root/NetworkManager".disconnect_from_server()
	else:
		$"/root/NetworkManager".connect_as_user($LineEdit.text)

func _on_login():
	get_tree().change_scene("res://scenes/game.scn")
