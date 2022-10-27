// https://github.com/libsdl-org/SDL_image/blob/main/showimage.c
// https://gigi.nullneuron.net/gigilabs/displaying-an-image-in-an-sdl2-window/
// https://gigi.nullneuron.net/gigilabs/loading-images-in-sdl2-with-sdl_image/
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <iostream>

constexpr int WIDTH = 800, HEIGHT = 600;

SDL_Rect scaleSurfaceToWindow(SDL_Renderer* renderer, SDL_Surface* surface)
{
	int renderer_width;
	int renderer_height;
	SDL_GetRendererOutputSize(renderer, &renderer_width, &renderer_height);
	const auto& rect = surface->clip_rect;
	const auto scale = [renderer, surface, renderer_width, renderer_height]()
	{
		const auto width_scale = static_cast<float>(renderer_width) / surface->clip_rect.w;
		const auto height_scale = static_cast<float>(renderer_height) / surface->clip_rect.h;
		return std::min(width_scale, height_scale);
	}();
	const auto new_w = static_cast<int>(rect.w * scale);
	const auto new_h = static_cast<int>(rect.h * scale);
	const auto new_x = (renderer_width - new_w) / 2;
	const auto new_y = (renderer_height - new_h) / 2;
	const SDL_Rect res{new_x, new_y, new_w, new_h};
	return res; 
}

int main(int argc, char* argv[])
{
	const char* img_path = "anakin013.jpg";
	if (argc > 2)
	{
		std::cerr << "You should provide at most one image path" << std::endl;
		return 0;
	}
	else if (argc == 2)
		img_path = argv[1];
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cerr << SDL_GetError() << std::endl;
		return 1;
	}
	constexpr auto window_flag = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
	auto window = SDL_CreateWindow("Image Viewer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, window_flag);
	if (window == nullptr)
	{
		std::cerr << SDL_GetError() << std::endl;
		return 2;
	}
	bool quit = false;
	auto renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr)
	{
		std::cerr << SDL_GetError();
		return 3;
	}
	auto surface = IMG_Load(img_path);
	if (surface == nullptr)
	{
		std::cerr << SDL_GetError() << std::endl;
		return 4;
	}
	auto texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == nullptr)
	{
		std::cerr << SDL_GetError() << std::endl;
		return 5;
	}
	SDL_Event event;
	while (!quit)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				quit = true;
				break;
		}
		SDL_RenderClear(renderer);
		const auto rect = scaleSurfaceToWindow(renderer, surface);
		SDL_RenderCopy(renderer, texture, nullptr, &rect);
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
