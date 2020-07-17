#include "Tile.h"



Tile::Tile()
{
}

//limeoats also had a Vector2 size as the second parameter
Tile::Tile(std::string tileName, bool hasCollision, Vector2 position) :
	filePath_(std::string("Images/Tiles/" + tileName + ".png")),
	tileName_(tileName),
	hasCollision_(hasCollision),
	size_(Vector2(16, 16)) , //if tile size ever changes, edit int tileX = tileCounter % size_.x * 16; in Level.cpp accordingly also the line where I add a rect to the collisionBoxes
	position_(Vector2(position.x * globals::SPRITE_SCALE, position.y * globals::SPRITE_SCALE))
{
}

void Tile::update(int elapsedTime) {
}

void Tile::draw(Graphics &graphics) {
	if(image_ == nullptr)
		image_ = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath_));
	SDL_Rect sourceRect = { 0, 0, size_.x, size_.y };
	SDL_Rect destRect = { position_.x, position_.y, size_.x * globals::SPRITE_SCALE, size_.y * globals::SPRITE_SCALE };
	graphics.blitSurface(image_, &sourceRect, &destRect);
}