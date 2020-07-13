#ifndef PLAYER_H_
#define PLAYER_H_
#include "AnimatedSprite.h"
#include "graphics.h"
#include "Globals.h"

#include <algorithm>

namespace p_consts {
	const float RUN_MULT = 1.75f;
	const float WALK_SPEED = 0.0225f;
	const float MAX_WALK_SPEED = 0.14f;
	const float JUMP_SPEED = 0.225f;
	const float DOUBLE_JUMP_MULT = 4.20f;
	const float MIN_DOUBLE_JUMP_SPEED = -0.0265f;
	const float MAX_DOUBLE_JUMP = 0.495f;
	const float GLIDE_MULT = 0.575f;
	const float GLIDE_START_SPEED = 0.1f;


	const int PLAYER_WIDTH = 16;
	const int PLAYER_HEIGHT = 16;

	const float BASE_GRAVITY = 0.0008f;//0.0007f
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
	void move(bool isRunning, bool movingLeft);
	void stopMoving();

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

private:
	float gravity_;

	float doubleJumpHeight() { 
		return std::min(std::abs(p_consts::DOUBLE_JUMP_MULT * std::max(5*dy_/6, p_consts::MIN_DOUBLE_JUMP_SPEED)), 
			p_consts::MAX_DOUBLE_JUMP); 
	}

	//bool overrideGrounded_; //Set to true when the full jump animation completes, essentially "charging" the double jump

	Direction facing_;
};

#endif