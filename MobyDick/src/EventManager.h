#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <SDL2/SDL.h>
#include <vector>
#include <memory>

struct PlayerInputEvent
{

	unsigned char keyStates[SDL_NUM_SCANCODES];
	SDL_Event event;
};

class EventManager
{
public:

	EventManager();
	~EventManager();

	static EventManager& instance();

	void pollEvents();
	void clearEvents();

	//Accessor functions
	std::vector<std::shared_ptr<PlayerInputEvent>> playerInputEvents() {
		return m_PlayerInputEvents;
	}

private:

	std::vector<std::shared_ptr<PlayerInputEvent>> m_PlayerInputEvents;




};








#endif