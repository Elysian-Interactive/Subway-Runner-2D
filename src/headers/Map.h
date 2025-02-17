#ifndef __MAP_H__
#define __MAP_H__

#include <Texture.h>
#include <Queue.h>

// Macro for easily typing complex casts on queue object
#define MO(V) ((Map_Object*)V)

typedef enum Map_Textures{
	MAP_BGTEXTURE, MAP_TRAINTEXTURE, MAP_ALLTEXTURES
}Map_Textures;

typedef struct Map_Object{
	SDL_Rect collider;
	Map_Textures texture_type;
}Map_Object;

typedef struct Map{
	SDL_Rect dimension;
	Texture map_textures[MAP_ALLTEXTURES];
	Queue* map_objects;
}Map;

extern int gMap_scrolling_offset;

Map* Map_create(int x, int y, int w, int h);
Map_Object* Map_createObject(int x, int y, int w, int h, Map_Textures texture_type);
void Map_addObject(Map* map, Map_Object* object);
void Map_spawnObjects(Map* map);
bool Map_loadTexture(Map* map, SDL_Renderer* renderer, const char* filepath, Map_Textures texture_type);
void Map_render(Map* map, SDL_Renderer* renderer);
void Map_destroy(Map* map);



#endif