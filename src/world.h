
#ifndef WORLD_H
#define WORLD_H
#include "framework.h"


enum eDirection : uint8 { DOWN, RIGHT, LEFT, UP };

struct sPlayer {
	Vector2  pos = Vector2 (10,50) ; 
	int width = 8;
	int hight = 8;
	float playerVelocity=20;
};

struct sAnimal {
	Vector2  pos = Vector2(10, 110);
	int width = 8;
	int hight = 8;
	float animalVelocity=20;
};

struct sGameData {
	sPlayer players[1];
};

class World 
{
public:


};

#endif