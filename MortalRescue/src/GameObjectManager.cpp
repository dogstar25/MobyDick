#include "GameObjectManager.h"
#include "TextureManager.h"
#include "game.h"

bool GameObjectManager::init(TextureManager* textureManager, b2World* physicsWorld)
{


	//Read file and stream it to a JSON object
	Json::Value root;
	ifstream ifs("assets/gameObjectAssets.json");
	ifs >> root;

	string id, textureId;
	GameObject* gameObject;

	for (auto itr : root["gameObjects"])
	{
		if (itr["loadOnInit"].asBool() == true)
		{
			gameObject = new GameObject();
			gameObject->id = itr["id"].asString();
			gameObject->description = itr["description"].asString();
			gameObject->isStaticObject = itr["static"].asBool();
			gameObject->playerSpeed = itr["playerSpeed"].asDouble();
			gameObject->xSize = itr["xSize"].asFloat();
			gameObject->ySize = itr["ySize"].asFloat();
			gameObject->initPosX = itr["initPosX"].asInt();
			gameObject->initPosY = itr["initPosY"].asInt();

			//If this has a textture then get and store it
			if (itr["primativeShape"].isNull() == false)
			{
				gameObject->isPrimitiveShape = true;

				//color
				if (itr["primativeColor"]["random"].asBool() == true)
				{
					gameObject->primativeColor = Game::util.generateRandomColor();
				}
				else
				{
					gameObject->primativeColor.r = itr["primativeColor"]["red"].asInt();
					gameObject->primativeColor.g = itr["primativeColor"]["green"].asInt();
					gameObject->primativeColor.b = itr["primativeColor"]["blue"].asInt();
					gameObject->primativeColor.a = itr["primativeColor"]["alpha"].asInt();
				}
			}

			//If this has a textture then get and store it
			if (itr["texture"].isNull() == false)
			{
				textureId = itr["texture"].asString();
				gameObject->staticTexture = textureManager->getTexture(textureId);
			}

			//If this is a physics object then build the box2d body
			if (itr["physicsObject"].isNull() == false)
			{
				gameObject->isPhysicsObject = true;
				gameObject->friction = itr["physicsObject"]["friction"].asFloat();
				gameObject->density = itr["physicsObject"]["density"].asFloat();
				gameObject->physicsBody = buildB2Body(gameObject, physicsWorld);

			}

			//If this is not an animated object then store its one texture
			if (itr["animations"].isNull() == false)
			{
				gameObject->isAnimated = true;
				for (auto animItr : itr["annimations"])
				{

				}
			}

			this->gameObjectMap[gameObject->id] = *gameObject;
		}
	}

	return true;

}

b2Body * GameObjectManager::buildB2Body(GameObject* gameObject, b2World* physicsWorld)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(gameObject->initPosX , gameObject->initPosY);
	b2Body* body = physicsWorld->CreateBody(&bodyDef);

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	//dynamicBox.SetAsBox(1.0f, 1.0f);
	//dynamicBox.SetAsBox(gameObject->xSize/2, gameObject->ySize/2);
	float32 xSize = gameObject->xSize / 2; //SetAsBox takes half-widths
	float32 YSize = gameObject->ySize / 2;
	dynamicBox.SetAsBox(xSize, YSize);
	std::cout << "BuildBox size for " << gameObject->description << " was " << xSize << "\n";
	//dynamicBox.SetAsBox((gameObject->xSize / 2)*.04, (gameObject->ySize / 2)*.04);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = gameObject->density;

	// Override the default friction.
	fixtureDef.friction = gameObject->friction;
	fixtureDef.restitution = .2;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);
	//body->SetTransform({ gameObject->initPosX, gameObject->initPosY }, 0.0f);

	//body->SetLinearDamping(.2);
	physicsWorld->SetAutoClearForces(true);
	
	return body;

}


GameObject * GameObjectManager::getGameObject(string id)
{
	GameObject *gameObject = nullptr;

	gameObject = &this->gameObjectMap[id];

	return gameObject;
}


void GameObjectManager::testBlocks(SDL_Event* event, b2World* physicsWorld)
{

	std::cout << "Object created " << " \n";
	std::cout << "X " << event->button.x << " \n";
	std::cout << "Y " << event->button.y << " \n";

	GameObject* gameObject;
	gameObject = new GameObject();

	//build id
	int count = this->gameObjectMap.size();
	string id = "block" + to_string(count);
	gameObject->id = id;

	gameObject->description = "block";
	gameObject->xSize = Game::util.generateRandomNumber(1,3);
	gameObject->ySize = Game::util.generateRandomNumber(1, 3);
	gameObject->initPosX = event->button.x / Game::config.scaleFactor;
	gameObject->initPosY = event->button.y / Game::config.scaleFactor;

	gameObject->isPrimitiveShape = true;

	gameObject->primativeColor = Game::util.generateRandomColor();


	gameObject->isPhysicsObject = true;
	gameObject->friction = .3;
	gameObject->density = 1;
	gameObject->physicsBody = buildB2Body(gameObject, physicsWorld);


	this->gameObjectMap[gameObject->id] = *gameObject;

}