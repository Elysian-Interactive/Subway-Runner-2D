#include <UI.h>

// Function defintions
Button* Button_create(unsigned int width, unsigned int height)
{
	Button* new_button = (Button*) malloc(sizeof(Button));
	check_mem(new_button);
	
	// Initialize the newly created button
	new_button->position.x = 0;
	new_button->position.y = 0;
	new_button->width = width;
	new_button->height = height;
	new_button->current_sprite = BUTTON_SPRITE_MOUSE_OUT;
	
	return new_button;
error:
	return NULL;
}

void Button_setPosition(Button* button, int x, int y)
{
	check(button != NULL,"Invalid Button");
	button->position.x = x;
	button->position.y = y;

error: // fallthrough
	return; 
}

void Button_handleMouseEvents(Button* button, SDL_Event* e)
{
	check(button != NULL, "Invalid Button");
	
	// First we will check if the mouse event happened
	if(e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP){
		// These events corresponds to 1. moving the mouse 2. clicking down a mouse button 3. releasing a mouse button
		
		// Fetch the mouse position
		int x, y;
		SDL_GetMouseState(&x, &y); // We use this function to obtatin the mouse position
		
		// now we will check if the mouse is inside the button or not
		bool inside = true;
		
		if(x < button->position.x) inside = false; // mouse is to the left of the button
		else if(x > button->position.x + button->width) inside = false; // mouse is to the right of the button
		else if(y < button->position.y) inside = false; // mouse is above the button
		else if(y > button->position.y + button->height) inside = false; // mouse is below the button
		
		// if mouse is outside the button
		if(!inside){
			button->current_sprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		else{ // if mouse inside the button
			switch(e->type){
				case SDL_MOUSEMOTION:
					button->current_sprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
					break;
				
				case SDL_MOUSEBUTTONDOWN:
					button->current_sprite = BUTTON_SPRITE_MOUSE_DOWN;
					break;
				
				case SDL_MOUSEBUTTONUP:
					button->current_sprite = BUTTON_SPRITE_MOUSE_UP;
					break;
			}
		}
		
	}

error: // fallthrough
	return;
}

void Button_render(SDL_Renderer* renderer, Button* button, Texture* texture, SDL_Rect button_sprite_clips[])
{
	check(button != NULL, "Invalid Button");
	check(texture != NULL, "Invalid Texture");
	
	// show the current button sprite
	Texture_render(renderer, texture, button->position.x, button->position.y, &button_sprite_clips[button->current_sprite]); 
	
error: // fallthrough
	return;
}

