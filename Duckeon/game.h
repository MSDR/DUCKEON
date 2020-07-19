#ifndef GAME_H_
#define GAME_H_

#include "Graphics.h"
#include "Input.h"
#include "Level.h"
#include "Player.h"

#include "SDL_include/SDL.h"

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <utility>

class Game {
public:
	Game();
	~Game();

	void gameLoop();

	//Draws all Sprites
	void draw(Graphics &graphics);

	//Updates member variables appropriately
	void update(float elapsedTime);

private:
	Player player_;
	Level level_;
	SDL_Texture* background_;

	int levelTier_;
};

#endif GAME_H_