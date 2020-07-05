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


void EventManager::clearEvents()
{

	//First delete all player input events from last loop
	m_PlayerInputEvents.clear();
	std::vector<std::shared_ptr<PlayerInputEvent>>().swap(m_PlayerInputEvents);
}


void EventManager::pollEvents()
{
	int keyCode = 0, scanCode, keyCount, keyStateCount;
	SDL_Event event;
	//PlayerInputEvent* playerInputEvent = nullptr;
	const Uint8* keyStates = nullptr;
	//Handle special events and everything else should be player control
	//input related so staore it for later
	while (SDL_PollEvent(&event)) {

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
				std::shared_ptr<PlayerInputEvent> playerInputEvent = std::make_unique<PlayerInputEvent>();
				playerInputEvent->event = event;

				//Get the keyboard state array and copy it to our save spot - memcpy!!!
				keyStates = SDL_GetKeyboardState(&keyStateCount);
				memcpy_s(playerInputEvent->keyStates, 
					SDL_NUM_SCANCODES*sizeof(unsigned char), 
					keyStates, 
					keyStateCount*sizeof(unsigned char));

				m_PlayerInputEvents.push_back(playerInputEvent);
			}
			break;
		default:
			std::shared_ptr<PlayerInputEvent> playerInputEvent = std::make_unique<PlayerInputEvent>();
			playerInputEvent->event = event;
			m_PlayerInputEvents.push_back(playerInputEvent);
			break;
		}


	}


}
