//Graphics class code adapted from LimeOats: https://limeoats.com/

#include "Globals.h"
#include "graphics.h"
#include <iostream>
Graphics::Graphics() {
	SDL_CreateWindowAndRenderer(globals::WINDOW_WIDTH, globals::WINDOW_HEIGHT,
								0, &window_, &renderer_);
	SDL_SetWindowTitle(window_, "DUCKEON");
	//SDL_SetWindowFullscreen(window_, SDL_TRUE);
	//SDL_DisplayMode DM;
	//SDL_GetWindowDisplayMode(0, &DM);
}

Graphics::~Graphics() {
	SDL_DestroyWindow(window_);
	SDL_DestroyRenderer(renderer_);
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
	if (spriteSheets_.count(filePath) == 0) {
		spriteSheets_[filePath] = IMG_Load(filePath.c_str());
		std::cout << "graphics::loadImage error on " << filePath << ": " << IMG_GetError() << std::endl;
	}

	return spriteSheets_[filePath];
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
	SDL_RenderCopy(renderer_, texture, sourceRectangle, destinationRectangle);
}

void Graphics::flip() {
	SDL_RenderPresent(renderer_);
}

void Graphics::clear() {
	SDL_RenderClear(renderer_);
}

SDL_Renderer* Graphics::getRenderer() const {
	return renderer_;
}