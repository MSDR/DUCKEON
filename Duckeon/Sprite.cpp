#include "Sprite.h"
#include "Globals.h"
#include <iostream>

Sprite::Sprite() {
}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY) :
	x_(posX),
	y_(posY)
{
	sourceRect_.x = sourceX;
	sourceRect_.y = sourceY;
	sourceRect_.w = width;
	sourceRect_.h = height;

	spriteSheet_ = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));

	if (spriteSheet_ == NULL) {
		std::cout << "\nError: Unable to load image\n";
	}

	boundingBox_ = Rectangle(x_, y_, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
}

Sprite::~Sprite() {
}

void Sprite::draw(Graphics &graphics, int x, int y) {
	SDL_Rect destinationRectangle = { x, y, sourceRect_.w * globals::SPRITE_SCALE, sourceRect_.h * globals::SPRITE_SCALE };
	graphics.blitSurface(spriteSheet_, &sourceRect_, &destinationRectangle);
}

const Rectangle Sprite::getBoundingBox() const {
	return boundingBox_;
}

const sides::Side Sprite::getCollisionSide(Rectangle & other) const {
	float amtRight;
	float amtLeft;
	float amtTop;
	float amtBottom;

	amtRight = getBoundingBox().getRight() - other.getLeft();
	amtLeft = other.getRight() - getBoundingBox().getLeft();
	amtTop = other.getBottom() - getBoundingBox().getTop();
	amtBottom = getBoundingBox().getBottom() - other.getTop();

	float vals[4] = { abs(amtRight), abs(amtLeft), abs(amtBottom), abs(amtTop) };
	float lowest = vals[0];
	for (int i = 0; i < 4; i++) {
		if (vals[i] <= lowest) {
			lowest = vals[i];
		}
	}

	return
		lowest == abs(amtRight) ? sides::RIGHT :
		lowest == abs(amtLeft) ? sides::LEFT :
		lowest == abs(amtTop) ? sides::TOP :
		lowest == abs(amtBottom) ? sides::BOTTOM :
		sides::NONE;
}

void Sprite::update() {
}

void Sprite::updateBoundingBox() {
	boundingBox_ = Rectangle(x_* globals::SPRITE_SCALE, y_* globals::SPRITE_SCALE, sourceRect_.w * globals::SPRITE_SCALE, sourceRect_.h * globals::SPRITE_SCALE);
}

const float Sprite::getWidth() const {
	return sourceRect_.w*globals::SPRITE_SCALE;
}

const float Sprite::getHeight() const {
	return sourceRect_.h*globals::SPRITE_SCALE;
}

