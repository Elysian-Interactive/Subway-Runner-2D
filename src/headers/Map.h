#ifndef __MAP_H__
#define __MAP_H__

#include <Texture.h>
#include <Queue.h>

// Macro for easily typing complex casts on queue object
#define MO(V) ((Map_Object*)V)

typedef enum Map_Texture{
	MAP_BGTEXTURE, MAP_TRAINTEXTURE, MAP_ALLTEXTURES
}Map_Texture;

typedef enum Map_ObjectType{
	MAP_VEHICLE, MAP_OBSTACLE, MAP_COLLECTIBLE, MAP_ALLOBJECTTYPES
}Map_ObjectType;

typedef struct Map_Object{
	SDL_Rect collider;
	Map_Texture texture_type;
	bool to_render;
}Map_Object;

typedef struct Map{
	SDL_Rect dimension;
	Texture map_textures[MAP_ALLTEXTURES];
	Queue* map_vehicles;
	Queue* map_obstacles;
	Queue* map_collectibles;
}Map;

extern int gMap_scrolling_offset;

Map* Map_create(int x, int y, int w, int h);
Map_Object* Map_createObject(int x, int y, int w, int h, Map_Texture type);
void Map_addObject(Map* map, Map_Object* object, Map_ObjectType type);
void Map_spawnObjects(Map* map); // All the logic for spawing different map objects will be written here
void Map_despawnObjects(Map* map); // All the logic for despawing and cleaning up map objects will be written here
void Map_destroyObject(Map_Object* object);
bool Map_loadTexture(Map* map, SDL_Renderer* renderer, const char* filepath, Map_Texture type);
void Map_render(Map* map, SDL_Renderer* renderer);
void Map_destroy(Map* map);



#endif