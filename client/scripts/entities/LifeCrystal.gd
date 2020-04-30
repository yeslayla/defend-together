extends Area2D

const ROTATION_SPEED = 1

func _process(delta):
	$Sprite.rotation = $Sprite.rotation + delta * ROTATION_SPEED
