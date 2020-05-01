extends Node


func _ready():
	$Tiles.clear()
	$"/root/NetworkManager".connect("world_data_recieved", self, "_on_world_update")
	$"/root/NetworkManager".request_world_map()
	
func _on_world_update():
	var data = $"/root/NetworkManager".world_data.split('\n')
	for tileUpdate in data:
		if len(tileUpdate) > 3:
			if "delete," in tileUpdate:
				var delete_data = tileUpdate.substr(len("delete,")).split(':')
				delete_entity(delete_data[1],delete_data[0])
			elif ',' in tileUpdate:
				var tile_data = tileUpdate.split(',')
				if ':' in tile_data[2]:
					var pos : Vector2 = $Tiles.map_to_world(Vector2(int(tile_data[0]), int(tile_data[1])))
					if 'player:' in tile_data[2]:
						print(tile_data)
						var player_name = tile_data[2].substr(len('player:'))
						print(player_name)
						if $"/root/NetworkManager".username == player_name:
							$Player.position = pos
						else:
							update_entity(player_name, pos, "player")
					else:
						var entity_data = tile_data[2].split(':')
						update_entity(entity_data[1], pos, entity_data[0])

				else:
					$Tiles.set_cell(int(tile_data[0]), int(tile_data[1]), int(tile_data[2]))
	if $Loading != null:
		$Loading.queue_free()
		
func display_error(error):
	print("Error " + error)
		
func update_entity(entity_id : String, pos : Vector2, type : String):
	var entity : Node2D = get_node_or_null( str(type + "-" + entity_id))
	if not entity:
		var entity_location = "res://nodes/entities/" + type + ".tscn"
		if File.new().file_exists(entity_location):
			var gobj = load(entity_location)
			entity = gobj.instance()
			add_child(entity, true)
			entity.set_name(str(type + "-" + entity_id))
		else:
			display_error("Trying to load entity of type: " + type + ", but failed.")
	if entity:
		entity.position = pos
		
func delete_entity(entity_id : String, type : String):
	var entity : Node2D = get_node_or_null( str(type + "-" + entity_id))
	if entity:
		entity.queue_free()
	
