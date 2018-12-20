
#include "game.h"

Clock Game::clock;
using namespace chrono_literals;
//#include <chipmunk/chipmunk.h>

bool Game::init()
{

	//Get all of the configuration values
	getConfig();

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) 
	{
		printf("SDL_Init success\n");

		//Create the game window
		pWindow = SDL_CreateWindow(this->windowTitle.c_str(),
			this->windowXpos,
			this->windowYPos,
			this->screenWidth, 
			this->screenHeight, 
			this->windowFlags);

		//Initialize the textture manager
		this->textureManager.init(pWindow);

		//Initilaze the Game Object Manager
		//This will hold all possible game objects that the game/level supports
		this->gameObjectManager.init(&this->textureManager);

		//Initialize me some chipmunk
		

		//Set the main player gameObject as defined by config
		this->player = this->gameObjectManager.getGameObject(this->playerGameObjectId);
		this->player->xPos = 0;
		this->player->yPos = 0;
		m_bRunning = true;

	}

	//Initialize the clock object
	clock.init();

	return true;
}

void Game::update() {

	this->player->update();

}


void Game::render() {

	//NOTE: This will be looping thru all of the game objects that need to be rendered
	this->textureManager.render(this->player);
}

void Game::clean() {
	printf("cleaning game\n");
	SDL_DestroyWindow(this->pWindow);
	SDL_Quit();
}

bool Game::getConfig()
{
	//Read file and stream it to a JSON object
	Json::Value root;
	ifstream ifs("config/gameConfig.json");
	ifs >> root;

	//Get and store config values
	this->gameTitle = root["gameTitle"].asString();
	this->windowTitle = root["windowSettings"]["title"].asString();
	this->screenWidth = root["windowSettings"]["width"].asInt();
	this->screenHeight = root["windowSettings"]["height"].asInt();
	this->playerGameObjectId = root["playerGameObjectId"].asString();

	return true;
}

void Game::handleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_bRunning = false;
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			if ((char)event.key.keysym.sym == SDLK_ESCAPE)
			{
				event.type = SDL_QUIT;
				SDL_PushEvent(&event);
			}
			else
			{
				this->player->handleEvent(&event);
			}

		default:
			break;
		}
	}
}
/**/
void Game::chipmunkTest(void) {
	// cpVect is a 2D vector and cpv() is a shortcut for initializing them.
	cpVect gravity = cpv(0, -100);

	// Create an empty space.
	cpSpace *space = cpSpaceNew();
	cpSpaceSetGravity(space, gravity);

	// Add a static line segment shape for the ground.
	// We'll make it slightly tilted so the ball will roll off.
	// We attach it to a static body to tell Chipmunk it shouldn't be movable.
	cpShape *ground = cpSegmentShapeNew(cpSpaceGetStaticBody(space), cpv(-20, 5), cpv(20, -5), 0);
	cpShapeSetFriction(ground, 1);
	cpSpaceAddShape(space, ground);

	// Now let's make a ball that falls onto the line and rolls off.
	// First we need to make a cpBody to hold the physical properties of the object.
	// These include the mass, position, velocity, angle, etc. of the object.
	// Then we attach collision shapes to the cpBody to give it a size and shape.

	cpFloat radius = 5;
	cpFloat mass = 1;

	// The moment of inertia is like mass for rotation
	// Use the cpMomentFor*() functions to help you approximate it.
	cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

	// The cpSpaceAdd*() functions return the thing that you are adding.
	// It's convenient to create and add an object in one line.
	cpBody *ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
	cpBodySetPosition(ballBody, cpv(0, 15));

	// Now we create the collision shape for the ball.
	// You can create multiple collision shapes that point to the same body.
	// They will all be attached to the body and move around to follow it.
	cpShape *ballShape = cpSpaceAddShape(space, cpCircleShapeNew(ballBody, radius, cpvzero));
	cpShapeSetFriction(ballShape, 0.7);

	// Now that it's all set up, we simulate all the objects in the space by
	// stepping forward through time in small increments called steps.
	// It is *highly* recommended to use a fixed size time step.
	cpFloat timeStep = 1.0 / 60.0;
	for (cpFloat time = 0; time < 2; time += timeStep) {
		cpVect pos = cpBodyGetPosition(ballBody);
		cpVect vel = cpBodyGetVelocity(ballBody);
		printf(
			"Time is %5.2f. ballBody is at (%5.2f, %5.2f). It's velocity is (%5.2f, %5.2f)\n",
			time, pos.x, pos.y, vel.x, vel.y
		);

		cpSpaceStep(space, timeStep);
	}

	// Clean up our objects and exit!
	ChipmunkDemoFreeSpaceChildren(space);
	//cpBodyFree(ballBody);
	//cpShapeFree(ballShape);
	//cpShapeFree(ground);
	cpSpaceFree(space);


	
	
}

static void shapeFreeWrap(cpSpace *space, cpShape *shape, void *unused) {
	cpSpaceRemoveShape(space, shape);
	cpShapeFree(shape);
}

static void postShapeFree(cpShape *shape, cpSpace *space) {
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)shapeFreeWrap, shape, NULL);
}

static void constraintFreeWrap(cpSpace *space, cpConstraint *constraint, void *unused) {
	cpSpaceRemoveConstraint(space, constraint);
	cpConstraintFree(constraint);
}

static void postConstraintFree(cpConstraint *constraint, cpSpace *space) {
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)constraintFreeWrap, constraint, NULL);
}

static void bodyFreeWrap(cpSpace *space, cpBody *body, void *unused) {
	cpSpaceRemoveBody(space, body);
	cpBodyFree(body);
}

static void postBodyFree(cpBody *body, cpSpace *space) {
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)bodyFreeWrap, body, NULL);
}

void
Game::ChipmunkDemoFreeSpaceChildren(cpSpace *space)
{
	// Must remove these BEFORE freeing the body or you will access dangling pointers.
	cpSpaceEachShape(space, (cpSpaceShapeIteratorFunc)postShapeFree, space);
	cpSpaceEachConstraint(space, (cpSpaceConstraintIteratorFunc)postConstraintFree, space);

	cpSpaceEachBody(space, (cpSpaceBodyIteratorFunc)postBodyFree, space);
}

