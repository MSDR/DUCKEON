#ifndef LEVEL_H_
#define LEVEL_H_
#include <string>
#include <vector>
#include "graphics.h"
#include "Globals.h"
#include "Tile.h"
#include "Rectangle.h"
#include <SDL.h>
class Level
{
public:
	Level();

	void update(int elapsedTime);
	void draw(Graphics &graphics, bool showCollisionBoxes = false);

	//Returns true on success, false if error
	bool changeMap(Graphics &graphics, const std::string &mapName, Vector2 spawnPoint = Vector2(100,100));

	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);

	Vector2 getSpawnPoint() {
		return spawnPoint_;
	}

private:
	std::string mapName_;
	Vector2 spawnPoint_;
	Vector2 size_;

	void setUpTiles();
	void loadMap(Graphics &graphics, const std::string &mapName);

	std::map<std::string, Tile> tileIndex_; //key should be "r_g_b". For example, white would be "255_255_255"
	std::vector<Tile> loadedTiles_;
	std::vector<Rectangle> collisionRects_;

};

#endif

