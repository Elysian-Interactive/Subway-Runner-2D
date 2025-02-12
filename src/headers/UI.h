#ifndef __UI_H__
#define __UI_H__

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Texture.h>
#include <dbg.h>

// An enumeration to define button action sprites
// This is basically genius since we can take a common texture
// and then depending on the state of the button we can render the
// specific sprite for that button

typedef enum ButtonSprite{
	BUTTON_SPRITE_MOUSE_OUT,
	BUTTON_SPRITE_MOUSE_OVER_MOTION,
	BUTTON_SPRITE_MOUSE_DOWN,
	BUTTON_SPRITE_MOUSE_UP,
	BUTTON_SPRITE_TOTAL
}ButtonSprite;

// a struct to represent a button
typedef struct Button{
	SDL_Point position; // defines the (x,y) position
	unsigned int width;
	unsigned int height;
	ButtonSprite current_sprite; // defines the currently used sprite by the button
}Button;

// Button Functions
Button* Button_create(unsigned int width, unsigned int height); // Creates an empty button
void Button_setPosition(Button* button, int x, int y); // Sets the x and y coordinate of the button
void Button_handleMouseEvents(Button* button, SDL_Event* e); // Handles the mouse event for the mouse
void Button_render(SDL_Renderer* renderer, Button* button, Texture* texture, SDL_Rect button_sprite_clips[]); // Renders the specified button, a clip must be provided
																							  // for the whole texture

#endif