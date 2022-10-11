#pragma once
#include "framework.h"

enum eCellType : uint8 {
	EMPTY = 0, START = 132, WATER = 108,
	WALL1 = 1, WALL2 = 2, WALL3 = 4, WALL4 = 17,
	WALL5 = 20, WALL6 = 33, WALL7 = 36, WALL8 = 49, WALL9 = 50, DOOR = 3,
	FENCE1 = 60, FENCE2 = 62, FENCE3 = 61, FENCE4 = 93, FENCE5 = 56, DOORFENCE1 = 56, DOORFENCE2 = 56, CAN = 108,
	BLUEBUTTON = 97, ORANGEBUTTON = 178, TREE = 184, TURTLE = 216, LEAVES = 200, GRASS = 232, SNAKE = 248, RAT = 219
}; 
enum eItemType : uint8 { NOTHING = 0 };

struct sMapHeader {
	int w; //width of map
	int h; //height of map
	unsigned char bytes; //num bytes per cell
	unsigned char extra[7]; //filling bytes, not used
};

struct sCell { //every cell of the world
	eCellType type; //which type (rock,water,etc)
	eItemType typeItem; 
	Vector2  pos;

};

class GameMap {
public:
	int width;
	int height;
	sCell* data;

	GameMap()
	{
		width = height = 0;
		data = NULL;
	}

	GameMap(int w, int h)
	{
		width = w;
		height = h;
		data = new sCell[w*h];
	}

	sCell& getCell(int x, int y)
	{
		return data[x + y * width];
	};

	sCell& getCellAtWorldPos(int posX, int posY)
	{
		int tileX = posX / 16;
		int tileY = posY / 16;
		return getCell(tileX, tileY);
	};

	GameMap* GameMap::loadGameMap(const char* filename);

};


