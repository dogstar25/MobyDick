#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <SDL2/SDL.h>
#include <vector>

class EventManager
{
public:

	EventManager();
	~EventManager();

	static EventManager& instance();

	void pollEvents();

	//Accessor functions
	std::vector<SDL_Event> playerInputEvents() {
		return m_PlayerInputEvents;
	}

private:

	std::vector<SDL_Event> m_PlayerInputEvents;




};








#endif