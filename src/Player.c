#include <Player.h>

Player* Player_create(int x, int y)
{
	Player* temp = calloc(1, sizeof(Player));
	check(temp != NULL, "Failed to create Player!");
	
	temp->state = PLAYER_IDLE;
	
	temp->position.x = x;
	temp->position.y = y;
	
	temp->collider.x = x;
	temp->collider.y = y;
	temp->collider.w = 48;
	temp->collider.h = 48;
	
	temp->animation_clip.x = 0;
	temp->animation_clip.y = 0;
	temp->animation_clip.w = 48;
	temp->animation_clip.h = 48;
	
	
	return temp;
error:
	return NULL;
}

bool Player_loadTexture(Player* player, SDL_Renderer* renderer, const char* filepath, Player_state state)
{
	check(player != NULL, "ERROR : Invalid Player!");
	
	bool r = Texture_loadFromFile(renderer, &player->player_textures[state], filepath);
	check(r != false, "ERROR : Failed to load the texture : %s", filepath);
	
	return true;
error:
	return false;
}

void Player_render(Player* player, SDL_Renderer* renderer, int x, int y)
{
	check(player != NULL, "ERROR : Invalid Player!");
	Texture_render(renderer, &(player->player_textures[player->state]), x, y, &player->animation_clip);
	
error: // fallthrough
	return;
}

void Player_animate(Player* player, Timer* timer)
{
	check(player != NULL, "ERROR : Invalid Player!");
	
	int frames = (int)(Texture_getWidth(&(player->player_textures[player->state])) / 48);
	
	player->animation_clip.x = 48 * (int)((Timer_getTicks(timer) / 120) % frames);

error: // fallthrough
	return;
}