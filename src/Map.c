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
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Collectibles rects are blue
	Map_renderQueue(map,map->collectibles, renderer);
	
	// Render the villian
	// Villian_render(villian, renderer, villian->position.x, villian->position.y);
	// Render the cyborg
	cyborg->state = PLAYER_RUNNING;
	Cyborg_render(cyborg, renderer, cyborg->position.x, cyborg->position.y);
	
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);// Player Rect is green 
	SDL_RenderDrawRect(renderer, &(cyborg->collider)); 
	
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Obstacles and vehicles Rects are red
	Map_renderQueue(map,map->obstacles, renderer);
	Map_renderQueue(map,map->vehicles, renderer);

error:
	return;
}

void Map_renderQueue(Map* map, Queue* q, SDL_Renderer* renderer)
{	
	// Rendering all the Map Objects in their respective queues
	if(q == map->collectibles)
	{
		QUEUE_FOREACH(q, cur){
			if(MO(cur->value)->to_render && !(MO(cur->value)->collided)){
			Texture_render(renderer, &(map->textures[MO(cur->value)->texture_type]), MO(cur->value)->collider.x , MO(cur->value)->collider.y, NULL);
			SDL_RenderDrawRect(renderer, &(MO(cur->value)->collider));
			}
		}
	}else{
		QUEUE_FOREACH(q, cur){
			if(MO(cur->value)->to_render){
			Texture_render(renderer, &(map->textures[MO(cur->value)->texture_type]), MO(cur->value)->collider.x , MO(cur->value)->collider.y, NULL);
			SDL_RenderDrawRect(renderer, &(MO(cur->value)->collider));
			}
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
	
	if(x > MAP_WIDTH){
		temp->to_render = false;
	} else {
		temp->to_render = true;
	}
	
	temp->collided = false;
	
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
	check(map != NULL, "ERROR : Invalid Map!");
	
	Map_Object* coach = NULL;
	Map_Object* money = NULL;
	int i = 0;
	
	// Setting up Beginning Scene
	for(i = 1;i < 5;i++){
		if(i % 2 != 0){			
			coach = Map_createObject((MAP_WIDTH * (i)) / 2, LANEPOS_1 - 33, 206, 57, MAP_COACHTEXTURE);
			check(coach != NULL, "ERROR : Failed to create the map object!");
			Map_addObject(map, coach, MAP_OBSTACLE);
			
			coach = Map_createObject((MAP_WIDTH * (i)) / 2, LANEPOS_3 - 33, 206, 57, MAP_COACHTEXTURE);
			check(coach != NULL, "ERROR : Failed to create the map object!");
			Map_addObject(map, coach, MAP_OBSTACLE);	
		}
		else{
			coach = Map_createObject(MAP_WIDTH * (i / 2), LANEPOS_2 - 33, 206, 57, MAP_COACHTEXTURE);
			check(coach != NULL, "ERROR : Failed to create the map object!");
			Map_addObject(map, coach, MAP_OBSTACLE);
		}
		
	}
	
	for(int i = 0;i < 5;i++){
		money = Map_createObject((MAP_WIDTH / 2) + 40 * (i), LANEPOS_2 - 8, 24, 24, MAP_MONEYTEXTURE);
		check(money != NULL, "ERROR : Failed to create the map object!");
		Map_addObject(map, money, MAP_COLLECTIBLE);
	}
	
	for(int i = 0;i < 5;i++){
		money = Map_createObject(MAP_WIDTH + 40 * (i), LANEPOS_3 - 8, 24, 24, MAP_MONEYTEXTURE);
		check(money != NULL, "ERROR : Failed to create the map object!");
		Map_addObject(map, money, MAP_COLLECTIBLE);
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
	Map_despawnFromQueue(map, map->vehicles,"Vehicles", gMap_vehicles_speed_offset); 
	// Since obstacles and collectible will be stationary the offset is same as the background offset
	Map_despawnFromQueue(map, map->obstacles,"Obstacles", gMap_stationary_objects_speed_offset); 
	Map_despawnFromQueue(map, map->collectibles,"Collectibles", gMap_stationary_objects_speed_offset);

error:
	return;

}

void Map_despawnFromQueue(Map* map, Queue* q, const char* queue_name, int speed_offset)
{
	
	int no_despawn = 0;
	
	// Switching off rendering on objects out of the screen
	QUEUE_FOREACH(q, cur){
		// if(MO(cur->value)->collider.x < -((int)Texture_getWidth(&(map->textures[MO(cur->value)->texture_type])))){
		
		Map_Object* o = MO(cur->value);
		
		if(!(o->collided) && !(o->to_render) && o->collider.x < MAP_WIDTH && o->collider.x > -(o->collider.w)){
			o->to_render = true;
		} else if(o->collider.x < -(o->collider.w)){
			o->to_render = false;
			no_despawn++;
		}
		
		// We decrement the postion of the objects by the given offset
		// Values have to decremented regardless whether they are being renderer or not
		o->collider.x -= speed_offset;
		
	}
	
	// Cleaning up the objects to be despawned
	for(int i = 0;i < no_despawn;i++){
		Map_destroyObject(MO(Queue_pop(q)));
	}
	
error:
	return;
}

bool checkCollision(SDL_Rect* a, SDL_Rect* b) // only check collision with objects that are being rendererd
{
	// The sides of the rectangle
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;
	
	// calculate the sides of the rect A
	leftA = a->x;
	rightA = a->x + a->w;
	topA = a->y;
	bottomA = a->y + a->h;
	
	// calculate the sides of the rect B
	leftB = b->x;
	rightB  = b->x + b->w;
	topB = b->y;
	bottomB = b->y + b->h;
	
	// Now we do our separating axis test and if any of the axis from 
	// either of the boxes are separate then there is not collision else
	// they will collide

	if(bottomA <= topB) return false;
	if(topA >= bottomB) return false;
	if(rightA <= leftB) return false;
	if(leftA >= rightB) return false;
	
	// if none of the sides of A are outside B then they are colliding
	return true;
}

void Map_checkQueueCollision(Queue* q, SDL_Rect* player_collider, bool dont_render_on_collision)
{
	
	QUEUE_FOREACH(q, cur){
		Map_Object* mo = MO(cur->value);
		
		if(mo->to_render){
			if(checkCollision(&(mo->collider), player_collider)){
				mo->collided = true;
				if(dont_render_on_collision){ // In case its a collecitbles
					mo->to_render = false; 
				} 
			} 
		}
	}
}

void Map_checkCollisions(Map* map, Player* player)
{
	Map_checkQueueCollision(map->collectibles, &(player->collider), true);
	Map_checkQueueCollision(map->obstacles, &(player->collider), false);
}