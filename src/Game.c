#include <Game.h>
#include <Texture.h>
#include <Player.h>
#include <Map.h>

// Usual Global Variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

TTF_Font* font = NULL;
bool quit = false;

Timer* timer = NULL;

// FPS Capping logic
const int SCREEN_FPS = 60; 
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
Timer* fps_timer = NULL; 
Timer* cap_timer = NULL;
int counted_frames = 0;

// Other Variables
Player* cyborg;
Player_lane prev_lane;
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
	Timer_start(fps_timer);
	
	while(!quit)
	{	
		Timer_start(cap_timer);
		
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
	
	window = SDL_CreateWindow("Subway Runner",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
	check(window != NULL, "Failed to create a window! SDL_Error: %s", SDL_GetError());
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	check(renderer != NULL, "Failed to create a renderer! SDL_Error: %s", SDL_GetError());
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	// Creating the timer
	timer = Timer_create();
	check(timer != NULL, "ERROR : Failed to create the timer!");
	
	// Creating our FPS timers
	fps_timer = Timer_create();
	check(fps_timer != NULL, "ERROR : Failed to create the FPS timer!");
	
	cap_timer = Timer_create();
	check(cap_timer != NULL, "ERROR : Failed to create the CAP timer!");
	
	// Creating the player
	cyborg = Cyborg_create(100, (int)(LANEPOS_2 + LANE_WIDTH / 2));
	check(cyborg != NULL, "ERROR : Failed to initialize the Cyborg!");
	
	villian = Villian_create(SCREEN_WIDTH / 6 - (2 * cyborg->collider.w) , SCREEN_HEIGHT / 2 - cyborg->collider.h);
	// villian = Villian_create(100, 100);
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
	// Loading player media
	bool r = Cyborg_loadAllMedia(cyborg, renderer);
	
	// Loading Villian Texture
	r = Player_loadTexture(villian, renderer, "Assets/Textures/Villian/Special.png", PLAYER_IDLE);
	check(r != false, "ERROR : Failed to load Player Texture");
	
	// Loading map media
	r = Map_loadAllMedia(map, renderer);

	
	
	// Map_spawnObjects(map);

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
		
		prev_lane = cyborg->lane;
		Player_handleEvents(cyborg, &e);
		
	}	
}

void update()
{
	Map_spawnObjects(map, timer);
	
	// Update Player Position
	if(prev_lane != cyborg->lane){ // Only move when a change occurs
		Player_move(cyborg, (int)(cyborg->lane + LANE_WIDTH / 2));
	}
	
	Player_animate(cyborg, timer);
	Player_animate(villian, timer);
	
	Map_checkCollisions(map, cyborg);
	
	Map_despawnObjects(map);
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // Clears the current frame
	
	// Map : to be drawn before the player
	Map_render(map, cyborg, villian, renderer);
	
	SDL_RenderPresent(renderer); // Display the frame to the screen

	// if frame finished early we want to cap it
	int frame_ticks = Timer_getTicks(cap_timer);
	if(frame_ticks < SCREEN_TICKS_PER_FRAME){
		// Wait the remaining time
		SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_ticks);
	}
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
