#include <Map.h>

int gMap_scrolling_offset = 0;

Map* Map_create(int x, int y, int w, int h)
{
	Map* temp = calloc(1,sizeof(Map));
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
	
	gMap_scrolling_offset--;
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
		// Values have to decremented regardless whether they are being renderer or not
		MO(cur->value)->collider.x -= 2;
	}
}

{	// Rendering all the Map Obstacles
	QUEUE_FOREACH(map->map_obstacles, cur){
		Texture_render(renderer, &(map->map_textures[MO(cur->value)->texture_type]), MO(cur->value)->collider.x , MO(cur->value)->collider.y, NULL);
		// In order to move them I decrease the x coordinate by a given number
		MO(cur->value)->collider.x -= 1;
	}
}

{	// Rendering all the Map Collectibles
	QUEUE_FOREACH(map->map_collectibles, cur){
		Texture_render(renderer, &(map->map_textures[MO(cur->value)->texture_type]), MO(cur->value)->collider.x , MO(cur->value)->collider.y, NULL);
		// In order to move them I decrease the x coordinate by a given number
		MO(cur->value)->collider.x -= 1;
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

void Map_addObject(Map* map, Map_Object* object, Map_ObjectType type )
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
	check(map->map_vehicles != NULL, "ERROR : Invalid Vehicles Queue!");
	check(map->map_obstacles != NULL, "ERROR : Invalid Obstacles Queue!");
	check(map->map_collectibles != NULL, "ERROR : Invalid Collectibles Queue!");
	
	// Variable to hold the no. of pops to be done
	int no_despawn = 0;
	
{ 	// Rendering all the Map Vehicles
	QUEUE_FOREACH(map->map_vehicles, cur){
		if(MO(cur->value)->collider.x < 10 /*-(Texture_getWidth(&(map->map_textures[MO(cur->value)->texture_type])))*/){
			MO(cur->value)->to_render = false;
			no_despawn++;
		}
	}
	
	/*
	for(int i = 0;i < no_despawn;i++){
		Map_destroyObject(MO(Queue_pop(map->map_vehicles)));
	}
	*/
}

{	// Rendering all the Map Obstacles
	QUEUE_FOREACH(map->map_obstacles, cur){
	
	}
}

{	// Rendering all the Map Collectibles
	QUEUE_FOREACH(map->map_vehicles, cur){
		
	}
}

error:
	return;

}