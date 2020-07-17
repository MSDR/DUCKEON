#include "Player.h"



Player::Player() {
}

Player::Player(Graphics & graphics, float x, float y)
	: AnimatedSprite(graphics, "Images/duck.png", 0, 0, p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, x, y, 75),
	dx_(0), dy_(0),
	facing_(LEFT),
	isGliding_(false), hasDoubleJump_(true), msSinceGrounded_(0.0f)
	//grounded_(false)
{
	graphics.loadImage("Images/duck.png");
	gravity_ = p_consts::BASE_GRAVITY;
	setUpAnimations();
	playAnimation("Idle");
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, x_, y_);

	SDL_Renderer* renderer = graphics.getRenderer();

	if(!grounded_) {
		SDL_SetRenderDrawColor(renderer, 6, 238, 89, 200);

		SDL_Rect r = { facing_ == LEFT ? boundingBox_.getRight() : boundingBox_.getLeft()-3*globals::SPRITE_SCALE, boundingBox_.getTop(), 
							3*globals::SPRITE_SCALE, 7*globals::SPRITE_SCALE };
		SDL_RenderDrawRect(renderer, &r); 

		if(hasDoubleJump_) {
			float jumpMeter = std::abs((doubleJumpHeight())/p_consts::MAX_DOUBLE_JUMP);
			//std::cout << jumpMeter << std::endl;
			r.y += 7*globals::SPRITE_SCALE - (int)(7*globals::SPRITE_SCALE)*jumpMeter;
			r.h = (7*globals::SPRITE_SCALE*jumpMeter);
			SDL_RenderFillRect(renderer, &r); 
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
	}
		

	//shows player bounding box 
	/*{ r = { boundingBox_.getLeft(), boundingBox_.getTop(), boundingBox_.getWidth(), boundingBox_.getHeight() };
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		//SDL_RenderDrawRect(renderer, &r); 
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); }*/
		 
}

