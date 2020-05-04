extends KinematicBody2D

export var user : String = ""
var main : bool = false
var input_locks = 0

func set_main():
	main = true
	$Label.hide()
	$Camera.show()
	$Camera.current = true
	
	$"/root/ImportantEntities".main_player = self

func _ready():
	$Camera.hide()
	set_username("")

func set_username(username):
	user = username
	$Label.text = user

func _process(delta):
	if main and input_locks <= 0:
		var movePos : Vector2 = Vector2(0,0)
		
		
		if(Input.is_action_just_pressed("ui_up")):
			movePos.y = movePos.y - 1
		if(Input.is_action_just_pressed("ui_down")):
			movePos.y = movePos.y + 1
		if(Input.is_action_just_pressed("ui_right")):
			movePos.x = movePos.x + 1
		if(Input.is_action_just_pressed("ui_left")):
			movePos.x = movePos.x - 1
	
		if movePos != Vector2.ZERO:
			$"/root/NetworkManager".move_player(movePos.x, movePos.y)

func lock_input():
	input_locks = input_locks + 1
func unlock_input():
	input_locks = input_locks - 1
