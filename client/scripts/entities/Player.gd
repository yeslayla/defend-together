extends KinematicBody2D

export var user : String = ""

func _ready():
	set_username("")

func set_username(username):
	print(username)
	user = username
	$Label.text = user

func _process(delta):
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
