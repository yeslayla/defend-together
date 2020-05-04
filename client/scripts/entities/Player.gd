extends KinematicBody2D

const BASE_SPEED : float = 50.0
export var user : String = ""
var main : bool = false
var input_locks = 0
const SPEED_VAL = 75

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
	
func update_position(pos : Vector2):
	if not main or abs(abs(pos.x) - abs(position.x)) > SPEED_VAL or abs(abs(pos.y) - abs(position.y)) > SPEED_VAL:
		position = pos

var velocity : Vector2 = Vector2(0,0)

func _process(delta):
	if main and input_locks <= 0:
		var relative_movement : Vector2 = Vector2(0,0)
		
		if(Input.is_action_pressed("ui_up")):
			relative_movement.y = relative_movement.y - 1
		if(Input.is_action_pressed("ui_down")):
			relative_movement.y = relative_movement.y + 1
		if(Input.is_action_pressed("ui_right")):
			relative_movement.x = relative_movement.x + 1
		if(Input.is_action_pressed("ui_left")):
			relative_movement.x = relative_movement.x - 1

		if relative_movement != Vector2.ZERO:
			move_and_collide(relative_movement * delta * BASE_SPEED)
			var dest = $"/root/ImportantEntities".tile_map.world_to_map(self.position)
			dest = dest + (position - (dest * 16))/16
			$"/root/NetworkManager".move_player(dest.x, dest.y)

func lock_input():
	input_locks = input_locks + 1
func unlock_input():
	input_locks = input_locks - 1
