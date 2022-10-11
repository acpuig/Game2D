#include "TutorialStage.h"
#include "input.h"
#include "game.h"
#include "world.h"
#include "GameMap.h"
#include "PlayStage.h"
#include <iostream>


sPlayer player;
sAnimal animal2;
Vector2 fence1 = Vector2(104, 65); //blue
Vector2 fence2 = Vector2(120, 65); //orange

void TutorialStage::render(Image& framebuffer, Image& tileset, GameMap* map, Image sprite)
{
	Game* game = Game::instance;

	framebuffer.drawImage(tileset, player.pos.x, player.pos.y, Area(16, 96, 8, 8));					//Player
	framebuffer.drawImage(tileset, animal2.pos.x, animal2.pos.y, Area(48, 8, 8, 8));				//Animal

	framebuffer.drawImage(tileset, fence1.x, fence1.y + 8, Area(40, 88, 8, 8));  					//BLUE FENCE
	framebuffer.drawImage(tileset, fence1.x, fence1.y + 16, Area(40, 88, 8, 8));
	framebuffer.drawImage(tileset, fence1.x, fence1.y + 24, Area(40, 88, 8, 8));
	framebuffer.drawImage(tileset, fence1.x, fence1.y + 32, Area(40, 88, 8, 8));
	framebuffer.drawImage(tileset, fence1.x, fence1.y + 38, Area(40, 88, 8, 8));
	

	framebuffer.drawImage(tileset, fence2.x, fence2.y, Area(40, 96, 8, 8)); 						//ORANGE FENCE
	framebuffer.drawImage(tileset, fence2.x, fence2.y + 8, Area(40, 96, 8, 8));	
	framebuffer.drawImage(tileset, fence2.x, fence2.y + 16, Area(40, 96, 8, 8));
	framebuffer.drawImage(tileset, fence2.x, fence2.y + 24, Area(40, 96, 8, 8));
	framebuffer.drawImage(tileset, fence2.x, fence2.y + 32, Area(40, 96, 8, 8));
	framebuffer.drawImage(tileset, fence2.x, fence2.y + 38, Area(40, 96, 8, 8));

	float time = getTime() * 0.001; //get system time in sec
	int type;

	//size in pixels of a cell, we assume every row has 16 cells so the cell size must be image.width / 16
	int cs = tileset.width / 16;
	//for every cell
	for (int x = 0; x < map->width; ++x) {
		for (int y = 0; y < map->height; ++y)
		{
			//get cell info
			sCell& cell = map->getCell(x, y);
			if (cell.type == 0) //skip empty
				continue;
			//store the type temporary in a local var
			type = cell.type;
			if (type == SNAKE)
				type = SNAKE + (int(time * 2)) % 3;
			else if (type == TREE)
				type = TREE + (int(time * 4)) % 2;
			else if (type == TURTLE)
				type = TURTLE + (int(time * 2)) % 2;
			else if (type == LEAVES)
				type = LEAVES + (int(time * 3)) % 2;
			else if (type == GRASS)
				type = GRASS + (int(time * 4)) % 4;
			else if (type == RAT)
				type = RAT + (int(time * 5)) % 2;
			//compute tile pos in tileset image
			int tilex = ((type % 16) * cs); 	//x pos in tileset
			int tiley = (floor(type / 16) * cs);	//y pos in tileset
			Area area(tilex, tiley, cs, cs); //tile area
			int screenx = (x * cs); //place offset here if you want
			int screeny = (y * cs);
			//avoid rendering out of screen stuff
			if (screenx < -cs || screenx >(int)framebuffer.width ||
				screeny < -cs || screeny >(int)framebuffer.height)
				continue;
			//draw region of tileset inside framebuffer
			framebuffer.drawImage(tileset, 		//image
				screenx, screeny, 	//pos in screen
				area); 		//area
		}
	}
	game->showFramebuffer(&framebuffer);
};

