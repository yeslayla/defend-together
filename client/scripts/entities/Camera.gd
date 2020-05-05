extends Camera2D

#-------------#
# Camera Zoom #
#-------------#
export var zoomDecelration = 4
export var zoomCapSpeed = 0.125

export var minZoom = 0.25
export var maxZoom = 0.5
var zoomChange = 0

func _ready():
	zoom.x = (minZoom + maxZoom) / 2
	zoom.y = (minZoom + maxZoom) / 2

func _process(delta):
	CameraZoom(delta)

func _input(event):
	if event is InputEventMouseButton:
		if event.is_pressed():
			if event.button_index == BUTTON_WHEEL_UP:
				zoomChange = -zoomCapSpeed
			if event.button_index == BUTTON_WHEEL_DOWN:
				zoomChange = zoomCapSpeed

func CameraZoom(delta):
	if(zoomChange > 0):
		zoomChange = clamp(zoomChange - zoomDecelration * delta, 0, zoomCapSpeed)
	elif(zoomChange < 0):
		zoomChange = clamp(zoomChange + zoomDecelration * delta, -zoomCapSpeed, 0)
		
	zoom.x = clamp(zoom.x + zoomChange, minZoom, maxZoom)
	zoom.y = clamp(zoom.y + zoomChange, minZoom, maxZoom)
