#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <Texture.h>
#include <Timer.h>
#include <Queue.h>

typedef enum {
	LANE_WIDTH = 29,
	LANEPOS_1 = 92,
	LANEPOS_2 = 168,
	LANEPOS_3 = 244
}Player_lane;

typedef enum Player_state{
	PLAYER_IDLE, 
	PLAYER_RUNNING, 
	PLAYER_JUMPING, 
	PLAYER_ALLSTATES
}Player_state;

typedef struct Player{
	Texture* player_textures;
	Player_state state;
	SDL_Rect collider;
	SDL_Point position;
	SDL_Rect animation_clip;
	Player_lane lane;
}Player;

Player* Cyborg_create(int x, int y);
Player* Villian_create(int x, int y);
bool Player_loadTexture(Player* player, SDL_Renderer* renderer, const char* filepath, Player_state state);
bool Cyborg_loadAllMedia(Player* cyborg, SDL_Renderer* renderer);
void Player_render(Player* player, SDL_Renderer* renderer, int x, int y);
void Cyborg_render(Player* player, SDL_Renderer* renderer, int x, int y);
void Villian_render(Player* player, SDL_Renderer* renderer, int x, int y);
void Player_animate(Player* player, Timer* timer);
void Player_handleEvents(Player* player, SDL_Event* e);
void Player_move(Player* player, int y);
bool Player_checkCollision(Player* player, Queue* otherColliders);

#endif