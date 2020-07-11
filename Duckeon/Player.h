#ifndef PLAYER_H_
#define PLAYER_H_
#include "AnimatedSprite.h"
#include "graphics.h"
#include "Globals.h"
#include <SDL.h>


class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, float x, float y);
	void draw(Graphics &graphics);
	void update(float elapsedTime);
	void updateBoundingBox();

	//void disableDoubleJump();
	//void startJump();
	//void launch();
	//void land();
	//void moveLeft(bool isWalking);
	//void moveRight(bool isWalking);

	void move(bool isRunning, bool movingLeft);
	void stopMoving();

	virtual void animationDone(std::string currentAnimation);
	virtual void setUpAnimations();

	void handleTileCollisions(std::vector<Rectangle> &others);

	const float getX() const;
	const float getY() const;
	
private:
	float dx_;
	float dy_;

	float gravity_;

	bool grounded_;
	bool overrideGrounded_; //Set to true when the full jump animation completes, essentially "charging" the double jump

	Direction facing_;
};

#endif