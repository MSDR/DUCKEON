#include "game.h"

namespace settings {
	int DRAW_FPS = 120;
	int UPDATE_FPS = 120;
}

namespace keys {
	SDL_Scancode moveLeft = SDL_SCANCODE_LEFT;
	SDL_Scancode moveRight = SDL_SCANCODE_RIGHT;
	SDL_Scancode run = SDL_SCANCODE_LSHIFT;

	SDL_Scancode duck = SDL_SCANCODE_DOWN;
	SDL_Scancode jump = SDL_SCANCODE_SPACE;

	SDL_Scancode quitGame = SDL_SCANCODE_ESCAPE;
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	if (IMG_Init(IMG_INIT_PNG) == -1) {
		std::cout << IMG_GetError << std::endl;
		SDL_Quit();
	}
	SDL_ShowCursor(SDL_DISABLE);

	gameLoop();
}

Game::~Game() {
	IMG_Quit();
	SDL_Quit();

}

void Game::gameLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;

	player_ = Player(graphics, 120, 200);
	level_ = Level();
	level_.changeMap(graphics, "demo");
	background_ = SDL_CreateTextureFromSurface(graphics.getRenderer(), IMG_Load("Images/background.png"));

	int LAST_UPDATE_TIME = SDL_GetTicks();
	int LAST_DRAW_TIME = LAST_UPDATE_TIME;

	//Start game loop
	while (true) {
		input.beginNewFrame();
		
		//Using SDL_WaitEvent over SDL_PollEvent cuts CPU usage from 17% to 0.1% !
		if (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
				}
			} else if (event.type = SDL_KEYUP) {
				input.keyUpEvent(event);
				
			} else if (event.type == SDL_QUIT) {
				return;
			}
		}

		if (input.wasKeyPressed(keys::quitGame)) {
			return;
		} 

		if (input.wasKeyPressed(SDL_SCANCODE_R)) {
			player_.x_ = 50; player_.y_ = 200; player_.dy_ = 0;
		}

		//Movement
		if (input.isKeyHeld(keys::moveLeft)) {
			player_.move(input.isKeyHeld(keys::run), input.isKeyHeld(keys::duck), LEFT);
		} 
		if (input.isKeyHeld(keys::moveRight)) {
			player_.move(input.isKeyHeld(keys::run), input.isKeyHeld(keys::duck), RIGHT);
		}
		if(!input.isKeyHeld(keys::moveLeft) && !input.isKeyHeld(keys::moveRight)) {
			player_.stopMoving(input.isKeyHeld(keys::duck));
		}

		if (input.wasKeyPressed(keys::jump)) {
			player_.jump();
		}
		if (input.wasKeyReleased(keys::jump) && player_.grounded_ == false) {
			player_.jump();
		}
		if (!player_.grounded_ && input.isKeyHeld(keys::jump)) {
			player_.isGliding_ = true;
			player_.playAnimation("Glide");
		}
		if (input.wasKeyReleased(keys::jump)) {
			player_.isGliding_ = false;
			player_.playAnimation("Idle");
		}

		int CURRENT_TIME_MILLIS = SDL_GetTicks();
		int ELAPSED_TIME_MILLIS = CURRENT_TIME_MILLIS - LAST_UPDATE_TIME;

		if(ELAPSED_TIME_MILLIS < 1000/settings::UPDATE_FPS) 
			SDL_Delay(1000 / settings::UPDATE_FPS - ELAPSED_TIME_MILLIS);

		CURRENT_TIME_MILLIS = SDL_GetTicks();
		ELAPSED_TIME_MILLIS = CURRENT_TIME_MILLIS - LAST_UPDATE_TIME;

		update(ELAPSED_TIME_MILLIS);
		LAST_UPDATE_TIME = CURRENT_TIME_MILLIS;
		
		if(CURRENT_TIME_MILLIS-LAST_DRAW_TIME > 1000/settings::DRAW_FPS) {
			draw(graphics);
			LAST_DRAW_TIME = SDL_GetTicks();
		}
	}//end game loop
}

//Primary draw function across game
void Game::draw(Graphics &graphics) {
	graphics.clear();
	//SDL_RenderCopy(graphics.getRenderer(), background_, NULL, NULL);
	player_.draw(graphics, true);
	level_.draw(graphics, true);
	graphics.flip();
}

void Game::update(float elapsedTime) {
	player_.update(elapsedTime);
	level_.update(elapsedTime);

	//std::vector<Rectangle> rectsHurting
	//make a level::getHurtCollisions(player_.getBoundingBox, player_.getBulletBoxes)
	//the level holds all the geese and their bullets, the player holds their bullets
	std::vector<Rectangle> rectsColliding;
	if ((rectsColliding = level_.checkTileCollisions(player_.getBoundingBox())).size() > 0){
		player_.handleTileCollisions(rectsColliding);
	}
	rectsColliding.clear(); //collision called twice due to some quirk that wrongly called for a collision on top if running into a wall
	if ((rectsColliding = level_.checkTileCollisions(player_.getBoundingBox())).size() > 0) {
		player_.handleTileCollisions(rectsColliding);
	}
}