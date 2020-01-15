#include "game.h"
#include "CompositeObject.h"


CompositeObject::CompositeObject(string gameObjectId, float xMapPos, float yMapPos, float angleAdjust) :
	WorldObject(gameObjectId, xMapPos, yMapPos, angleAdjust)
{

	this->buildComposite();

}

CompositeObject::~CompositeObject()
{


}
void CompositeObject::update()
{
	//Call base game object update
	WorldObject::update();
}

void CompositeObject::render()
{
	WorldObject::render();
}

void CompositeObject::buildComposite()
{


	
}

void CompositeObject::updatePieces()
{



}
