#ifndef PLAYSTAGE_H
#define PLAYSTAGE_H
#include "framework.h"
#include "GameMap.h"
#include "image.h"


class PlayStage
{
public:

	bool PlayStage::isValid(Vector2 target, Image& tileset, GameMap* map);
	void  PlayStage::render(Image& framebuffer, Image& tileset, GameMap* map, Image sprite);
	void PlayStage::update(double seconds_elapsed, GameMap* map, Image& tileset, Image& framebuffer);
	sCell& PlayStage::Celda(int x, int y, GameMap map);
	//void PlayStage::Movement(double seconds_elapsed, Image& framebuffer, int characterX, int characterY, Vector2 target, int characterVelocity);
	//void PlayStage::MovY(double seconds_elapsed, Image& framebuffer, int characterX, int characterY, Vector2 target, int one, int characterVelocity);
	//void PlayStage::MovX(double seconds_elapsed, Image& framebuffer, int characterX, int characterY, Vector2 target, int one, int characterVelocity);



};

#endif