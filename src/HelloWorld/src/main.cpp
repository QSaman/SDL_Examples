// https://wiki.libsdl.org/SDL_CreateWindow#code_examples
// https://gigi.nullneuron.net/gigilabs/showing-an-empty-window-in-sdl2/
#include <SDL2/SDL.h>
#include <iostream>

constexpr int WIDTH = 800, HEIGHT = 600;

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << "SDL2 initialization failed: " << SDL_GetError() <<  std::endl;
		return 1;
	}

	auto window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	if (window == nullptr)
	{
		std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
		return 2;
	}
	auto renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr)
	{
		std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
		return 3;
	}
	SDL_Event event;
	bool quit = false;
	while (!quit)
	{
		// Without this, you cannot see a window in Wayland. For more information:
		// https://github.com/libsdl-org/SDL/issues/6074
		SDL_RenderPresent(renderer);
		if (SDL_WaitEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					quit = true;
					break;
			}
		}
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
