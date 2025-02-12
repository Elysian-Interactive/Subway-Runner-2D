#include <Player.h>

Player* Player_create(int x, int y)
{
	Player* temp = calloc(1, sizeof(Player));
	check(temp != NULL, "Failed to create Player!");
	
	temp->state = PLAYER_IDLE;
	
	temp->position.x = x;
	temp->position.y = y;
	
	temp->collider.x = x;
	temp->colider. y = y;
	
	
	
error:
	return NULL;
}