#pragma once
#include <SDL.h>
#include <SDL_image.h>

/*
Overloaded operators used throught the game
*/
static bool operator ==(SDL_Color a, SDL_Color b)
{
	return (a.r == b.r) && (a.g == b.g) && (a.b == b.b);
}

static bool operator !=(SDL_Color a, SDL_Color b)
{
	return (a.r != b.r) || (a.g != b.g) || (a.b != b.b);
}

enum GameState {

	QUIT = 0,
	PLAY = 1,
	PAUSE = 2

};

enum GameOjectLayer {

	BACKGROUND = 0,
	MAIN = 1,
	TEXT = 2,
	DEBUG = 3,
};



