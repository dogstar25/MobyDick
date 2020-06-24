#ifndef DEBUGPANEL_H
#define DEBUGPANEL_H

#include <SDL2/SDL.h>

#include <string>


class DebugPanel
{
public:
	DebugPanel();
	~DebugPanel();

	static DebugPanel& instance();
	void addItem(std::string, std::string);
	void addItem(std::string, float, int);

private:
	SDL_Point m_location;
	int m_itemCount;

};

#endif