#include "Player.h"
#include <algorithm>
#include <iostream>

namespace player_constants {
	const float RUN_MULT = 2.5f;
	const float WALK_SPEED = 0.07f;
	const float MAX_WALK_SPEED = 0.15f;
	const float JUMP_SPEED = 0.23f;
	const int PLAYER_WIDTH = 16;
	const int PLAYER_HEIGHT = 16;

	const float BASE_GRAVITY = 0.0001f;//0.0007f
	const float GRAVITY_CAP = 0.8f;
}

Player::Player() {
}

Player::Player(Graphics & graphics, float x, float y)
	: AnimatedSprite(graphics, "Images/duckwalk.png", 0, 0, player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, x, y, 75),
	dx_(0),
	dy_(0),
	facing_(RIGHT),
	grounded_(false)
	
{
	graphics.loadImage("Images/duckwalk.png");
	gravity_ = player_constants::BASE_GRAVITY;
	setUpAnimations();
	currentAnimation_ = "L_Idle";
	playAnimation("L_Idle");
}

void Player::draw(Graphics & graphics) {
	AnimatedSprite::draw(graphics, x_, y_);

	//shows player bounding box 
	{   SDL_Renderer* renderer = graphics.getRenderer();
		SDL_Rect r = { boundingBox_.getLeft(), boundingBox_.getTop(), boundingBox_.getWidth(), boundingBox_.getHeight() };
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		//SDL_RenderFillRect(renderer, &r); 
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); }
		 
}

void Player::update(float elapsedTime) {
	//apply gravity
	if (dy_ <= 0) {
		gravity_ = player_constants::BASE_GRAVITY / 1.2f;
	} else {
		gravity_ = player_constants::BASE_GRAVITY / 0.8f;
		if (abs(dy_) > gravity_*100) 
			grounded_ = false;
	}
	dy_ = std::min(dy_ + gravity_ * elapsedTime, player_constants::GRAVITY_CAP);

	x_ += dx_ * elapsedTime; 
	y_ += dy_ * elapsedTime;
	//std::cout << "Player x, " << x_*globals::SPRITE_SCALE << "y: " << y_ * globals::SPRITE_SCALE << "\n";
	//std::cout << "Bounds x, " << boundingBox_.x_ << "y: " << boundingBox_.y_ << " Bound bottom" << boundingBox_.getBottom() << "\n";
	AnimatedSprite::update(elapsedTime);
	updateBoundingBox();
}

void Player::updateBoundingBox() {
	boundingBox_ = Rectangle((x_+1)* globals::SPRITE_SCALE, (y_+4)* globals::SPRITE_SCALE, 14 * globals::SPRITE_SCALE, 12 * globals::SPRITE_SCALE);
}

void Player::move(bool isRunning, bool movingLeft) {
	dx_ = (dx_ <= 0.1f ? player_constants::WALK_SPEED : dx_);

	if (isRunning) dx_ = player_constants::MAX_WALK_SPEED*player_constants::RUN_MULT;
	else dx_ += std::max((player_constants::MAX_WALK_SPEED-std::abs(dx_))/4, 0.0f);

	if (movingLeft) dx_ *= -1;

	std::cout << dx_ << std::endl;
	playAnimation(movingLeft ? "L_Walk" : "R_Walk");
}


void Player::stopMoving() {
	dx_ = 0;
	playAnimation(currentAnimation_.substr(0,1) + "_Idle");
	
	/*
	if (currentAnimation_ == (facing_ == RIGHT ? "RunRight" : "RunLeft")) { 	
		dx_ = (facing_ == RIGHT ? 1 : -1) * player_constants::RUN_SPEED / (20 * (frameIndex_ + 1));	
		playAnimation(facing_ == RIGHT ? "StopRunRight" : "StopRunLeft");
	} else if(currentAnimation_ != "StopRunRight" && currentAnimation_ != "StopRunLeft"){	
		dx_ = 0;
		if(grounded_ && currentAnimation_ != (facing_ == RIGHT ? "JumpRight" : "JumpLeft") && currentAnimation_ != (facing_ == RIGHT ? "LandRight" : "LandLeft"))
			playAnimation(facing_ == RIGHT ? "IdleRight" : "IdleLeft");
	}
	*/
}

void Player::animationDone(std::string currentAnimation) {
	/*if (currentAnimation == (facing_ == RIGHT ? "StartRunRight" : "StartRunLeft")) {
		playAnimation(facing_ == RIGHT ? "RunRight" : "RunLeft");
	} else if (currentAnimation == (facing_ == RIGHT ? "StopRunRight" : "StopRunLeft")) {
		playAnimation(facing_ == RIGHT ? "IdleRight" : "IdleLeft");
	} else if (currentAnimation == (facing_ == RIGHT ? "JumpRight" : "JumpLeft")) {
		overrideGrounded_ = true;
		launch();
		grounded_ = false;
	} else if (currentAnimation == (facing_ == RIGHT ? "LandRight" : "LandLeft")) {
		playAnimation(facing_ == RIGHT ? "IdleRight" : "IdleLeft");
	}*/
}

