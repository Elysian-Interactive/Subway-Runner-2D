#ifndef __MAP_H__
#define __MAP_H__

#include <Texture.h>
#include <Queue.h>
#include <Player.h>
#include <Timer.h>

#define MAP_WIDTH 640
#define MAP_HEIGHT 360

#define LANEPOS_1 92
#define LANEPOS_2 168
#define LANEPOS_3 244
#define LANE_WIDTH 29


// Macro for easily typing complex casts on queue object
#define MO(V) ((Map_Object*)V)

typedef enum Map_Texture{
	MAP_BGTEXTURE, 
	MAP_TRAINTEXTURE, 
	MAP_COACHTEXTURE,
	MAP_MONEYTEXTURE,
	MAP_ALLTEXTURES
}Map_Texture;

typedef enum Map_ObjectType{
	MAP_VEHICLE, 
	MAP_OBSTACLE, 
	MAP_COLLECTIBLE, 
	MAP_ALLOBJECTTYPES
}Map_ObjectType;

typedef struct Map_Object{
	SDL_Rect collider;
	Map_Texture texture_type;
	bool to_render;
	bool collided; // for collectibles
}Map_Object;

typedef struct Map{
	SDL_Rect dimension;
	Texture* textures;
	Queue* vehicles;
	Queue* obstacles;
	Queue* collectibles;
}Map;

// Function pointer for differnt scenes
typedef void (*Map_sceneX) (Map* map);

extern Map_sceneX scenes[4]; // Holds the functions for different scens
extern int gMap_vehicles_speed_offset; // Determines how fast the vehicles should move
extern int gMap_stationary_objects_speed_offset; // Determines how fast the map and the stationary objects should move
extern int gMap_scrolling_offset;

Map* Map_create(int x, int y, int w, int h);
Map_Object* Map_createObject(int x, int y, int w, int h, Map_Texture type);
void Map_addObject(Map* map, Map_Object* object, Map_ObjectType type);
void Map_spawnObjects(Map* map, Timer* timer); // All the logic for spawing different map objects will be written here
void Map_despawnObjects(Map* map); // All the logic for despawing and cleaning up map objects will be written here
void Map_despawnFromQueue(Map* map, Queue* q, const char* queue_name, int speed_offset); // Despawn Helper function to clear objects from a queue
void Map_destroyObject(Map_Object* object);
bool Map_loadTexture(Map* map, SDL_Renderer* renderer, const char* filepath, Map_Texture type);
bool Map_loadAllMedia(Map* map, SDL_Renderer* renderer);
void Map_render(Map* map, Player* cyborg, Player* villian, SDL_Renderer* renderer);
void Map_renderQueue(Map* map,Queue* q, SDL_Renderer* renderer); // Map Rendering helper function for rendering individual queues
void Map_destroy(Map* map);

bool checkCollision(SDL_Rect* a, SDL_Rect* b); // function to check collision
void Map_checkQueueCollision(Queue* a, SDL_Rect* player_collider, bool dont_render_on_collision);
void Map_checkCollisions(Map* map, Player* player);

// Map Scenes
void Map_scene1(Map* map);
void Map_scene2(Map* map);
void Map_scene3(Map* map);
void Map_scene4(Map* map);

#endif