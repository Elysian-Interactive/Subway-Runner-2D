#include <Player.h>

Player* Cyborg_create(int x, int y)
{
	Player* temp = malloc(sizeof(Player));
	check(temp != NULL, "Failed to create Player!");
	
	temp->player_textures = calloc(PLAYER_ALLSTATES, sizeof(Texture));
	check(temp->player_textures != NULL, "ERROR : Failed to allocate memory to player textures!");
	
	temp->state = PLAYER_IDLE;
	
	temp->collider.w = 20;
	temp->collider.h = 32;

	temp->animation_clip.x = 0;
	temp->animation_clip.y = 0;
	temp->animation_clip.w = 48;
	temp->animation_clip.h = 48;
	
	temp->position.x = x;
	temp->position.y = y - temp->animation_clip.h;
	
	temp->collider.x = x + 4;
	temp->collider.y = temp->position.y + 20;
	
	temp->lane = LANEPOS_2;
	
	temp->score = 0;
	
	return temp;
error:
	return NULL;
}

Player* Villian_create(int x, int y)
{
	Player* temp = malloc(sizeof(Player));
	check(temp != NULL, "Failed to create Player!");
	
	temp->player_textures = calloc(PLAYER_ALLSTATES, sizeof(Texture));
	check(temp->player_textures != NULL, "ERROR : Failed to allocate memory to player textures!");
	
	temp->state = PLAYER_IDLE;
	
	temp->position.x = x;
	temp->position.y = y;
	
	temp->collider.x = x;
	temp->collider.y = y;
	temp->collider.w = 96;
	temp->collider.h = 96;
	
	temp->animation_clip.x = 0;
	temp->animation_clip.y = 0;
	temp->animation_clip.w = 96;
	temp->animation_clip.h = 96;
	
	
	return temp;
error:
	return NULL;
}

bool Player_loadTexture(Player* player, SDL_Renderer* renderer, const char* filepath, Player_state state)
{
	check(player != NULL, "ERROR : Invalid Player!");
	
	bool r = Texture_loadFromFile(renderer, &(player->player_textures[state]), filepath);
	check(r != false, "ERROR : Failed to load the texture : %s", filepath);
	
	return true;
error:
	return false;
}

bool Cyborg_loadAllMedia(Player* cyborg, SDL_Renderer* renderer)
{
	bool r = Player_loadTexture(cyborg, renderer, "Assets/Textures/Cyborg/Angry.png", PLAYER_IDLE);
	check(r != false, "ERROR : Failed to load Player Texture");
	
	r = Player_loadTexture(cyborg, renderer, "Assets/Textures/Cyborg/Run.png", PLAYER_RUNNING);
	check(r != false, "ERROR : Failed to load Player Texture");
	
	r = Player_loadTexture(cyborg, renderer, "Assets/Textures/Cyborg/Double_jump.png", PLAYER_JUMPING);
	check(r != false, "ERROR : Failed to load Player Texture");

error: // fallthrough
	return r;
}

void Cyborg_render(Player* player, SDL_Renderer* renderer, int x, int y)
{
	check(player != NULL, "ERROR : Invalid Player!");
	if(player->state == PLAYER_IDLE){
		Texture_renderEx(renderer, &(player->player_textures[player->state]), x, y, &player->animation_clip, 0, NULL, SDL_FLIP_HORIZONTAL);
	}
	else{
		Texture_render(renderer, &(player->player_textures[player->state]), x, y, &player->animation_clip);
	}
	
error: // fallthrough
	return;
}

void Villian_render(Player* player, SDL_Renderer* renderer, int x, int y)
{
	check(player != NULL, "ERROR : Invalid Player!");
	
	Texture_render(renderer, &(player->player_textures[player->state]), x, y, &player->animation_clip);
	
error: // fallthrough
	return;
}

void Player_animate(Player* player, Timer* timer)
{
	check(player != NULL, "ERROR : Invalid Player!");
	
	int frames = (int)(Texture_getWidth(&(player->player_textures[player->state])) / player->animation_clip.w);
	
	player->animation_clip.x = player->animation_clip.w * (int)((Timer_getTicks(timer) / 120) % frames);

error: // fallthrough
	return;
}

void Player_handleEvents(Player* player, SDL_Event* e)
{
	// If a key is pressed
	if(e->type == SDL_KEYDOWN && e->key.repeat == 0){
		switch(e->key.keysym.sym){
			case SDLK_UP: 
				switch(player->lane){
					case LANEPOS_1:
						break;
					case LANEPOS_2:
						player->lane = LANEPOS_1;
						break;
					case LANEPOS_3:
						player->lane = LANEPOS_2;
						break;
				}
				break;
				
			case SDLK_DOWN: 
				switch(player->lane){
					case LANEPOS_1:
						player->lane = LANEPOS_2;
						break;
					case LANEPOS_2:
						player->lane = LANEPOS_3;
						break;
					case LANEPOS_3:
						break;
				}
				break;
		}
	}
	
	// if a key is released
	if(e->type == SDL_KEYUP && e->key.repeat == 0){
		switch(e->key.keysym.sym){
			case SDLK_UP : break;
			case SDLK_DOWN : break;	
		}
	}
}

void Player_move(Player* player, int y)
{
	check(player != NULL, "ERROR : Invalid Player!");
	
	player->position.y = y - player->animation_clip.h;
	player->collider.y = player->position.y + 18;
	
error:
	return;
}
