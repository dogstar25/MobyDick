#include "GUIEvent.h"
#include "game.h"

GUIEvent::GUIEvent()
{
}

GUIEvent::GUIEvent(string guiObjectId)
{
	this->state = ACTIVE;
	cout << guiObjectId +"\n";

	GameObjectDefinition* definition;
	definition = game->gameObjectManager.gameObjectDefinitions[guiObjectId];

	//Set the mouse mode
	SDL_ShowCursor(true);
	SDL_SetRelativeMouseMode(SDL_FALSE);

	//Right now ALL GUIEvents are centers on the screen
	//calculate  world grid position for panel in the center of the screen
	int posX, posY;
	
	//Adjust to camera position center
	float centerInPixelsX = game->camera.frame.w / 2;
	float centerInPixelsY = game->camera.frame.h / 2;

	//Adjust to gridsize
	int centerInTilesX = centerInPixelsX / game->worldGridSize.w;
	int centerInTilesY = centerInPixelsY / game->worldGridSize.h;

	//Adjust to center of oject to be center of the point we calculated, otherwise it woudl 
	//position the objects top left corver at the point
	centerInTilesX = centerInTilesX - (definition->xSize / 2 / game->worldGridSize.w);
	centerInTilesY = centerInTilesY - (definition->ySize / 2 / game->worldGridSize.h);


	posX = centerInTilesX;
	posY = centerInTilesY;


	//Panel game object
	GameObject* guiPanel = game->gameObjectManager.buildGameObject <GameObject>("GUIPausePanel", posX, posY, 0);

	this->uiObjectCollections[game->MAIN].gameObjects.push_back(make_unique<GameObject>(*guiPanel));

}

GUIEvent::~GUIEvent()
{

	//Set the mouse mode
	SDL_ShowCursor(false);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	//Free the resources
	for (int x = 0; x < game->MAX_LAYERS; x++)
	{
		this->uiObjectCollections[x].gameObjects.clear();
	}


}

void GUIEvent::run()
{

	this->state = ACTIVE;
	cout << "GUIEvent\n";

	while (this->state != EXITGUI)
	{

		handleInput();

		update();

		render();

	}

}

void GUIEvent::update()
{
	//Update all of the GUI Event gameObjects
	for (auto& gameObjectCollection : this->uiObjectCollections)
	{
		//Update normal game objects
		for (auto& gameObject : gameObjectCollection.gameObjects)
		{
			gameObject->update();
		}

	}
}

void GUIEvent::render()
{

	game->textureManager.clear();

	//Render all of the game objects in the world
	game->renderCollection(&game->gameCollections);

	//Render all of the GUI Event game objects
	game->renderCollection(&this->uiObjectCollections);

	game->textureManager.present();

}

void GUIEvent::handleInput()
{


	SDL_Event event;
	if (SDL_PollEvent(&event))
	{

		switch (event.type)
		{
		case SDL_KEYDOWN:
			if ((char)event.key.keysym.sym == SDLK_ESCAPE)
			{
				state = EXITGUI;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:

			//change the game menu state to exit and send the event
			state = EXITGUI;
			event.type = SDL_QUIT;
			SDL_PushEvent(&event);
			break;
		}
	}





}
