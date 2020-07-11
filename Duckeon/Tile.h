#ifndef TILE_H_
#define TILE_H_


#include "Globals.h"
#include "graphics.h"
#include <string>

class Tile {
public:
	Tile();
	Tile(std::string tileName, Vector2 position = Vector2(0,0));

	void update(int elapsedTime);
	void draw(Graphics &graphics);

	std::string getFilePath() { return filePath_; }
	std::string getTileName() { return tileName_; }
private:
	SDL_Texture* image_;
	std::string filePath_;
	std::string tileName_;
	Vector2 size_;
	Vector2 position_;
};
#endif 
