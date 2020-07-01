#include "PlayerObject.h"


#include "WeaponObject.h"
#include "Animation.h"
#include "GameConfig.h"
#include "Game.h"



PlayerObject::PlayerObject()
{
}

PlayerObject::PlayerObject(std::string gameObjectId, int xMapPos, int yMapPos, int angleAdjust) :
	WorldObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	this->direction = 0;
	this->strafe = 0;
	this->setCurrentAnimationState("IDLE");
	this->pieceCollectedCount = 0;



}

PlayerObject::~PlayerObject()
{


}

void PlayerObject::handlePlayerMovementEvent()
{

	int keyCode=0, scanCode, keyCount, keyStateCount;
	float angularVelocity = 0;
	SDL_Event event;

/*
Handle all user input 
Escape key is special and creates a user defined event that will be captured in PLAY game loop
*/
	while (keyCode != SDLK_ESCAPE && SDL_PollEvent(&event)) {

		switch (event.type)
		{
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			keyCode = event.key.keysym.sym;
			//Special Escape key check to allow for quitting
			if (keyCode == SDLK_ESCAPE)
			{
				std::string* actionCode = new std::string("GUI_PAUSE_PANEL");
				SDL_Event event;
				event.user.data1 = static_cast<void*>(actionCode);
				event.type = SDL_USEREVENT;
				SDL_PushEvent(&event);
				break;
			}
			else
			{
				this->direction = 0;
				this->strafe = 0;
				const Uint8* keyStates = SDL_GetKeyboardState(&keyStateCount);

				if (keyStates[SDL_SCANCODE_W])
				{
					this->direction = -1;
				}
				if (keyStates[SDL_SCANCODE_S])
				{
					this->direction = 1;
				}
				if (keyStates[SDL_SCANCODE_A])
				{
					this->strafe = 1;
				}
				if (keyStates[SDL_SCANCODE_D])
				{
					this->strafe = -1;
				}
			}
			break;

		case SDL_MOUSEMOTION:
			angularVelocity = event.motion.xrel * GameConfig::instance().mouseSensitivity();
			this->physicsBody()->SetAngularVelocity(angularVelocity);
			DebugPanel::instance().addItem("ANGULAR_VELOCITY", std::to_string(angularVelocity));
			break;
		case SDL_MOUSEBUTTONDOWN:
			fire();
			break;
		default:
			break;
		}


	}

}


void PlayerObject::update()
{

	//Call base game object update
	WorldObject::update();

	this->handlePlayerMovementEvent();

	//Call all player movement updates
	this->updatePlayerMovement();

//test
	DebugPanel::instance().addItem("PLAYERX", std::to_string(this->physicsBody()->GetTransform().p.x));
	DebugPanel::instance().addItem("PLAYERY", std::to_string(this->physicsBody()->GetTransform().p.y));


}

void PlayerObject::render()
{

	DebugPanel::instance().addItem("ANIMATIONState", this->currentAnimationState());
	DebugPanel::instance().addItem("ANIMATIONFrame",
		std::to_string(this->animations()[this->currentAnimationState()]->getCurrentAnimFrame()));
	WorldObject::render();
}


void PlayerObject::updatePlayerMovement()
{
	/*
	Lots of tweeks can be down here depending on the game you are building for smoother movement
	*/

	float acceleration = this->speed();
	float velocity = acceleration;

	//Calc direction XY
	//float dx = cos(this->physicsBody->GetAngle()) * velocity * this->direction; // X-component.
	//float dy = sin(this->physicsBody->GetAngle()) * velocity * this->direction; // Y-component.
	float dx = cos(1.5708) * velocity * this->direction; // X-component.
	float dy = sin(1.5708) * velocity * this->direction; // Y-component.

	//calc strafe xy and add direction and strafe vectors
	//1.5708 is 90 degrees
	//float sx = cos(this->physicsBody->GetAngle() + 1.5708) * velocity * this->strafe; // X-component.
	//float sy = sin(this->physicsBody->GetAngle() + 1.5708) * velocity * this->strafe; // Y-component.
	float sx = cos((1.5708) + 1.5708) * velocity * this->strafe; // X-component.
	float sy = sin((1.5708) + 1.5708) * velocity * this->strafe; // Y-component.

	//Create the vector for forward/backward  direction
	b2Vec2 directionVector = b2Vec2(dx, dy);

	//Create the vector for strafe direction
	b2Vec2 strafeVector = b2Vec2(sx, sy);

	//Initialize new final movement vector
	b2Vec2 vec2;
	vec2.SetZero();

	vec2 = (directionVector + strafeVector);

	//Update Animation state
	if (vec2.Length() > 0)
	{
		this->setCurrentAnimationState("RUN");
	}
	else
	{
		this->setCurrentAnimationState("IDLE");
	}

	//this->physicsBody->SetTransform(vec3, this->physicsBody->GetAngle());
	this->physicsBody()->SetLinearVelocity(vec2);

	//physicsBody()->ApplyLinearImpulseToCenter(vec2, true);

	//std:cout << "angle is " << this->physicsBody->GetAngle() << "\n";


}

void PlayerObject::fire()
{
	
	//Calculate the origin of the bullet
	b2Vec2 origin = { this->physicsBody()->GetTransform().p.x , this->physicsBody()->GetTransform().p.y };
	this->weapon()->fire(origin, this->physicsBody()->GetAngle(), this->definition()->fireOffset);


}

void PlayerObject::weaponLevelUp()
{
	weapon()->levelUp();
	
}

void PlayerObject::incrementPiecesCollected()
{

	pieceCollectedCount += 1;
	
	//attemp to level up weapon
	if (weapon()->checkLevelUp(pieceCollectedCount) == true)
	{
		this->weaponLevelUp();
		pieceCollectedCount = 0;
	}
	
	
}


void PlayerObject::setBox2DUserData(PlayerObject* playerObject)
{

	this->physicsBody()->SetUserData(playerObject);
}