void Player::update(float elapsedTime) {
	msSinceGrounded_ += elapsedTime; //collision check comes after update

	//apply gravity
	if (dy_ <= 0) {
		gravity_ = p_consts::BASE_GRAVITY / 1.2f;
	} else {
		gravity_ = p_consts::BASE_GRAVITY / 0.8f;
		grounded_ = false;
	}
	dy_ = std::min(dy_ + gravity_ * elapsedTime, p_consts::GRAVITY_CAP) * (isGliding_ && dy_ > p_consts::GLIDE_START_SPEED ? p_consts::GLIDE_MULT : 1.0f);

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

void Player::jump() {
	if (!hasDoubleJump_) {
		return;
	}

	if (!grounded_) {
		hasDoubleJump_ = msSinceGrounded_ < p_consts::JUMP_FORGIVENESS_WINDOW ? hasDoubleJump_ : false;
		dy_ = -doubleJumpHeight();
		//std::cout << "double jumpin\n";
		return;
	}

	dy_ -= p_consts::JUMP_SPEED;

	grounded_ = false;
	//std::cout << "jumpin" << std::endl;
}

void Player::move(bool isRunning, Direction dir) {
	dx_ = (std::abs(dx_) <= 0.00001f ? p_consts::MIN_WALK_SPEED : std::abs(dx_));

	if (isRunning) dx_ += (p_consts::MAX_RUN_SPEED  - std::abs(dx_))*0.15;
	else				dx_ += (p_consts::MAX_WALK_SPEED - std::abs(dx_))*0.08;

	if (dir != facing_) {
		dx_ = 0; //adds a single update frame of turn around delay
	}
	if (dir == LEFT) { 
		dx_ *= -1;
	} 

	facing_ = dir;

	std::cout << dx_ << std::endl;
	if(!isGliding_) playAnimation(isRunning ? "Run" : "Waddle");
}


void Player::stopMoving() {
	dx_ = grounded_  ? (1 - p_consts::GROUNDED_FRICTION)*dx_ : 
			isGliding_ ? (1 - p_consts::GLIDING_FRICTION)*dx_ : 
							 (1 - p_consts::AERIAL_FRICTION)*dx_;

	if (!isGliding_) playAnimation("Idle");
	
	/*
	if (currentAnimation_ == (facing_ == RIGHT ? "RunRight" : "RunLeft")) { 	
		dx_ = (facing_ == RIGHT ? 1 : -1) * p_consts::RUN_SPEED / (20 * (frameIndex_ + 1));	
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
	addAnimation(3, 4, 0, "L_Run", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(3, 0, 0, "R_Run", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));

	addAnimation(4, 4, 2, "L_Waddle", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(4, 0, 2, "R_Waddle", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));

	addAnimation(4, 4, 4, "L_Glide", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(4, 0, 4, "R_Glide", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));

	addAnimation(1, 4, 6, "L_Idle", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(1, 0, 6, "R_Idle", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));

	


	/*addAnimation(1, 13, 0, "IdleLeft", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));

	addAnimation(3, 0, 1, "JumpRight", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(3, 13, 1, "JumpLeft", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(1, 3, 1, "FallRight", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(1, 16, 1, "FallLeft", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(3, 4, 1, "LandRight", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(3, 17, 1, "LandLeft", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));

	addAnimation(3, 0, 2, "WalkRight", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(3, 13, 2, "WalkLeft", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));

	addAnimation(3, 0, 3, "StartRunRight", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(4, 3, 3, "RunRight", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(6, 7, 3, "StopRunRight", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(3, 13, 3, "StartRunLeft", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(4, 16, 3, "RunLeft", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));
	addAnimation(6, 20, 3, "StopRunLeft", p_consts::PLAYER_WIDTH, p_consts::PLAYER_HEIGHT, Vector2(0, 0));*/
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
				hasDoubleJump_ = true;
				isGliding_ = false;
				grounded_ = true;	
				msSinceGrounded_ = 0.0f;
				break;
			case sides::TOP :
				y_ = (others.at(i).getBottom())/globals::SPRITE_SCALE;
				//std::cout << "collided top, new y_: " << y_*globals::SPRITE_SCALE << "\n";
				//if (currentAnimation_ == (facing_ == RIGHT ? "LandRight" : "LandLeft") || grounded_ == false) 
				//	land();
				
				dy_ = 0;
				break;
			case sides::RIGHT :
				x_ = (others.at(i).getLeft() - 15*globals::SPRITE_SCALE) / globals::SPRITE_SCALE;
				//std::cout << "collided right\n";
				dx_ = 0;
				break;
			case sides::LEFT :
				x_ = (others.at(i).getRight()) / globals::SPRITE_SCALE;
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
dy_ = -p_consts::JUMP_SPEED;
else
dy_ = -p_consts::JUMP_SPEED * 0.75;
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
dx_ = -p_consts::WALK_SPEED*frameIndex_;
} else {
if (grounded_ && currentAnimation_ != (facing_ == RIGHT ? "JumpRight" : "JumpLeft"))
playAnimation("RunLeft");
dx_ = -p_consts::RUN_SPEED;
}
} else {
if (grounded_ && currentAnimation_ != (facing_ == RIGHT ? "JumpRight" : "JumpLeft"))
playAnimation("WalkLeft");
dx_ = -p_consts::WALK_SPEED;
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
dx_ = p_consts::WALK_SPEED*frameIndex_;
} else {
if(grounded_ && currentAnimation_ != (facing_ == RIGHT ? "JumpRight" : "JumpLeft"))
playAnimation("RunRight");
dx_ = p_consts::RUN_SPEED;
}
} else {
dx_ = p_consts::WALK_SPEED;
if(grounded_ && currentAnimation_ != (facing_ == RIGHT ? "JumpRight" : "JumpLeft"))
playAnimation("WalkRight");
}
facing_ = RIGHT;
}
*/