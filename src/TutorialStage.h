#ifndef TUTORIAL_H
#define TUTORIAL_H
#include "framework.h"
#include "GameMap.h"
#include "image.h"


class TutorialStage
{
public:

	bool TutorialStage::isValid(Vector2 target, Image& tileset, GameMap* map);
	void  TutorialStage::render(Image& framebuffer, Image& tileset, GameMap* map, Image sprite);
	void TutorialStage::update(double seconds_elapsed, GameMap* map, Image& tileset, Image& framebuffer);
	sCell& TutorialStage::Celda(int x, int y, GameMap map);



};

#endif