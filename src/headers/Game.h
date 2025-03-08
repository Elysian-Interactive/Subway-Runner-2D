#ifndef __GAME_H__
#define __GAME_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h> 
#include <stdbool.h>
#include <dbg.h>

// Usual Global Variables
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern TTF_Font* font; // globally used font
extern bool quit;

// Functions
bool init(); // Intializes subsystems and 
bool loadMedia(); // Loads the textures, music etc
void handleEvents(); // handles the user and game event
void update(); // Handles physics and other updates
void render(); // Display the sprites and textures on the screen
void close(); // frees the resources and closes the subsystems
int run();




#endif