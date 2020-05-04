extends Node


func _ready():
	$"/root/ImportantEntities".tile_map = $Tiles
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
					var pos : Vector2 = $Tiles.map_to_world(Vector2(float(tile_data[0]), float(tile_data[1]))) 
					pos = Vector2(pos.x + get_decimals(float(tile_data[0])), pos.y + get_decimals(float(tile_data[1])))
					
					#pos = Vector2(pos.x + decs.x, pos.y + decs.y)
					var entity_data = tile_data[2].split(':')
					update_entity(entity_data[1], pos, entity_data[0])

				else:
					$Tiles.set_cell(int(tile_data[0]), int(tile_data[1]), int(tile_data[2]))
	if get_node_or_null("Loading") != null:
		$Loading.queue_free()
		
func get_decimals(number : float):
	var temp : float = int(number)
	return (number - temp) * 16
		
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
			if type == "player":
				if entity_id == $"/root/NetworkManager".username:
					entity.set_main()
		else:
			display_error("Trying to load entity of type: " + type + ", but failed.")
	if entity:
		if entity.has_method("update_position"):
			entity.update_position(pos)
		else:
			entity.position = pos
		if entity.has_method("set_username"):
			entity.set_username(entity_id)
		
func delete_entity(entity_id : String, type : String):
	var entity : Node2D = get_node_or_null( str(type + "-" + entity_id))
	if entity:
		entity.queue_free()
	
