#include <SDL2/SDL_mixer.h> 
#include <Game.h>
#include <Texture.h>
#include <Player.h>
#include <Map.h>

// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

TTF_Font* font = NULL;
bool quit = false;

Timer* timer = NULL;

// Other Variables
Player* cyborg;
Player* villian;
Map* map;

int run()
{
	bool r = init();
	check(r == true, "Something went wrong");
	
	r = loadMedia();
	check(r == true, "Something went wrong");
	
	// Starting the timer
	Timer_start(timer);
	
	while(!quit)
	{	
		handleEvents();
		update();
		render();	
	}
	
	close();
	
	return 0;
error:
	return 1;
}

// Function definitions
bool init()
{
	check(SDL_Init(SDL_INIT_EVERYTHING) >= 0, "Failed to initialize SDL! SDL_Error: %s", SDL_GetError());
	check((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) > 0, "Failed to initialize SDL_image! IMG_Error: %s", IMG_GetError());
	check(TTF_Init() != -1, "Failed to intialzie SDL_ttf! TTF_Error: %s", TTF_GetError()); 
	check(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) >= 0, "Failed to initialize SDL_mixer! Mix_Error: %s", Mix_GetError());
	
	window = SDL_CreateWindow("Subway Runner",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
	check(window != NULL, "Failed to create a window! SDL_Error: %s", SDL_GetError());
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	check(renderer != NULL, "Failed to create a renderer! SDL_Error: %s", SDL_GetError());
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	// Creating the timer
	timer = Timer_create();
	check(timer != NULL, "ERROR : Failed to create the timer!");
	
	// Creating the player
	cyborg = Cyborg_create(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	check(cyborg != NULL, "ERROR : Failed to initialize the Cyborg!");
	
	villian = Villian_create(SCREEN_WIDTH / 2 - (2 * cyborg->collider.w) , SCREEN_HEIGHT / 2 - cyborg->collider.h);
	check(villian != NULL, "ERROR : Failed to initialize the Villian!");
	
	// Creating the map
	map = Map_create(0,0,SCREEN_WIDTH, SCREEN_HEIGHT);
	check(map != NULL, "ERROR : Failed to initialize the Map!");
	
	
	return true;
error:
	return false;
}

bool loadMedia()
{
	// Loading player sprites
	bool r = Player_loadTexture(cyborg, renderer, "Assets/Textures/Cyborg/Angry.png", PLAYER_IDLE);
	check(r != false, "ERROR : Failed to load Player Texture")
	
	r = Player_loadTexture(cyborg, renderer, "Assets/Textures/Cyborg/Run.png", PLAYER_RUNNING);
	check(r != false, "ERROR : Failed to load Player Texture")
	
	r = Player_loadTexture(cyborg, renderer, "Assets/Textures/Cyborg/Double_jump.png", PLAYER_JUMPING);
	check(r != false, "ERROR : Failed to load Player Texture")
	
	// Loading Villian Texture
	r = Player_loadTexture(villian, renderer, "Assets/Textures/Villian/Special.png", PLAYER_IDLE);
	check(r != false, "ERROR : Failed to load Player Texture");
	
	// Loading map texture
	r = Map_loadTexture(map, renderer, "Assets/Textures/Map/Background/background.png", MAP_BGTEXTURE);
	check(r != false, "ERROR : Failed to load Map Texture");
	
	r = Map_loadTexture(map, renderer, "Assets/Textures/Map/Objects/Train.png", MAP_TRAINTEXTURE);
	check(r != false, "ERROR : Failed to load Map Texture");
	
	// TEMP : Checking Map Object functionality
	Map_Object* train = Map_createObject(SCREEN_WIDTH, SCREEN_HEIGHT / 2 - (Texture_getHeight(&(map->map_textures[MAP_TRAINTEXTURE]))), 446, 80, MAP_TRAINTEXTURE);
	check(train != NULL, "ERROR : Failed to create the map object!");
	Map_addObject(map, train);
	
	
	return true;
error:
	return false;
}

void handleEvents()
{
	SDL_Event e; // Queue to store the events
	
	while(SDL_PollEvent(&e) != 0){
		if(e.type == SDL_QUIT){
			quit = true;
		}
		
	}	
}

void update()
{
	Player_animate(cyborg, timer);
	Player_animate(villian, timer);
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clears the current frame
	
	// Map : to be drawn before the player
	Map_render(map, renderer);
	
	// Render the villian
	Villian_render(villian, renderer, villian->position.x, villian->position.y);
	// Render the cyborg
	cyborg->state = PLAYER_RUNNING;
	Cyborg_render(cyborg, renderer, cyborg->position.x, cyborg->position.y);
	
	SDL_RenderPresent(renderer); // Display the frame to the screen
}

void close()
{
	// Stopping the timer
	Timer_stop(timer);
	free(timer);
	timer = NULL;
	
	// Destroy the Cyborg
	free(cyborg);
	cyborg = NULL;
	
	// Destroy the Villian
	free(villian);
	villian = NULL;
	
	// Destroy the map
	Map_destroy(map);
	map = NULL;
	
	// Close our window and the renderer
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	
	// Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
