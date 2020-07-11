#include "AnimatedSprite.h"


AnimatedSprite::AnimatedSprite()
{
}

AnimatedSprite::AnimatedSprite(Graphics & graphics, const std::string & filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY, float timeToUpdate) :
	Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
	frameIndex_(0),
	timeElapsed_(0),
	timeToUpdate_(timeToUpdate),
	visible_(true),
	currentAnimationOnce_(false),
	currentAnimation_("")
{

}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
	currentAnimationOnce_ = once;
	if (currentAnimation_ != animation) {
		currentAnimation_ = animation;
		frameIndex_ = 0;
	}
}

void AnimatedSprite::update(int elapsedTime) {
	Sprite::update();

	timeElapsed_ += elapsedTime;

	if (timeElapsed_ > timeToUpdate_) {
		timeElapsed_ -= timeToUpdate_;
		if (frameIndex_ < animations_[currentAnimation_].size() - 1) {
			frameIndex_++;
		} else {
			if (currentAnimationOnce_ == true) {
				setVisible(false);
			}
			frameIndex_ = 0;
			animationDone(currentAnimation_);
		}
	}

}

void AnimatedSprite::draw(Graphics & graphics, int x, int y) {
	if (visible_) {
		SDL_Rect destinationRectangle;
		destinationRectangle.x = (x + offsets_[currentAnimation_].x) * globals::SPRITE_SCALE;
		destinationRectangle.y = (y + offsets_[currentAnimation_].y) * globals::SPRITE_SCALE;
		destinationRectangle.w = sourceRect_.w * globals::SPRITE_SCALE;
		destinationRectangle.h = sourceRect_.h * globals::SPRITE_SCALE;

		SDL_Rect sourceRect = animations_[currentAnimation_][frameIndex_];
		graphics.blitSurface(spriteSheet_, &sourceRect, &destinationRectangle);
	}
}



//X-Value should be the tile number (0-based), NOT the x coordinate of the first tile
void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset){
	std::vector<SDL_Rect> rectangles;

	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { (i + x) * width, y * height, width, height};
		rectangles.push_back(newRect);
	}

	animations_.insert(std::pair<std::string, std::vector<SDL_Rect> >(name, rectangles));
	offsets_.insert(std::pair<std::string, Vector2>(name, offset));
}

void AnimatedSprite::resetAnimations(){
	animations_.clear();
	offsets_.clear();
}

void AnimatedSprite::stopAnimation() {
	frameIndex_ = 0;
	animationDone(currentAnimation_);
}

void AnimatedSprite::setVisible(bool visible) {
	visible_ = visible;
}


