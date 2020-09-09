#include "Renderer_SF.h"
#include "GameConfig.h"

Renderer_SF::Renderer_SF()
{


}

Renderer_SF::~Renderer_SF()
{
}


Renderer_SF& Renderer_SF::instance()
{
	static Renderer_SF singletonInstance;
	return singletonInstance;

}


void Renderer_SF::init()
{
	//create the window
	sf::Window window(sf::VideoMode(GameConfig::instance().windowWidth(), 
		GameConfig::instance().windowHeight()), 
		GameConfig::instance().gameTitle(),
		sf::Style::None);

	//Limit the framerate - Todo:move the 60 to config
	window.setFramerateLimit(60);



 }

bool Renderer_SF::present()
{
	return false;
}

bool Renderer_SF::clear()
{
	return false;
}
