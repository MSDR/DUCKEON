#ifndef SPRITE_H_
#define SPRITE_H_
#include "graphics.h"
#include "Rectangle.h"
#include <SDL.h>
#include <string>
class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, 
		float posX, float posY);

	virtual ~Sprite();
	virtual void update();
	virtual void updateBoundingBox();
	void draw(Graphics &graphics, int x, int y);

	const Rectangle getBoundingBox() const;
	const sides::Side getCollisionSide(Rectangle &other) const;

	const float getWidth() const;
	const float getHeight() const;

protected:
	SDL_Rect sourceRect_;
	SDL_Texture* spriteSheet_;
	float x_;
	float y_;

	Rectangle boundingBox_;



};

#endif SPRITE_H_