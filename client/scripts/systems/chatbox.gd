extends Control

const MESSAGE_TIMEOUT : int = 17

var locking = false

func _ready():
	$"/root/NetworkManager".connect("chat_message_recieved", self, "_on_new_message")

func _on_new_message(message):
	print(message)
	var message_block : Label = Label.new()
	message_block.text = message
	var timer = Timer.new()
	timer.connect("timeout", message_block, "queue_free")
	message_block.add_child(timer)
	$Messages.add_child(message_block)
	timer.start(MESSAGE_TIMEOUT)

func _process(delta):

	if $LineEdit.has_focus() != locking:
		locking = $LineEdit.has_focus()
		if locking:
			$"/root/ImportantEntities".main_player.lock_input()
		else:
			$"/root/ImportantEntities".main_player.unlock_input()
		
		
	if(Input.is_action_just_pressed("send_chat_message")):
		if $LineEdit.has_focus():
			if len($LineEdit.text) > 0:
				$"/root/NetworkManager".send_chat_message($LineEdit.text)
				$LineEdit.text = ""
			$LineEdit.release_focus()
		else:
			$LineEdit.grab_focus()
