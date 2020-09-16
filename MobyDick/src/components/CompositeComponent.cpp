#include "CompositeComponent.h"

#include <memory>

#include "../game.h"
#include "../GameObject.h"



CompositeComponent::CompositeComponent(Json::Value definitionJSON)
{
	Json::Value compositeComponentJSON = definitionJSON["compositeComponent"];


	Json::Value bluePrintJSON = compositeComponentJSON["blueprint"];

	m_physicsWeldPiecesOn = compositeComponentJSON["physicsWeldPiecesOn"].asBool();

	m_blueprint.textureId = bluePrintJSON["texture"].asString();

	for (Json::Value itrlegend : bluePrintJSON["legend"]) {

		auto& legendItem = m_blueprint.legend.emplace_back();
		legendItem.gameObjectId = itrlegend["gameObjectId"].asString();

		legendItem.color = util::JsonToColor(itrlegend["color"]);

	}

	for (Json::Value itrlevel : compositeComponentJSON["levels"]) {

		auto& levelItem = m_levels.emplace_back();
		levelItem.levelNum = itrlevel["level"].asInt();
		levelItem.durability = itrlevel["durability"].asInt();

		levelItem.color = util::JsonToColor(itrlevel["color"]);

	}

	_buildComposite();

}

CompositeComponent::~CompositeComponent()
{

	m_pieces.clear();

}

void CompositeComponent::update()
{

	_updatePieces();

}

void CompositeComponent::render()
{

	for (GameObjectPiece pieceObject : m_pieces)
	{
		if (pieceObject.isDestroyed == false)
		{
			pieceObject.pieceObject->render();
		}
	}

}

void CompositeComponent::weldOnPieces()
{

	for (auto& piece : m_pieces) {

		auto& piecePhysicsComponent =  piece.pieceObject->physicsComponent.value();
		auto& parentPhysicsComponent = parent()->physicsComponent.value();
		auto& pieceTransformComponent = piece.pieceObject->transformComponent.value();
		auto& parentTransformComponent = parent()->transformComponent.value();
		auto pieceSize = pieceTransformComponent->size();
		auto parentSize = parentTransformComponent->size();

		//calculate the center offset 
		b2Vec2 center = { 
			parentTransformComponent->size().x / 2 / GameConfig::instance().scaleFactor(), 
			parentTransformComponent->size().y / 2 / GameConfig::instance().scaleFactor() 
		};

		//calculate the base parent location to weld this piece on
		auto x = (piece.parentPositionOffset.x  - parentSize.x/2 + (pieceSize.x / 2)) / GameConfig::instance().scaleFactor();
		auto y = (piece.parentPositionOffset.y  - parentSize.y/2 + (pieceSize.y / 2)) / GameConfig::instance().scaleFactor();

		b2Vec2 weldLocation = { x,y };
		
		parentPhysicsComponent->attachItem(piece.pieceObject.get(), weldLocation);

	}

}

void CompositeComponent::_buildComposite()
{

	SDL_Surface* blueprintSurface;
	std::string blueprintTexureId;
	Uint8 red, green, blue, alpha;

	//Get the texture and the surface
	blueprintTexureId = m_blueprint.textureId;
	blueprintSurface = TextureManager::instance().getTexture(blueprintTexureId)->surface;
	int bpp = blueprintSurface->format->BytesPerPixel;

	SDL_LockSurface(blueprintSurface);

	//Loop through entire image, top to bottom, left to right and build the pieces
	for (int y = 0; y < blueprintSurface->h; y++)
	{
		for (int x = 0; x < blueprintSurface->w; x++)
		{

			//get the pixel at this location
			Uint8* currentPixel = (Uint8*)blueprintSurface->pixels + y * blueprintSurface->pitch + x * bpp;

			//Parse the pixel info into a color
			SDL_GetRGBA(*(Uint32*)currentPixel, blueprintSurface->format, &red, &green, &blue, &alpha);
			SDL_Color currentPixelcolor = { red, green, blue };

			//Loop through the legend to find which gameObject should be built
			for (CompositeLegendItem legendItem : m_blueprint.legend)
			{
				if (currentPixelcolor == legendItem.color == true)
				{
					_buildPiece(legendItem, x, y);
				}
			}

		}
	}

	SDL_UnlockSurface(blueprintSurface);
}

void CompositeComponent::_buildPiece(CompositeLegendItem legendItem, int xPos, int yPos)
{
	float xOffset, yOffset;
	GameObjectPiece piece = {};
	piece.currentlevel = 1;
	piece.isDestroyed = false;
	piece.time_snapshot = std::chrono::steady_clock::now();

	/*
	Build the game objects off screen. They will be placed in expect location during update loop
	*/
	const auto& pieceObject = std::make_shared<GameObject>(legendItem.gameObjectId, LAYER_MAIN, -5.f, -5.f);
	pieceObject->init();

	auto& pieceVitalityComponent = pieceObject->vitalityComponent.value();

	pieceVitalityComponent->setDurability(m_levels[0].durability);
	piece.pieceObject = pieceObject;

	//calculate the X,Y offset position in relating to the base object
	auto& pieceTransformComponent = pieceObject->transformComponent.value();
	xOffset = xPos * pieceTransformComponent->size().x;
	yOffset = yPos * pieceTransformComponent->size().y;

	piece.parentPositionOffset.x = xOffset;
	piece.parentPositionOffset.y = yOffset;

	//Temp color setting
	auto& pieceRenderComponent = pieceObject->renderComponent.value();

	//Initialize color and strength to level 1
	pieceRenderComponent->setColor(m_levels[0].color);

	m_pieces.push_back(piece);

}