void TutorialStage::update(double seconds_elapsed, GameMap* map, Image& tileset, Image& framebuffer) {
	
	Game* game = Game::instance;
	Vector2 targetanimal2 = animal2.pos;
	Vector2 target = player.pos;

	if (isValid(target, tileset, map)) {
		player.pos = target;
	}
	else if (isValid(Vector2(target.x, player.pos.y), tileset, map)) {
		player.pos = Vector2(target.x, player.pos.y);
	}
	else if (isValid(Vector2(player.pos.x, target.y), tileset, map)) {
		player.pos = Vector2(player.pos.x, target.y);
	}
	else player.pos = player.pos - Vector2(1, 1);

	if (isValid(targetanimal2, tileset, map)) {
		animal2.pos = targetanimal2;
	}
	else if (isValid(Vector2(targetanimal2.x, animal2.pos.y), tileset, map)) {
		animal2.pos = Vector2(targetanimal2.x, animal2.pos.y);
	}
	else if (isValid(Vector2(animal2.pos.x, targetanimal2.y), tileset, map)) {
		animal2.pos = Vector2(animal2.pos.x, targetanimal2.y);
	}
	else animal2.pos = animal2.pos - Vector2(1, 1);


	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	int playerVelocity = 20.0f;
	//player 1 
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		framebuffer.drawRectangle(player.pos.x, player.pos.y, 8, 8, Color::BLACK);
		player.pos.y -= player.playerVelocity * seconds_elapsed;
		target.y -= 1;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		framebuffer.drawRectangle(player.pos.x, player.pos.y, 8, 8, Color::BLACK);
		player.pos.y += player.playerVelocity * seconds_elapsed;
		target.y += 1;
	}


	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key up
	{
		framebuffer.drawRectangle(player.pos.x, player.pos.y, 8, 8, Color::BLACK);
		player.pos.x -= player.playerVelocity * seconds_elapsed;
		target.x -= 1;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key down
	{
		framebuffer.drawRectangle(player.pos.x, player.pos.y, 8, 8, Color::BLACK);
		player.pos.x += player.playerVelocity * seconds_elapsed;
		target.x += 1;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_S)) {
		if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
		{
			framebuffer.drawRectangle(animal2.pos.x, animal2.pos.y, 8, 8, Color::BLACK);
			animal2.pos.y -= animal2.animalVelocity * seconds_elapsed;
			framebuffer.drawImage(tileset, player.pos.x, player.pos.y, Area(40, 0, 8, 8));
		}

		if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
		{
			framebuffer.drawRectangle(animal2.pos.x, animal2.pos.y, 8, 8, Color::BLACK);
			animal2.pos.y += animal2.animalVelocity * seconds_elapsed;
			framebuffer.drawImage(tileset, player.pos.x, player.pos.y, Area(40, 0, 8, 8));
		}

		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key up
		{
			framebuffer.drawRectangle(animal2.pos.x, animal2.pos.y, 8, 8, Color::BLACK);
			animal2.pos.x -= animal2.animalVelocity * seconds_elapsed;
			framebuffer.drawImage(tileset, player.pos.x, player.pos.y, Area(40, 0, 8, 8));
		}

		if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key down
		{
			framebuffer.drawRectangle(animal2.pos.x, animal2.pos.y, 8, 8, Color::BLACK);
			animal2.pos.x += animal2.animalVelocity * seconds_elapsed;
			framebuffer.drawImage(tileset, player.pos.x, player.pos.y, Area(40, 0, 8, 8));
		}
	};

	sCell playerCell = Celda(player.pos.x, player.pos.y, *map);
	sCell animal2Cell = Celda(animal2.pos.x, animal2.pos.y, *map);

	if (playerCell.type == BLUEBUTTON) {
		framebuffer.drawRectangle(fence1.x, fence1.y, 8, 8, Color::BLACK);
		fence1.y += playerVelocity * seconds_elapsed;
	}
	if (playerCell.type == ORANGEBUTTON) {
		framebuffer.drawRectangle(fence2.x, fence2.y, 8, 8, Color::BLACK);
		fence2.y += playerVelocity * seconds_elapsed;
	}

	if (playerCell.type == DOOR) {
		game->synth.playSample("data/Fire Crackers.wav", 1, false);
		game->synth.playSample("data/Ta Da-SoundBible.wav", 1, false);
	}

	if (animal2Cell.type == CAN) {
		game->synth.playSample("data/CoonHoundDogLickin.wav", 1, false);
	}
	if (animal2Cell.type == GRASS) {
		game->synth.playSample("data/mixkit-happy-puppy-barks.wav", 1, false);
	}

};


// Cell/Pixel translator
sCell& TutorialStage::Celda(int x, int y, GameMap map) {
	int character_celdax = x / 8;
	int character_celday = y / 8;
	sCell& characterCell = map.getCell(character_celdax, character_celday);

	return characterCell;
};

bool TutorialStage::isValid(Vector2 target, Image& tileset, GameMap* map) {
	if (target.x <0.0f || target.y <0.0f || target.x>(map->width * 8) || target.y>(map->height * 8)) {
		return false;
	}
	int celdax = target.x / 8;
	int celday = target.y / 8;
	sCell& targetcell = map->getCell(celdax, celday);
	if (targetcell.type == eCellType::FENCE1 ||
		targetcell.type == eCellType::FENCE2 ||
		targetcell.type == eCellType::FENCE3 ||
		targetcell.type == eCellType::FENCE4 ||
		targetcell.type == eCellType::FENCE5 ||
		targetcell.type == eCellType::WALL1 ||
		targetcell.type == eCellType::WALL2 ||
		targetcell.type == eCellType::WALL3 ||
		targetcell.type == eCellType::WALL4 ||
		targetcell.type == eCellType::WALL5 ||
		targetcell.type == eCellType::WALL6 ||
		targetcell.type == eCellType::WALL7 ||
		targetcell.type == eCellType::WALL6) return false;
	else return true;
};


