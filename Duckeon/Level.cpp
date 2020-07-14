#include "Level.h"
#include <iostream>

namespace tiles {
	Tile brick = Tile("brick");
	Tile backgroundBricks = Tile("bg_bricks");
	Tile collision = Tile("collision");
}

Level::Level() {
	setUpTiles();
}



//function courtesy of lazyfoo.net/SDL_tutorials/lesson31/index.php
Uint32 get_pixel32(SDL_Surface *surface, int x, int y) {
	//Convert the pixels to 32 bit
	Uint32 *pixels = (Uint32 *)surface->pixels;

	//Get the requested pixel
	return pixels[(y * surface->w) + x];
}

void Level::setUpTiles() {
	tileIndex_.insert(std::make_pair("255_255_255", tiles::brick));
}

bool Level::changeMap(Graphics & graphics, const std::string &mapName, Vector2 spawnPoint) {
	try {
		loadMap(graphics, mapName);
	} catch (std::string error) {
		std::cerr << error;
		return false;
	}
	return true;
}

//ALL LEVELS SHOULD BE 30x17 PIXELS
void Level::loadMap(Graphics & graphics, const std::string &mapName) {
	SDL_Surface* map = graphics.loadImage("Levels/" + mapName + ".png");
	SDL_Surface* collisionMap = graphics.loadImage("Levels/C_" + mapName + ".png");

	if (map == NULL) {
		throw std::string("Map " + mapName + " not found.");
		return;
	}
	if (collisionMap == NULL) {
		throw std::string("A collision map for " + mapName + " not found.");
		return;
	}

	loadedTiles_.clear();
	collisionRects_.clear();

	//std::cout << "loaded\n";
	size_.x = map->w;
	size_.y = map->h;

	//std::cout << "Size test: " << size_.x << ", " << size_.y;

	int tileCounter = 0;
	while (tileCounter < (size_.x * size_.y)) {
		int tileX = (tileCounter % size_.x) * 16;
		int tileY = ((tileCounter / size_.x) * 16);
		Vector2 tilePos = Vector2(tileX, tileY);

		//Parse tiles
		Uint32 pixel = get_pixel32(map, tileCounter % size_.x, tileCounter / size_.x);
		//std::cout << pixel << "\n";
		Uint8 r = 0;
		Uint8 g = 0;
		Uint8 b = 0;
		Uint8 a = 0;

		SDL_GetRGBA(pixel, map->format, &r, &g, &b, &a);
		//std::cout << "r: " << r << ", g: " << g << ", b: " << b << ", a: " << a << "\n";
		if (a == 0) { //empty tile
			//Tile newTile = Tile("bg_bricks", tilePos);
			//loadedTiles_.push_back(newTile);
			std::cout << "Empty tile at " << tilePos.x << ", " << tilePos.y << std::endl;

			tileCounter++;
			continue;
		} else {
			std::string rgb = std::to_string(r) + '_' + std::to_string(g) + '_' + std::to_string(b);
			Tile newTile = Tile(tileIndex_[rgb].getTileName(), tilePos);
			loadedTiles_.push_back(newTile);
			if(rgb=="255_255_255") collisionRects_.push_back(Rectangle(tileX * globals::SPRITE_SCALE, tileY * globals::SPRITE_SCALE,
				16 * globals::SPRITE_SCALE, 16 * globals::SPRITE_SCALE)); //this is a pretty jank solution to combine visual&collision maps but its ok for now
			std::cout << "Loaded " << newTile.getTileName() << " tile at " << tilePos.x << ", " << tilePos.y << std::endl;
		}
		/*
		//Parse collision
		Uint32 pixelC = get_pixel32(collisionMap, tileCounter % size_.x, tileCounter / size_.x);
		SDL_GetRGBA(pixelC, map->format, &r, &g, &b, &a);

		if (a == 0) { //empty tile
			tileCounter++;
			//std::cout << "empty tile\n";
			continue;
		}
		else {
			if (r == 255 && g == 0 && b == 0) {
				//std::cout << "Rects x: " << tileX * globals::SPRITE_SCALE << " Rects y: " << tileY * globals::SPRITE_SCALE << "\n";
				collisionRects_.push_back(Rectangle(tileX * globals::SPRITE_SCALE, tileY * globals::SPRITE_SCALE,
					16 * globals::SPRITE_SCALE, 16 * globals::SPRITE_SCALE));
				//Tile tile(tiles::collision.getTileName(), tilePos); loadedTiles_.push_back(tile);  //REMOVE COMMENT IF SHOW COLLISION
			}
		}
		*/
		tileCounter++;
		//std::cout << "tileCounter: " << tileCounter << ", limit: " << size_.x * size_.y << "\n";
	}

}



void Level::update(int elapsedTime) {

}

void Level::draw(Graphics &graphics) {
	for (int i = 0; i < loadedTiles_.size(); i++) {
		loadedTiles_[i].draw(graphics);
	}
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle & other) {
	std::vector<Rectangle> collidingRects;
	for (int i = 0; i < collisionRects_.size(); i++) {
		if (collisionRects_.at(i).collidesWith(other)) {
			collidingRects.push_back(collisionRects_.at(i));
			//std::cout << "Colliding Rectangle x: " << collisionRects_.at(i).x_ << "y: " << collisionRects_.at(i).y_ << "\n";
			//std::cout << "Other Rectangle x: " << other.x_ << "y: " << other.y_ << "\n";
		}
	}
	return collidingRects;
}
