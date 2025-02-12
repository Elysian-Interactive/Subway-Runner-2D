#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <Texture.h>
#include <Timer.h>
#include <Queue.h>

typedef enum Player_state{
	PLAYER_IDLE, PLAYER_RUNNING, PLAYER_JUMPING, PLAYER_ALLSTATES
};

typedef struct Player{
	Texture* texture[PLAYER_STATES];
	Player_state state;
	SDL_Rect collider;
	SDL_Point position;
}Player;

Player* Player_create(int x, int y);
void Player_loadTextures(Player* player, const char* filepath, Player_state state);
void Player_render(Player* player);
void Player_animate
void Player_handleEvents(Player* player);
void Player_move(Player* player);
bool Player_checkCollision(Player* player, Queue* otherColliders);

#endif