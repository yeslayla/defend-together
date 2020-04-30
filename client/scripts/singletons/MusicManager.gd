extends AudioStreamPlayer

var main_player : AudioStreamPlayer
var looping : bool = false

func _ready():
	main_player = self

func play_music(song, loop=true):
	var audio_file = "res://assets/audio/music/" + song + ".ogg"
	if File.new().file_exists(audio_file):
		var track = load(audio_file)
		looping = loop
		main_player.stream = track
		#main_player.play()
