#ifndef __MAP_H__
#define __MAP_H__

#include <Texture.h>
#include <Queue.h>

typedef enum Map_Textures{
	MAP_BGTEXTURE, MAP_ALLTEXTURES
}Map_Textures;


typedef struct Map_Object{
	SDL_Rect Collider;
}Map_Object;

typedef struct Map{
	SDL_Rect dimension;
	Texture map_textures[MAP_ALLTEXTURES];
	Queue* all_colliders;
}Map;

Map* Map_create(int x, int y, int w, int h);
bool Map_loadTexture(Map* map, SDL_Renderer* renderer, const char* filepath, Map_Textures texture_type);
void Map_render(Map* map, SDL_Renderer* renderer);
void Map_destroy(Map* map);



#endif