void Player::setUpAnimations() {
	addAnimation(1, 0, 1, "L_Idle", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(1, 0, 0, "R_Idle", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));

	addAnimation(4, 0, 1, "L_Walk", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(4, 0, 0, "R_Walk", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));

	/*addAnimation(1, 13, 0, "IdleLeft", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));

	addAnimation(3, 0, 1, "JumpRight", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(3, 13, 1, "JumpLeft", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(1, 3, 1, "FallRight", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(1, 16, 1, "FallLeft", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(3, 4, 1, "LandRight", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(3, 17, 1, "LandLeft", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));

	addAnimation(3, 0, 2, "WalkRight", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(3, 13, 2, "WalkLeft", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));

	addAnimation(3, 0, 3, "StartRunRight", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(4, 3, 3, "RunRight", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(6, 7, 3, "StopRunRight", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(3, 13, 3, "StartRunLeft", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(4, 16, 3, "RunLeft", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(6, 20, 3, "StopRunLeft", player_constants::PLAYER_WIDTH, player_constants::PLAYER_HEIGHT, Vector2(0, 0));*/
}

void Player::handleTileCollisions(std::vector<Rectangle>& others) {
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::BOTTOM :
				y_ = (others.at(i).getTop() - 16*globals::SPRITE_SCALE)/globals::SPRITE_SCALE;
				//std::cout << "collided bottom\n";
				dy_ = 0;
				break;
			case sides::TOP :
				y_ = (others.at(i).getBottom())/globals::SPRITE_SCALE;
				//std::cout << "collided top, new y_: " << y_*globals::SPRITE_SCALE << "\n";
				//if (currentAnimation_ == (facing_ == RIGHT ? "LandRight" : "LandLeft") || grounded_ == false) 
				//	land();
				grounded_ = true;	
				dy_ = 0;
				break;
			case sides::RIGHT :
				x_ = (others.at(i).getLeft() - 15*globals::SPRITE_SCALE) / globals::SPRITE_SCALE;
				//std::cout << "collided right\n";
				dx_ = 0;
				break;
			case sides::LEFT :
				x_ = (others.at(i).getRight() + 1*globals::SPRITE_SCALE) / globals::SPRITE_SCALE;
				//std::cout << " collided left\n";
				break;
			}
		}
		updateBoundingBox();
	}
}

const float Player::getX() const {
	return x_;
}

const float Player::getY() const {
	return y_;
}





/*
void Player::disableDoubleJump() {
grounded_ = false;
overrideGrounded_ = false;
}

void Player::startJump() {
if(grounded_)
playAnimation(facing_ == RIGHT ? "JumpRight" : "JumpLeft");
}

void Player::launch() {
if (grounded_ || overrideGrounded_) {
if(overrideGrounded_)
dy_ = -player_constants::JUMP_SPEED;
else
dy_ = -player_constants::JUMP_SPEED * 0.75;
}
playAnimation(facing_ == RIGHT ? "FallRight" : "FallLeft");
}

void Player::land() {
playAnimation(facing_ == RIGHT ? "LandRight" : "LandLeft");
}


void Player::moveLeft(bool isWalking) {
if (!grounded_)
playAnimation("FallLeft");

if (!isWalking) {
if (currentAnimation_ != "RunLeft" && currentAnimation_ != "WalkLeft" && currentAnimation_ != "FallLeft") {
if(grounded_ && currentAnimation_ != (facing_ == RIGHT ? "JumpRight" : "JumpLeft"))
playAnimation("StartRunLeft");
dx_ = -player_constants::WALK_SPEED*frameIndex_;
} else {
if (grounded_ && currentAnimation_ != (facing_ == RIGHT ? "JumpRight" : "JumpLeft"))
playAnimation("RunLeft");
dx_ = -player_constants::RUN_SPEED;
}
} else {
if (grounded_ && currentAnimation_ != (facing_ == RIGHT ? "JumpRight" : "JumpLeft"))
playAnimation("WalkLeft");
dx_ = -player_constants::WALK_SPEED;
}
facing_ = LEFT;
}

void Player::moveRight(bool isWalking) {
if (!grounded_)
playAnimation("FallRight");

if (!isWalking) {
if (currentAnimation_ != "RunRight" && currentAnimation_ != "WalkRight" && currentAnimation_ != "FallRight") {
if(grounded_ && currentAnimation_ != (facing_ == RIGHT ? "JumpRight" : "JumpLeft"))
playAnimation("StartRunRight");
dx_ = player_constants::WALK_SPEED*frameIndex_;
} else {
if(grounded_ && currentAnimation_ != (facing_ == RIGHT ? "JumpRight" : "JumpLeft"))
playAnimation("RunRight");
dx_ = player_constants::RUN_SPEED;
}
} else {
dx_ = player_constants::WALK_SPEED;
if(grounded_ && currentAnimation_ != (facing_ == RIGHT ? "JumpRight" : "JumpLeft"))
playAnimation("WalkRight");
}
facing_ = RIGHT;
}
*/