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
	
	gMap_scrolling_offset--;
	if(gMap_scrolling_offset < -(Texture_getWidth(&(map->map_textures[MAP_BGTEXTURE])))){
		gMap_scrolling_offset = 0;
	}
	
	Texture_render(renderer, &(map->map_textures[MAP_BGTEXTURE]), gMap_scrolling_offset, 0, NULL);
	Texture_render(renderer, &(map->map_textures[MAP_BGTEXTURE]), gMap_scrolling_offset + Texture_getWidth(&(map->map_textures[MAP_BGTEXTURE])), 0, NULL); 
	
error: // fallthrough
	return;
}

void Map_destroy(Map* map)
{
	if(map){
		if(map->all_colliders){
			Queue_destroy(map->all_colliders);
		}
		free(map);
	}
}