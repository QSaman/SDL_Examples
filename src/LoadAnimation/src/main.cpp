// https://github.com/libsdl-org/SDL_image/blob/release-2.6.0/showanim.c
#include "SDL_image.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_render.h>
#include <SDL_timer.h>
#include <algorithm>
#include <iostream>
#include <vector>

constexpr int WIDTH = 800, HEIGHT = 600;

// Modify below two lines to see the number of rendered times
//int rendered_count = 0;
#define print_rendered_count() //std::cout << "rendered count: " << ++rendered_count << std::endl

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
	const char* animation_path = "giphy.gif";
	if (argc > 2)
	{
		std::cerr << "You should provide at most one image path" << std::endl;
		return 0;
	}
	else if (argc == 2)
		animation_path = argv[1];
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
	auto animation = IMG_LoadAnimation(animation_path);
	if (animation == nullptr)
	{
		std::cerr << SDL_GetError() << std::endl;
		return 4;
	}
	std::vector<SDL_Texture*> textures(animation->count);
	for (std::vector<SDL_Texture*>::size_type i = 0; i < textures.size(); ++i)
	{
		textures[i] = SDL_CreateTextureFromSurface(renderer, animation->frames[i]);
		if (textures[i] == nullptr)
		{
			std::cerr << SDL_GetError() << std::endl;
			return 5;
		}
	}
	int current_frame = 0;
	SDL_Event event;
	auto time_for_next_frame = SDL_GetTicks();
	int timeout = 0;
	while (!quit)
	{
		// We don't want to render the page when it's unnecessary (e.g. instead of an animated gif, we have an image):
		if (timeout == 0)
			SDL_WaitEvent(&event);
		else
			SDL_WaitEventTimeout(&event, timeout);
		switch (event.type)
		{
			case SDL_QUIT:
				quit = true;
				break;
		}
		print_rendered_count();
		SDL_RenderClear(renderer);
		const auto rect = scaleSurfaceToWindow(renderer, animation->frames[current_frame]);
		SDL_RenderCopy(renderer, textures[current_frame], nullptr, &rect);
		SDL_RenderPresent(renderer);
		auto now = SDL_GetTicks();
		if (time_for_next_frame <= now)
		{
			time_for_next_frame = now + animation->delays[current_frame];
			current_frame = (current_frame + 1) % animation->count;
		}
		timeout = time_for_next_frame - now;
	}
	for (auto& texture : textures)
		SDL_DestroyTexture(texture);
	IMG_FreeAnimation(animation);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
