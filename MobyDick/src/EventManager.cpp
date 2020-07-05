#include "EventManager.h"
#include "game.h"

EventManager::EventManager()
{

	m_PlayerInputEvents.reserve(256);

}

EventManager::~EventManager()
{

}

EventManager& EventManager::instance()
{
	static EventManager singletonInstance;
	return singletonInstance;

}

void EventManager::pollEvents()
{
	int keyCode = 0, scanCode, keyCount, keyStateCount;
	SDL_Event event;

	//First delete all player input events from last loop
	m_PlayerInputEvents.clear();
	std::vector<SDL_Event>().swap(m_PlayerInputEvents);

	//Handle special events and everything else should be player control
	//input related so staore it for later
	if (SDL_PollEvent(&event)) {

		switch (event.type)
		{
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			keyCode = event.key.keysym.sym;
			if (keyCode == SDLK_ESCAPE)
			{
				//std::unique_ptr<GUIEvent> guiEvent = std::make_unique<GUIEvent>("GUIPausePanel");
				Game::instance().setGameState(QUIT);
				//guiEvent->run();
			}
			else
			{
				m_PlayerInputEvents.push_back(event);
			}
			break;
		default:
			m_PlayerInputEvents.push_back(event);
			break;
		}


	}


}
