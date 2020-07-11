#ifndef ANIMATEDSPRITE_H_
#define ANIMATEDSPRITE_H_

#include "graphics.h"
#include "Sprite.h"
#include "Globals.h"

#include <SDL.h>

#include <map>
#include <string>
#include <vector>


class AnimatedSprite : public Sprite {
public:
	AnimatedSprite();
	AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, float timeToUpdate);


	//plays animation provided if not already playing
	void playAnimation(std::string animation, bool once = false);

	//updates the AnimatedSprite (timer)
	void update(int elapsedTime);

	void draw(Graphics &graphics, int x, int y);


	
protected:
	double timeToUpdate_;
	bool currentAnimationOnce_;
	std::string currentAnimation_;


	virtual void updateBoundingBox() = 0;

	//A required function that sets up animations for a Sprite
	virtual void setUpAnimations() = 0;

	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

	void resetAnimations();

	void stopAnimation();

	void setVisible(bool visible);

	virtual void animationDone(std::string currentAnimation) = 0;

	int frameIndex_;
private: 
	std::map<std::string, std::vector<SDL_Rect> > animations_;
	std::map<std::string, Vector2> offsets_;


	double timeElapsed_;
	bool visible_;


};

#endif

