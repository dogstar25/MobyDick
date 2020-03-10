#pragma once
#include <stdio.h>
#include<map>
#include <string>

#include <SDL_rect.h>

class DebugPanel
{
public:
	DebugPanel();
	~DebugPanel();

	void addItem(std::string, std::string);
	void addItem(std::string, float, int);

private:
	SDL_Point m_location;
	int m_itemCount;

};

