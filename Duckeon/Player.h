#ifndef PLAYER_H_
#define PLAYER_H_
#include "AnimatedSprite.h"
#include "graphics.h"
#include "Globals.h"

#include <algorithm>
#include <iostream>

namespace p_consts {
	const float MIN_WALK_SPEED = 0.02f;
	const float WALK_ACCELERATION = 0.09;
	const float MAX_WALK_SPEED = 0.04f;
	const float AIR_WALK_SPEED = 0.065f;
	const float RUN_ACCELERATION = 0.03f;
	const float MAX_RUN_SPEED = .13f;
	const float GROUNDED_FRICTION = 0.25f; //btw 1 and 0

	const float JUMP_SPEED = 0.25f;
	const float JUMP_FORGIVENESS_WINDOW = 75.0f;
	const float MIN_DOUBLE_JUMP = 0.095f;
	const float MAX_DOUBLE_JUMP = 0.28f;
	const float DOUBLE_JUMP_CHARGE = 540.0f;
	const float AERIAL_FRICTION = 0.045f; //btw 1 and 0

	const float GLIDE_MULT = 0.575f;
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
	Player(Graphics &graphics, Vector2 spawnPos);
	void draw(Graphics &graphics, bool showCollisionBoxes = false);
	void update(float elapsedTime);
	void updateBoundingBox();

	void respawn(Vector2 spawnPos) { x_ = spawnPos.x; y_ = spawnPos.y; dx_ = 0; dy_ = 0; }

	void jump();
	void move(bool isRunning, bool isDucking, Direction dir);
	void stopMoving(bool isDucking);

	void shoot();

	void playAnimation(std::string animation, bool once = false) {
		std::string anim = (facing_ == LEFT ? "L_" : "R_"); anim += (hasGun_ ? "G_" : "") + animation;
		AnimatedSprite::playAnimation(anim, once); }
	
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

	bool hasGun_;
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