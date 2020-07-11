#ifndef GAME_H_
#define GAME_H_

#include "Graphics.h"
#include "Input.h"
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

	Player player_;

};

#endif GAME_H_