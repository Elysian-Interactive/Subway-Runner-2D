#ifndef __MAP_H__
#define __MAP_H__

#include <Texture.h>
#include <Queue.h>

typedef enum Map_Textures{
	MAP_BGTEXTURE, MAP_ALLTEXTURES
};

typedef struct Map{
	SDL_Rect dimensions;
	Texture* alltextures[MAP_ALLTEXTURES]
	Queue* allcolliders;
};

Map* Map_create(int x, int y, int w, int h);
void Map_loadTextures(Map* map, const char* filename, Map_Textures texture_type);
void Map_render();


#endif