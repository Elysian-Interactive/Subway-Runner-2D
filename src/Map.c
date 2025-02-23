#include <Map.h>

int gMap_scrolling_offset = 0;
int gMap_vehicles_speed_offset = 2; 
int gMap_stationary_objects_speed_offset = 1;

Map* Map_create(int x, int y, int w, int h)
{
	Map* temp = malloc(sizeof(Map));
	check(temp != NULL, "ERROR : Failed to creat the Map!");
	
	temp->dimension.x = x;
	temp->dimension.y = y;
	temp->dimension.w = w;
	temp->dimension.h = h;
	
	// Creating three queues to hold different types of objects
	// i.e. 1. Vehicles 2. Obstacles 3. Collectibles
	
	temp->map_vehicles = Queue_create();
	check(temp->map_vehicles != NULL, "ERROR : Failed to create the Vehicles Queue!");
	
	temp->map_obstacles = Queue_create();
	check(temp->map_obstacles != NULL, "ERROR : Failed to create the Obstacles Queue!");
	
	temp->map_collectibles = Queue_create();
	check(temp->map_collectibles != NULL, "ERROR : Failed to create the Collectibles Queue!");
	
	return temp;
error:
	return NULL;
}

bool Map_loadTexture(Map* map, SDL_Renderer* renderer, const char* filepath, Map_Texture type)
{
	check(map != NULL, "ERROR : Invalid Map!");
	
	bool r = Texture_loadFromFile(renderer, &(map->map_textures[type]), filepath);
	check(r != false, "ERROR : Failed to load the texture : %s", filepath);
	
	return true;
error:
	return false;
}

void Map_render(Map* map, SDL_Renderer* renderer)
{
	check(map != NULL, "ERROR : Invalid Map");
	check(map->map_vehicles != NULL, "ERROR : Invalid Vehicles Queue!");
	check(map->map_obstacles != NULL, "ERROR : Invalid Obstacles Queue!");
	check(map->map_collectibles != NULL, "ERROR : Invalid Collectibles Queue!");
	
	gMap_scrolling_offset -= gMap_stationary_objects_speed_offset;
	if(gMap_scrolling_offset < -(Texture_getWidth(&(map->map_textures[MAP_BGTEXTURE])))){
		gMap_scrolling_offset = 0;
	}
	// Rendering the background
	Texture_render(renderer, &(map->map_textures[MAP_BGTEXTURE]), gMap_scrolling_offset, 0, NULL);
	Texture_render(renderer, &(map->map_textures[MAP_BGTEXTURE]), gMap_scrolling_offset + Texture_getWidth(&(map->map_textures[MAP_BGTEXTURE])), 0, NULL); 
	
	
{ 	// Rendering all the Map Vehicles
	QUEUE_FOREACH(map->map_vehicles, cur){
		if(MO(cur->value)->to_render){
			Texture_render(renderer, &(map->map_textures[MO(cur->value)->texture_type]), MO(cur->value)->collider.x , MO(cur->value)->collider.y, NULL);
			// In order to move them I decrease the x coordinate by a given number
		}
	}
}

{	// Rendering all the Map Obstacles
	QUEUE_FOREACH(map->map_obstacles, cur){
		Texture_render(renderer, &(map->map_textures[MO(cur->value)->texture_type]), MO(cur->value)->collider.x , MO(cur->value)->collider.y, NULL);
		
	}
}

{	// Rendering all the Map Collectibles
	QUEUE_FOREACH(map->map_collectibles, cur){
		Texture_render(renderer, &(map->map_textures[MO(cur->value)->texture_type]), MO(cur->value)->collider.x , MO(cur->value)->collider.y, NULL);
	}
}

	
error: // fallthrough
	return;
}

void Map_destroy(Map* map)
{
	if(map){
		if(map->map_vehicles){
			Queue_destroy(map->map_vehicles);
		}
		if(map->map_obstacles){
			Queue_destroy(map->map_obstacles);
		}
		if(map->map_collectibles){
			Queue_destroy(map->map_collectibles);
		}
		
		free(map);
	}
}

Map_Object* Map_createObject(int x, int y, int w, int h, Map_Texture type)
{
	Map_Object* temp = calloc(1, sizeof(Map_Object));
	check(temp != NULL, "ERROR : Failed to create the Map Object!");
	
	temp->collider.x = x;
	temp->collider.y = y;
	temp->collider.w = w;
	temp->collider.h = h;
	
	temp->texture_type = type;
	
	temp->to_render = true;
	
	return temp;
error:
	return NULL;
}

void Map_addObject(Map* map, Map_Object* object, Map_ObjectType type)
{
	check(map != NULL, "ERROR : Invalid Map!");
	check(object != NULL, "ERROR : Invalid Object!");
	
	// Simply add it in the suitable queue
	switch(type){
		case MAP_VEHICLE:
			Queue_push(map->map_vehicles, object);
			break;
		case MAP_OBSTACLE:
			Queue_push(map->map_obstacles, object);
			break;
		case MAP_COLLECTIBLE:
			Queue_push(map->map_collectibles, object);
			break;
	}
		
error:
	return;
}

// Simple Function to Deallocate memory from the Map Object datatype
void Map_destroyObject(Map_Object* object)
{
	if(object){
		free(object);
	}
}

void Map_despawnObjects(Map* map)
{
	check(map != NULL, "ERROR : Invalid Map!");
	
	// Despawing from all the queues and moving them to the left by the speed offset
	
	// Since vehicles have to go fast the offset is bigger
	Map_despawnFromQueue(map, map->map_vehicles,"Vehicles", gMap_vehicles_speed_offset); 
	// Since obstacles and collectible will be stationary the offset is same as the background offset
	Map_despawnFromQueue(map, map->map_obstacles,"Obstacles", gMap_stationary_objects_speed_offset); 
	Map_despawnFromQueue(map, map->map_collectibles,"Collectibles", gMap_stationary_objects_speed_offset);

error:
	return;

}

void Map_despawnFromQueue(Map* map, Queue* q, const char* queue_name, int speed_offset)
{
	check(q != NULL, "ERROR : Invalid %s Queue", queue_name);
	
	int no_despawn = 0;
	
	// Switching off rendering on objects out of the screen
	QUEUE_FOREACH(q, cur){
		if(MO(cur->value)->collider.x < -((int)Texture_getWidth(&(map->map_textures[MO(cur->value)->texture_type])))){
			MO(cur->value)->to_render = false;
			no_despawn++;
		}
		else{
			// We decrement the postion of the objects by the given offset
			// Values have to decremented regardless whether they are being renderer or not
			MO(cur->value)->collider.x -= speed_offset;
		}
	}
	// Cleaning up the objects to be despawned
	for(int i = 0;i < no_despawn;i++){
		Map_destroyObject(MO(Queue_pop(q)));
		// log_info("[INFO] : Items in Vehicle Queue : %d",Queue_count(map->map_vehicles));
	}
	
error:
	return;
}