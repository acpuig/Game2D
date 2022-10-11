#include "PlayStage.h"
#include "input.h"
#include "game.h"
#include "world.h"
#include "GameMap.h"
#include <iostream>



sPlayer player1;
sAnimal animal;
Vector2 fence = Vector2 (145,80); //mitj



void PlayStage::render(Image& framebuffer, Image& tileset, GameMap* map, Image sprite)
{
	Game* game = Game::instance;

	float time = getTime() * 0.001; //get system time in sec
	int type;

	framebuffer.drawImage(tileset, player1.pos.x , player1.pos.y , Area(16, 96, 8, 8));					//Player
	framebuffer.drawImage(tileset, animal.pos.x, animal.pos.y, Area(48, 8, 8, 8));						//Dog
	framebuffer.drawImage(tileset, fence.x, fence.y, Area(40, 88, 8, 8));								//Fences
	framebuffer.drawImage(tileset, fence.x-8, fence.y, Area(40, 88, 8, 8));								
	framebuffer.drawImage(tileset, fence.x - 16, fence.y, Area(40, 88, 8, 8));
	framebuffer.drawImage(tileset, fence.x - 16, fence.y+8, Area(40, 88, 8, 8));
	framebuffer.drawImage(tileset, fence.x - 16, fence.y + 16, Area(40, 88, 8, 8));
	framebuffer.drawImage(tileset, fence.x  -16, fence.y +24, Area(40, 88, 8, 8));

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
			//Animations
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


void PlayStage::update(double seconds_elapsed, GameMap* map, Image& tileset, Image& framebuffer) {

	Game* game = Game::instance;
	Vector2 target = player1.pos;
	Vector2 targetanimal = animal.pos;

	//LOOK FOR PLAYERS COLLITON
	if (isValid(target, tileset, map)) {
		player1.pos = target;
	}
	else if (isValid(Vector2(target.x, player1.pos.y), tileset, map)) {
		player1.pos = Vector2(target.x, player1.pos.y);
	}
	else if (isValid(Vector2(player1.pos.x, target.y), tileset, map)) {
		player1.pos = Vector2(player1.pos.x, target.y);
	}
	else player1.pos = player1.pos - Vector2(1, 1);

	//LOOK FOR DOG COLLITON
	if (isValid(targetanimal, tileset, map)) {
		animal.pos = targetanimal;
	}
	else if (isValid(Vector2(targetanimal.x, animal.pos.y), tileset, map)) {
		animal.pos = Vector2(targetanimal.x, animal.pos.y);
	}
	else if (isValid(Vector2(animal.pos.x, targetanimal.y), tileset, map)) {
		animal.pos = Vector2(animal.pos.x, targetanimal.y);
	}
	else animal.pos = animal.pos - Vector2(1, 1);



	//PLAYER 1 MOVEMENT
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		framebuffer.drawRectangle(player1.pos.x, player1.pos.y, 8, 8, Color::BLACK);
		player1.pos.y -= player1.playerVelocity * seconds_elapsed;
		target.y -= 1;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		framebuffer.drawRectangle(player1.pos.x, player1.pos.y, 8, 8, Color::BLACK);
		player1.pos.y += player1.playerVelocity * seconds_elapsed;
		target.y += 1;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key up
	{
		framebuffer.drawRectangle(player1.pos.x, player1.pos.y, 8, 8, Color::BLACK);
		player1.pos.x -= player1.playerVelocity * seconds_elapsed;
		target.x -= 1;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key down
	{
		framebuffer.drawRectangle(player1.pos.x, player1.pos.y, 8, 8, Color::BLACK);
		player1.pos.x += player1.playerVelocity * seconds_elapsed;
		target.x += 1;
	}

	//DOG MOVEMENT
	if (Input::isKeyPressed(SDL_SCANCODE_S)) {
		if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
		{
			framebuffer.drawRectangle(animal.pos.x, animal.pos.y, 8, 8, Color::BLACK);
			animal.pos.y -= animal.animalVelocity * seconds_elapsed;
		}

		if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
		{
			framebuffer.drawRectangle(animal.pos.x, animal.pos.y, 8, 8, Color::BLACK);
			animal.pos.y += animal.animalVelocity * seconds_elapsed;
		}

		if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key up
		{
			framebuffer.drawRectangle(animal.pos.x, animal.pos.y, 8, 8, Color::BLACK);
			animal.pos.x -= animal.animalVelocity * seconds_elapsed;
		}

		if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key down
		{
			framebuffer.drawRectangle(animal.pos.x, animal.pos.y, 8, 8, Color::BLACK);
			animal.pos.x += animal.animalVelocity * seconds_elapsed;
		}
	};

	sCell playerCell = Celda(player1.pos.x, player1.pos.y, *map); 
	sCell animalCell = Celda(animal.pos.x, animal.pos.y, *map);

	//Move blue fance
	if (playerCell.type == BLUEBUTTON) {
		framebuffer.drawRectangle(fence.x, fence.y, 8, 8, Color::BLACK);
		fence.y += player1.playerVelocity * seconds_elapsed; 
	}
	if (playerCell.type == DOOR) {
		game->synth.playSample("data/Fire Crackers.wav", 1, false);
		game->synth.playSample("data/Ta Da-SoundBible.wav", 1, false);
	}
	//Dog Sounds
	if (animalCell.type == CAN) {
		game->synth.playSample("data/CoonHoundDogLickin.wav", 1, false);
	}
	if (animalCell.type == GRASS) {
		game->synth.playSample("data/mixkit-happy-puppy-barks.wav", 1, false);
	}

};
	
// Cell/Pixel translator
sCell& PlayStage::Celda(int x, int y, GameMap map) {
	int character_celdax = x / 8;
	int character_celday = y / 8;
	sCell& characterCell = map.getCell(character_celdax, character_celday);

	return characterCell;
}

//Check if the next cell is busy
bool PlayStage::isValid(Vector2 target, Image& tileset, GameMap* map) {
	if (target.x <0.0f || target.y <0.0f || target.x>(map->width * 8) || target.y>(map->height * 8)) {
		return false;
	}
	int celdax = target.x / 8;
	int celday = target.y / 8;
	sCell& targetcell = map->getCell(celdax, celday);
	if (targetcell.type == FENCE1 ||
		targetcell.type == FENCE2 ||
		targetcell.type == FENCE3 ||
		targetcell.type == FENCE4 ||
		targetcell.type == FENCE5 ||
		targetcell.type == WALL1 ||
		targetcell.type == WALL2 ||
		targetcell.type == WALL3 ||
		targetcell.type == WALL4 ||
		targetcell.type == WALL5 ||
		targetcell.type == WALL6 ||
		targetcell.type == WALL7 || 
		targetcell.type == WALL6 ) return false;
	else return true;
};

/*									-TRY TO REDUCE CODE REPETITION (but won't work)-
void PlayStage::Movement(double seconds_elapsed, Image& framebuffer, int characterX, int characterY, Vector2 target, int characterVelocity) {
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		framebuffer.drawRectangle(characterX, characterY, 8, 8, Color::BLACK);
		characterY -= characterVelocity * seconds_elapsed;
		target.y -= 1;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		framebuffer.drawRectangle(characterX, characterY, 8, 8, Color::BLACK);
		characterY += characterVelocity * seconds_elapsed;
		target.y += 1;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key up
	{
		framebuffer.drawRectangle(characterX, characterY, 8, 8, Color::BLACK);
		characterX -= characterVelocity * seconds_elapsed;
		target.x -= 1;
	}

	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key down
	{
		framebuffer.drawRectangle(characterX, characterY, 8, 8, Color::BLACK);
		characterX += characterVelocity * seconds_elapsed;
		target.x += 1;
	}
}

void PlayStage::MovX(double seconds_elapsed, Image& framebuffer, int characterX, int characterY, Vector2 target, int one, int characterVelocity) {
	framebuffer.drawRectangle(characterX, characterY, 8, 8, Color::BLACK);
	characterX += (characterVelocity * seconds_elapsed * one);
	target.x = target.x + (one);
};

void PlayStage::MovY(double seconds_elapsed, Image& framebuffer, int characterX, int characterY, Vector2 target, int one, int characterVelocity) {
	framebuffer.drawRectangle(characterX, characterY, 8, 8, Color::BLACK);
	characterY = characterY + (characterVelocity * seconds_elapsed * one);
	target.y = target.y + (one);
};
*/