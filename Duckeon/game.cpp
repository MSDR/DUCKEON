#include "game.h"

namespace {
	const int FPS = 60;
	const int MAX_FRAME_TIME = 75;
}

namespace keys {
	SDL_Scancode moveLeft = SDL_SCANCODE_LEFT;
	SDL_Scancode moveRight = SDL_SCANCODE_RIGHT;
	SDL_Scancode run = SDL_SCANCODE_LSHIFT;

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

	player_ = Player(graphics, 50, 200);
	level_ = Level();
	level_.changeMap(graphics, "demo");
	background_ = SDL_CreateTextureFromSurface(graphics.getRenderer(), IMG_Load("Images/background.png"));

	int LAST_UPDATE_TIME = SDL_GetTicks();

	//Start game loop
	while (true) {
		input.beginNewFrame();
		
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

		if (input.isKeyHeld(keys::moveLeft)) {
			player_.move(input.isKeyHeld(keys::run), true);
		} 

		if (input.isKeyHeld(keys::moveRight)) {
			player_.move(input.isKeyHeld(keys::run), false);
		}

		if(!input.isKeyHeld(keys::moveLeft) && !input.isKeyHeld(keys::moveRight)) {
			player_.stopMoving();
		}
		

		const int CURRENT_TIME_MILLIS = SDL_GetTicks();
		int ELAPSED_TIME_MILLIS = CURRENT_TIME_MILLIS - LAST_UPDATE_TIME;
		update(std::min(ELAPSED_TIME_MILLIS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MILLIS;
		draw(graphics);
	}//end game loop
}

//Primary draw function across game
void Game::draw(Graphics &graphics) {
	graphics.clear();
	SDL_RenderCopy(graphics.getRenderer(), background_, NULL, NULL);
	player_.draw(graphics);
	level_.draw(graphics);
	graphics.flip();
}

void Game::update(float elapsedTime) {
	player_.update(elapsedTime);
	level_.update(elapsedTime);


	std::vector<Rectangle> rectsColliding;
	if ((rectsColliding = level_.checkTileCollisions(player_.getBoundingBox())).size() > 0){
		player_.handleTileCollisions(rectsColliding);
	}
	rectsColliding.clear(); //collision called twice due to some quirk that wrongly called for a collision on top if running into a wall
	if ((rectsColliding = level_.checkTileCollisions(player_.getBoundingBox())).size() > 0) {
		player_.handleTileCollisions(rectsColliding);
	}
}