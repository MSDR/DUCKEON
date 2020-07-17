#ifndef PLAYER_H_
#define PLAYER_H_
#include "AnimatedSprite.h"
#include "graphics.h"
#include "Globals.h"

#include <algorithm>
#include <iostream>

namespace p_consts {
	const float MAX_RUN_SPEED = .14f;
	const float WALK_SPEED = 0.02f;
	const float MAX_WALK_SPEED = 0.04f;
	const float GROUNDED_FRICTION = 0.25f; //btw 1 and 0

	const float JUMP_SPEED = 0.227f;
	const float JUMP_FORGIVENESS_WINDOW = 75.0f;
	const float MIN_DOUBLE_JUMP = 0.094f;
	const float MAX_DOUBLE_JUMP = 0.27f;
	const float DOUBLE_JUMP_CHARGE = 520.0f;
	const float AERIAL_FRICTION = 0.045f; //btw 1 and 0

//	const float DOUBLE_JUMP_MULT = 4.20f;
//	const float MIN_DOUBLE_JUMP_SPEED = -0.0265f;
//	const float MAX_DOUBLE_JUMP = 0.495f;
	const float GLIDE_MULT = 0.577f;
	const float GLIDE_START_SPEED = 0.095f;
	const float GLIDING_FRICTION = 0.015f; //btw 1 and 0


	const int PLAYER_WIDTH = 16;
	const int PLAYER_HEIGHT = 16;

	const float BASE_GRAVITY = 0.00081f;//0.0007f
	const float GRAVITY_CAP = 0.8f;
}

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

	void jump();
	void move(bool isRunning, Direction dir);
	void stopMoving();

	void playAnimation(std::string animation, bool once = false)
		{	AnimatedSprite::playAnimation((facing_ == LEFT ? "L_" : "R_") + animation, once); }
	virtual void animationDone(std::string currentAnimation);
	virtual void setUpAnimations();

	void handleTileCollisions(std::vector<Rectangle> &others);

	const float getX() const;
	const float getY() const;
	
	float dx_;
	float dy_;

	bool isGliding_;
	bool hasDoubleJump_;
	bool grounded_;
	float msSinceGrounded_;

private:
	float gravity_;

	float doubleJumpHeight() { 
		return ((msSinceGrounded_ > p_consts::DOUBLE_JUMP_CHARGE ? 1 : std::pow(msSinceGrounded_/p_consts::DOUBLE_JUMP_CHARGE, 3)) *
				  (p_consts::MAX_DOUBLE_JUMP-p_consts::MIN_DOUBLE_JUMP)) +
				 p_consts::MIN_DOUBLE_JUMP;
			
			//std::min(std::abs(p_consts::DOUBLE_JUMP_MULT * std::max(5*dy_/6, p_consts::MIN_DOUBLE_JUMP_SPEED)), 
			//p_consts::MAX_DOUBLE_JUMP); 
	}

	//bool overrideGrounded_; //Set to true when the full jump animation completes, essentially "charging" the double jump

	Direction facing_;
};

#endif