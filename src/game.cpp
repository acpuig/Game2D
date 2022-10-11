#include "game.h"
#include "utils.h"
#include "input.h"
#include "game.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "world.h"
#include "PlayStage.h"
#include "TutorialStage.h"
#include "GameMap.h"


#include <cmath>

Game* Game::instance = NULL;



//Create a new Image (or we could create a global one if we want to keep the previous frame)
Image framebuffer(160, 120); //do not change framebuffer size
Image* tileset;
Image font;
Image minifont;
Color bgcolor;
Image sprite;
int press = 0;

GameMap* map;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;
	map = new GameMap(32, 32);
	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.1f;
	tileset = new Image();
	tileset->loadTGA("data/tileset.tga");
	//some new useful functions
	//framebuffer.fill(bgcolor);								//fills the image with one color
	font.loadTGA("data/bitmap-font-black.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	sprite.loadTGA("data/players.tga"); //example to load an sprite

		//propiedades de la clase Game
	tutorial_stage = new TutorialStage();
	play_stage = new PlayStage();

	enableAudio(); //enable this line if you plan to add audio to your application
	//synth.playSample("data/coin.wav",1,true);
	//synth.osc1.amplitude = 0.5;
}

//what to do when the image has to be draw
void Game::render(void)
{
	//// MENU 
		framebuffer.fill(Color::BLACK); 
		framebuffer.drawRectangle(30, 20, 100,20, Color::WHITE);				//draws some text using a bitmap font in an image (assuming every char is 7x9)
		framebuffer.drawText("1. Play", 35, 25, font);				//draws some text using a bitmap font in an image (assuming every char is 7x9)
	
		framebuffer.drawRectangle(30,50 , 100, 20, Color::WHITE);				
		framebuffer.drawText("2. Controls", 35, 55, font);				

		framebuffer.drawRectangle(30, 80, 100, 20, Color::WHITE);				
		framebuffer.drawText("3. Tutorial",35, 85, font);				
		showFramebuffer(&framebuffer);

	 if (press ==1) {
		framebuffer.fill(Color::BLACK);
		map = map->loadGameMap("data/playmap.map");
		play_stage->render(framebuffer, *tileset, map, sprite);
	}
	else if (press ==2) {
		framebuffer.fill(Color::BLACK);
		framebuffer.drawRectangle(10, 10, 140, 100, Color::WHITE);
		framebuffer.drawText("Arrows -> MANs mov", 20, 20, font);
		framebuffer.drawText("S -> MANs+DOG mov ", 20, 50, font);
		framebuffer.drawText("2 -> Menu any time", 20, 75, font);
		showFramebuffer(&framebuffer);
	}
	else if (press ==3) {
		framebuffer.fill(Color::BLACK);
		map = map->loadGameMap("data/tutorial.map");
		tutorial_stage->render(framebuffer, *tileset, map, sprite);
	}
}

void Game::update(double seconds_elapsed)
{

	if (press == 1) {
		framebuffer.fill(Color::BLACK);
		play_stage->update(seconds_elapsed, map, *tileset, framebuffer);
	}
	else if (press == 3) {
		tutorial_stage->update(seconds_elapsed, map, *tileset, framebuffer);
	}

}

//Keyboard event handler (sync input)
void Game::onKeyDown(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
	case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
	case SDLK_1: 	
		 press = 1; 
		break;
	case SDLK_2: 
		 press =2;
		break;
	case SDLK_3:
		 press = 3;
		break;
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Game::onMouseButtonDown(SDL_MouseButtonEvent event)
{
	
	
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Game::onResize(int width, int height)
{
	std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport(0, 0, width, height);
	window_width = width;
	window_height = height;
}

//sends the image to the framebuffer of the GPU
void Game::showFramebuffer(Image* img)
{
	static GLuint texture_id = -1;
	static GLuint shader_id = -1;
	if (!texture_id)
		glGenTextures(1, &texture_id);

	//upload as texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST); glEnable(GL_TEXTURE_2D);
	float startx = -1.0; float starty = -1.0;
	float width = 2.0; float height = 2.0;

	//center in window
	float real_aspect = window_width / (float)window_height;
	float desired_aspect = img->width / (float)img->height;
	float diff = desired_aspect / real_aspect;
	width *= diff;
	startx = -diff;

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(startx, starty + height);
	glTexCoord2f(1.0, 0.0); glVertex2f(startx + width, starty + height);
	glTexCoord2f(1.0, 1.0); glVertex2f(startx + width, starty);
	glTexCoord2f(0.0, 1.0); glVertex2f(startx, starty);
	glEnd();

	/* this version resizes the image which is slower
	Image resized = *img;
	//resized.quantize(1); //change this line to have a more retro look
	resized.scale(window_width, window_height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (1) //flip
	{
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	}
	glDrawPixels( resized.width, resized.height, GL_RGBA, GL_UNSIGNED_BYTE, resized.pixels );
	*/
}

//AUDIO STUFF ********************

SDL_AudioSpec audio_spec;

void AudioCallback(void*  userdata,
	Uint8* stream,
	int    len)
{
	static double audio_time = 0;

	memset(stream, 0, len);//clear
	if (!Game::instance)
		return;

	Game::instance->onAudio((float*)stream, len / sizeof(float), audio_time, audio_spec);
	audio_time += len / (double)audio_spec.freq;
}

void Game::enableAudio()
{
	SDL_memset(&audio_spec, 0, sizeof(audio_spec)); /* or SDL_zero(want) */
	audio_spec.freq = 48000;
	audio_spec.format = AUDIO_F32;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = AudioCallback; /* you wrote this function elsewhere. */
	if (SDL_OpenAudio(&audio_spec, &audio_spec) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_PauseAudio(0);
}

void Game::onAudio(float *buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec)
{
	//fill the audio buffer using our custom retro synth
	synth.generateAudio(buffer, len, audio_spec);
}