void CompositeComponent::_updatePieces()
{
	for (auto& pieceObject : m_pieces)
	{

		//Update the state of the piece
		_updatePieceState(pieceObject);

		//Update the position of the piece for composites where pieces are not
		//welded on
		if (m_physicsWeldPiecesOn == false) {
			_updatePiecePosition(pieceObject);
		}

		//The piece object itself is a gameObject that should have its update called
		pieceObject.pieceObject->update();

	}

}

void CompositeComponent::_updatePieceState(GameObjectPiece& piece)
{
	//Get now time
	std::chrono::steady_clock::time_point now_time = std::chrono::steady_clock::now();

	//Should this object be removed?
	if (piece.pieceObject->removeFromWorld() == true)
	{
		auto& piecePhysicsComponent = piece.pieceObject->physicsComponent.value();
		piecePhysicsComponent->setPhysicsBodyActive(false);
		piece.isDestroyed = true;
		piece.time_snapshot = now_time;
		piece.pieceObject->setRemoveFromWorld(false);
	}

	//Has enough time gone by to regenerate the next armor level
	if (piece.isDestroyed == true)
	{
		std::chrono::duration<double, std::milli> timeDiffMilliSeconds = now_time - piece.time_snapshot;
		if (timeDiffMilliSeconds.count() >= m_levelUpSpeed)
		{

			//Level up the piece object
			_levelUp(piece);
		}

	}


}

void CompositeComponent::_updatePiecePosition(GameObjectPiece& piece)
{

	b2Vec2 piecePosition{ 0,0 };
	b2Vec2 adjustment{ 0,0 };

	auto& pieceTransformComponent = piece.pieceObject->transformComponent.value();
	auto& piecePhysicsComponent = piece.pieceObject->physicsComponent.value();
	auto& parentTransformComponent = parent()->transformComponent.value();

	//calculate the X,Y offset position in relating to the base object
	SDL_FRect parentPositionRect = parentTransformComponent->getPositionRect();
	SDL_FPoint parentCenterPosition = parentTransformComponent->getCenterPosition();

	//piecePositions
	SDL_FRect piecePositionRect{};
	piecePositionRect.x = parentPositionRect.x + piece.parentPositionOffset.x;
	piecePositionRect.y = parentPositionRect.y + piece.parentPositionOffset.y;
	piecePositionRect.w = pieceTransformComponent->size().x;
	piecePositionRect.h = pieceTransformComponent->size().y;

	//Adjust the piece position based on the base objects rotation/angle
	//Both the parent and piece position needs to be their center
	SDL_FPoint piecePos = { piecePositionRect.x += piecePositionRect.w /2, piecePositionRect.y += piecePositionRect.h /2 };
	SDL_FPoint parentPos = { parentCenterPosition.x , parentCenterPosition.y };
	adjustment = util::matchParentRotation(piecePos, parentPos, parentTransformComponent->angle());
	piecePositionRect.x += adjustment.x;
	piecePositionRect.y += adjustment.y;

	//Adjust for box2d scale size
	piecePositionRect.x /= GameConfig::instance().scaleFactor();
	piecePositionRect.y /= GameConfig::instance().scaleFactor();

	//Convert the angle to radians
	auto parentRadianAngle = util::degreesToRadians(parentTransformComponent->angle());

	//Create a vec position object to pass to setPosition
	piecePosition.x = piecePositionRect.x;
	piecePosition.y = piecePositionRect.y;

	piecePhysicsComponent->setTransform(piecePosition, parentRadianAngle);

	//piece.pieceObject->setPosition(piecePosition, parentTransformComponent->angle());

}


void CompositeComponent::_levelUp(GameObjectPiece& piece)
{
	int nextLevel = piece.currentlevel + 1;

	for (CompositeLevel level : m_levels)
	{

		piece.isDestroyed = false;

		if (level.levelNum == nextLevel)
		{
			auto& vitalityComponent = piece.pieceObject->vitalityComponent.value();
			auto& physicsComponent = piece.pieceObject->physicsComponent.value();
			auto& renderComponent = piece.pieceObject->renderComponent.value();

			piece.currentlevel = level.levelNum;
			//			piece.isDestroyed = false;

			renderComponent->setColor(level.color);
			vitalityComponent->setDurability(level.durability);
			physicsComponent->setPhysicsBodyActive(true);

		}

	}


}
