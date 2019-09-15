#include <SDL.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* argv[])
{
	// Initialize all SDL subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cerr << "SDL could not be initialized. SDL_Error: " << SDL_GetError() << '\n';
		return 1;
	}

	// Create a window, we don't care about position
	SDL_Window* window = SDL_CreateWindow(
		"CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		std::cerr << "Window could not be created. SDL_Error: " << SDL_GetError() << '\n';
		return 1;
	}

	// Create renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Create texture that stores frame buffer
	SDL_Texture* sdlTexture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		64, 32);

	uint32_t pixels[2048];

	// get surface of window, fill it with white pixels, then update and wait two seconds
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(window);
	SDL_Delay(2000);

	// frees memory of window and surface
	SDL_DestroyWindow(window);

	// quit SDL subsystems
	SDL_Quit();

	return 0;
}