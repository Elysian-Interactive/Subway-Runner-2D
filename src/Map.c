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
	
	temp->textures = calloc(MAP_ALLTEXTURES, sizeof(Texture));
	check(temp->textures != NULL, "ERROR : Failed to allocate memory to textures");
	
	// Creating three queues to hold different types of objects
	// i.e. 1. Vehicles 2. Obstacles 3. Collectibles
	
	temp->vehicles = Queue_create();
	check(temp->vehicles != NULL, "ERROR : Failed to create the Vehicles Queue!");
	
	temp->obstacles = Queue_create();
	check(temp->obstacles != NULL, "ERROR : Failed to create the Obstacles Queue!");
	
	temp->collectibles = Queue_create();
	check(temp->collectibles != NULL, "ERROR : Failed to create the Collectibles Queue!");
	
	return temp;
error:
	return NULL;
}

bool Map_loadTexture(Map* map, SDL_Renderer* renderer, const char* filepath, Map_Texture type)
{
	check(map != NULL, "ERROR : Invalid Map!");
	
	bool r = Texture_loadFromFile(renderer, &(map->textures[type]), filepath);
	check(r != false, "ERROR : Failed to load the texture : %s", filepath);
	
	return true;
error:
	return false;
}

bool Map_loadAllMedia(Map* map, SDL_Renderer* renderer)
{
	bool r = Map_loadTexture(map, renderer, "Assets/Textures/Map/Background/background2.png", MAP_BGTEXTURE);
	check(r != false, "ERROR : Failed to load Map Texture");
	
	r = Map_loadTexture(map, renderer, "Assets/Textures/Map/Vehicles/Train.png", MAP_TRAINTEXTURE);
	check(r != false, "ERROR : Failed to load Map Texture");
	
	r = Map_loadTexture(map, renderer, "Assets/Textures/Map/Obstacles/Coach2.png", MAP_COACHTEXTURE);
	check(r != false, "ERROR : Failed to load Map Texture");
	
	r = Map_loadTexture(map, renderer, "Assets/Textures/Map/Collectibles/Money.png", MAP_MONEYTEXTURE);
	check(r != false, "ERROR : Failed to load Map Texture");

error:
	return r;
}

void Map_render(Map* map, Player* cyborg, Player* villian, SDL_Renderer* renderer)
{
	check(map != NULL, "ERROR : Invalid Map");
	check(map->vehicles != NULL, "ERROR : Invalid Vehicles Queue!");
	check(map->obstacles != NULL, "ERROR : Invalid Obstacles Queue!");
	check(map->collectibles != NULL, "ERROR : Invalid Collectibles Queue!");
	check(cyborg != NULL, "ERROR : Invalid Cyborg Player!");
	check(villian != NULL, "ERROR : Invalid Villian Player!");
	
	gMap_scrolling_offset -= gMap_stationary_objects_speed_offset;
	if(gMap_scrolling_offset < -(Texture_getWidth(&(map->textures[MAP_BGTEXTURE])))){
		gMap_scrolling_offset = 0;
	}
	// Rendering the background
	Texture_render(renderer, &(map->textures[MAP_BGTEXTURE]), gMap_scrolling_offset, 0, NULL);
	Texture_render(renderer, &(map->textures[MAP_BGTEXTURE]), gMap_scrolling_offset + Texture_getWidth(&(map->textures[MAP_BGTEXTURE])), 0, NULL); 

	// Rendering through the various object queues
	Map_renderQueue(map,map->collectibles, renderer);
	
	// Render the villian
	// Villian_render(villian, renderer, villian->position.x, villian->position.y);
	// Render the cyborg
	cyborg->state = PLAYER_RUNNING;
	Cyborg_render(cyborg, renderer, cyborg->position.x, cyborg->position.y);
	
	Map_renderQueue(map,map->obstacles, renderer);
	Map_renderQueue(map,map->vehicles, renderer);

error:
	return;
}

void Map_renderQueue(Map* map, Queue* q, SDL_Renderer* renderer)
{	
	// Rendering all the Map Vehicles
	QUEUE_FOREACH(q, cur){
		if(MO(cur->value)->to_render){
			Texture_render(renderer, &(map->textures[MO(cur->value)->texture_type]), MO(cur->value)->collider.x , MO(cur->value)->collider.y, NULL);
		}
	}
}

void Map_destroy(Map* map)
{
	if(map){
		if(map->vehicles){
			Queue_destroy(map->vehicles);
		}
		if(map->obstacles){
			Queue_destroy(map->obstacles);
		}
		if(map->collectibles){
			Queue_destroy(map->collectibles);
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
			Queue_push(map->vehicles, object);
			break;
		case MAP_OBSTACLE:
			Queue_push(map->obstacles, object);
			break;
		case MAP_COLLECTIBLE:
			Queue_push(map->collectibles, object);
			break;
	}
		
error:
	return;
}

void Map_spawnObjects(Map* map)
{
	
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
	Map_despawnFromQueue(map, map->vehicles,"Vehicles", gMap_vehicles_speed_offset); 
	// Since obstacles and collectible will be stationary the offset is same as the background offset
	Map_despawnFromQueue(map, map->obstacles,"Obstacles", gMap_stationary_objects_speed_offset); 
	Map_despawnFromQueue(map, map->collectibles,"Collectibles", gMap_stationary_objects_speed_offset);

error:
	return;

}

void Map_despawnFromQueue(Map* map, Queue* q, const char* queue_name, int speed_offset)
{
	check(q != NULL, "ERROR : Invalid %s Queue", queue_name);
	
	int no_despawn = 0;
	
	// Switching off rendering on objects out of the screen
	QUEUE_FOREACH(q, cur){
		if(MO(cur->value)->collider.x < -((int)Texture_getWidth(&(map->textures[MO(cur->value)->texture_type])))){
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