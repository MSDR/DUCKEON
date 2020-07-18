#include "Level.h"
#include <iostream>

namespace tiles {
	Tile brick = Tile("brick", true);
	Tile backgroundBricks = Tile("bg_bricks", false);
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
	tileIndex_.insert(std::make_pair("100_100_100", tiles::brick));
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
void Level::loadMap(Graphics &graphics, const std::string &mapName) {
	SDL_Surface* map = graphics.loadImage("Levels/" + mapName + ".png");
	SDL_Surface* collisionMap = graphics.loadImage("Levels/C_" + mapName + ".png");

	if (map == NULL) {
		throw std::string("Map " + mapName + " not found.");
		return;
	}
	/*if (collisionMap == NULL) {
		throw std::string("A collision map for " + mapName + " not found.");
		return;
	}*/

	loadedTiles_.clear();
	collisionRects_.clear();

	//std::cout << "loaded\n";
	size_.x = map->w;
	size_.y = map->h;

	//std::cout << "Size test: " << size_.x << ", " << size_.y;

	int tileCounter = 0;
	std::set<int> collisionTiles; //keeps track of which tiles (using the tilecounter) have been incorporated into collision already
	while (tileCounter < (size_.x * size_.y)) {
		//Adjust tilePos -- tileCounter iterated at bottom of loop
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

		//Process pixel
		if (a == 0) { //empty tile
			//Tile newTile = Tile("bg_bricks", tilePos);
			//loadedTiles_.push_back(newTile);
			//std::cout << "Empty tile at " << tileCounter << std::endl; //<< tilePos.x << ", " << tilePos.y << std::endl;

			tileCounter++;
			continue;
		} else { //non-empty tile
			//Add the new tile to visuals
			std::string rgb = std::to_string(r) + '_' + std::to_string(g) + '_' + std::to_string(b);
			if (tileIndex_.find(rgb) == tileIndex_.end()) {
				//std::cout << "Unrecognized tile with color << " << rgb << " found at [" << tilePos.x/16 << "," << tilePos.y/16 << "]\n";
				tileCounter++;
				continue;
			}
			Tile newTile = Tile(tileIndex_[rgb].getTileName(), tileIndex_[rgb].hasCollision_, tilePos);
			loadedTiles_.push_back(newTile);

			//Deal with collision
			if(tileIndex_[rgb].hasCollision_ && collisionTiles.count(tileCounter) == 0) {
				//Mark current tile positions in overall level logic
				Vector2 currentPos = tilePos;
				int currentTileCounter = tileCounter;

				//Look down from marked row of pixels
				int numRowsIterated = 1;
				int numColsIterated = 1;
				tileCounter += size_.x; //first we iterate by row
				while (tileCounter < (size_.x*size_.y)) {
					//Search down for new rows of pixels in box
					tileX = (tileCounter % size_.x) * 16;
					tileY = ((tileCounter / size_.x) * 16);
					tilePos = Vector2(tileX, tileY);
					//if (tileX != currentPos.x) break;

					//Parse tiles in row
					bool intactRow = true;
					for (int tc = currentTileCounter+size_.x*numRowsIterated; tc <= tileCounter; ++tc) {
						//Parse tiles
						pixel = get_pixel32(map, tc % size_.x, tc / size_.x);
						//std::cout << pixel << "\n";

						SDL_GetRGBA(pixel, map->format, &r, &g, &b, &a);
						std::string rgbNewPixel = std::to_string(r) + '_' + std::to_string(g) + '_' + std::to_string(b);
						if (rgb != rgbNewPixel || collisionTiles.count(tc) > 0) {
							intactRow = false; 
							break;
						}
					}
					if (!intactRow) break;

					//Add the (now known to be intact) row of tiles into collisionTiles
					for (int tc = currentTileCounter+size_.x*numRowsIterated; tc <= tileCounter; ++tc) {
						collisionTiles.insert(tc);
					}

					tileCounter += size_.x;
					numRowsIterated++;
				}
				tileCounter -= size_.x;

				//Look to the right of current pixel
				tileCounter++;
				while (tileCounter < (size_.x * size_.y)) {
					//Search to the right for new pixels in box
					tileX = (tileCounter % size_.x) * 16;
					tileY = ((tileCounter / size_.x) * 16);
					tilePos = Vector2(tileX, tileY);
					if (tileY != currentPos.y) break;

					//Parse tiles
					pixel = get_pixel32(map, tileCounter % size_.x, tileCounter / size_.x);
					//std::cout << pixel << "\n";

					SDL_GetRGBA(pixel, map->format, &r, &g, &b, &a);

					std::string rgbNewPixel = std::to_string(r) + '_' + std::to_string(g) + '_' + std::to_string(b);
					if (rgb != rgbNewPixel || collisionTiles.count(tileCounter) > 0) break;

					collisionTiles.insert(tileCounter);
					tileCounter++;
				}
				tileCounter--;

				//Readjust tilePos -- this represents the bottom right of the collisionRect
				tileX = (tileCounter % size_.x) * 16;
				tileY = ((tileCounter / size_.x) * 16);
				tilePos = Vector2(tileX, tileY);
				tileCounter = currentTileCounter;

				collisionRects_.push_back(Rectangle(currentPos.x*globals::SPRITE_SCALE, currentPos.y*globals::SPRITE_SCALE,
					(tilePos.x - currentPos.x+16)*globals::SPRITE_SCALE, (tilePos.y - currentPos.y+16)*globals::SPRITE_SCALE));
				//std::cout << "Rects x1: " << currentPos.x * globals::SPRITE_SCALE << " Rects y1: " << currentPos.y * globals::SPRITE_SCALE << "\n";
				//std::cout << "Rects x2: " << tilePos.x * globals::SPRITE_SCALE+16 * globals::SPRITE_SCALE << " Rects y2: " << tilePos.y * globals::SPRITE_SCALE+16 * globals::SPRITE_SCALE << "\n";
				//std::cout << std::endl;

				//collisionRects_.push_back(Rectangle(tileX * globals::SPRITE_SCALE, tileY * globals::SPRITE_SCALE,
				//16 * globals::SPRITE_SCALE, 16 * globals::SPRITE_SCALE)); //this is a pretty jank solution to combine visual&collision maps but its ok for now
			}				


			//std::cout << "Loaded " << newTile.getTileName() << " tile at " << tileCounter << std::endl;//<< tilePos.x << ", " << tilePos.y << std::endl;
		}
		
		tileCounter++;
		//std::cout << "tileCounter: " << tileCounter << ", limit: " << size_.x * size_.y << "\n";
	}

}

void Level::update(int elapsedTime) {
	//update geese
}

void Level::draw(Graphics &graphics, bool showCollisionBoxes) {
	for (int i = 0; i < loadedTiles_.size(); ++i) {
		loadedTiles_[i].draw(graphics);
	}

	if(showCollisionBoxes) {
		SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 0, 0, 255);
		for (int c = 0; c < collisionRects_.size(); ++c) {
			SDL_Rect colRect;
			colRect.x = collisionRects_[c].x_;
			colRect.y = collisionRects_[c].y_;
			colRect.w = collisionRects_[c].w_;
			colRect.h = collisionRects_[c].h_;

			SDL_RenderDrawRect(graphics.getRenderer(), &colRect);
		}
		SDL_SetRenderDrawColor(graphics.getRenderer(), 0, 0, 0, 255);
	}
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle & other) {
	std::vector<Rectangle> collidingRects;
	for (int i = 0; i < collisionRects_.size(); ++i) {
		if (collisionRects_.at(i).collidesWith(other)) {
			collidingRects.push_back(collisionRects_.at(i));
			//std::cout << "Colliding Rectangle x: " << collisionRects_.at(i).x_ << "y: " << collisionRects_.at(i).y_ << "\n";
			//std::cout << "Other Rectangle x: " << other.x_ << "y: " << other.y_ << "\n";
		}
	}
	return collidingRects;
}
