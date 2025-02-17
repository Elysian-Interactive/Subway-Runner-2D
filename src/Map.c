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
	
	// Create the queue to hold all of our map objects
	temp->map_objects = Queue_create();
	check(temp->map_objects != NULL, "ERROR : Failed to create the Object Queue!");
	
	return temp;
error:
	return NULL;
}

bool Map_loadTexture(Map* map, SDL_Renderer* renderer, const char* filepath, Map_Textures texture_type)
{
	check(map != NULL, "ERROR : Invalid Map!");
	
	bool r = Texture_loadFromFile(renderer, &(map->map_textures[texture_type]), filepath);
	check(r != false, "ERROR : Failed to load the texture : %s", filepath);
	
	return true;
error:
	return false;
}

void Map_render(Map* map, SDL_Renderer* renderer)
{
	check(map != NULL, "ERROR : Invalid Map");
	check(map->map_objects != NULL, "ERROR : Invalid Object Queue!");
	
	gMap_scrolling_offset--;
	if(gMap_scrolling_offset < -(Texture_getWidth(&(map->map_textures[MAP_BGTEXTURE])))){
		gMap_scrolling_offset = 0;
	}
	// Rendering the background
	Texture_render(renderer, &(map->map_textures[MAP_BGTEXTURE]), gMap_scrolling_offset, 0, NULL);
	Texture_render(renderer, &(map->map_textures[MAP_BGTEXTURE]), gMap_scrolling_offset + Texture_getWidth(&(map->map_textures[MAP_BGTEXTURE])), 0, NULL); 
	
	// Texture_render(renderer, &(map->map_textures[MAP_TRAINTEXTURE]), 0, 0, NULL); 
	
	
	// Rendering all the map objects
	QUEUE_FOREACH(map->map_objects, cur){
		Texture_render(renderer, &(map->map_textures[MO(cur->value)->texture_type]), MO(cur->value)->collider.x , MO(cur->value)->collider.y, NULL);
		// In order to move them I decrease the x coordinate by a given number
		MO(cur->value)->collider.x -= 2;
	}
	
error: // fallthrough
	return;
}

void Map_destroy(Map* map)
{
	if(map){
		if(map->map_objects){
			Queue_destroy(map->map_objects);
		}
		free(map);
	}
}

Map_Object* Map_createObject(int x, int y, int w, int h, Map_Textures texture_type)
{
	Map_Object* temp = calloc(1, sizeof(Map_Object));
	check(temp != NULL, "ERROR : Failed to create the Map Object!");
	
	temp->collider.x = x;
	temp->collider.y = y;
	temp->collider.w = w;
	temp->collider.h = h;
	
	temp->texture_type = texture_type;
	
	return temp;
error:
	return NULL;
}

void Map_addObject(Map* map, Map_Object* object)
{
	check(map != NULL, "ERROR : Invalid Map!");
	check(object != NULL, "ERROR : Invalid Object!");
	// Simply add it in the queue
	Queue_push(map->map_objects, object);
	
	
error:
	return;
}