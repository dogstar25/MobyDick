#include "CompositeComponent.h"

#include <memory>

#include "../game.h"
#include "../GameObject.h"



CompositeComponent::CompositeComponent(Json::Value definitionJSON, Scene* parentScene)
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

	_buildComposite(parentScene);

}

CompositeComponent::~CompositeComponent()
{

	m_pieces.clear();

}

void CompositeComponent::update()
{

	_updatePieces();

}

void CompositeComponent::postInit()
{

	if (m_physicsWeldPiecesOn == true) {
		weldOnPieces();
	}

}

void CompositeComponent::render()
{

	for (auto& piece : m_pieces)
	{
		const auto& vitals = piece.pieceObject->getComponent<VitalityComponent>(ComponentTypes::VITALITY_COMPONENT);

		if (vitals->isBroken() == false && vitals->isDestroyed() == false)
		{
			piece.pieceObject->render();
		}
	}

}

void CompositeComponent::weldOnPieces()
{

	for (auto& piece : m_pieces) {

		const auto& piecePhysicsComponent =  piece.pieceObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
		const auto& parentPhysicsComponent = parent()->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
		const auto& pieceTransformComponent = piece.pieceObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
		const auto& parentTransformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
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
		
		parentPhysicsComponent->attachItem(piece.pieceObject.get(), b2JointType::e_weldJoint, weldLocation);

	}

}

void CompositeComponent::_buildComposite(Scene* parentScene)
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
	for (auto y = 0; y < blueprintSurface->h; y++)
	{
		for (auto x = 0; x < blueprintSurface->w; x++)
		{

			//get the pixel at this location
			Uint8* currentPixel = (Uint8*)blueprintSurface->pixels + (uint64_t)y * (uint64_t)blueprintSurface->pitch + (uint64_t)x * (uint64_t)bpp;

			//Parse the pixel info into a color
			SDL_GetRGBA(*(Uint32*)currentPixel, blueprintSurface->format, &red, &green, &blue, &alpha);
			SDL_Color currentPixelcolor = { red, green, blue };

			//Loop through the legend to find which gameObject should be built
			for (CompositeLegendItem legendItem : m_blueprint.legend)
			{
				if (currentPixelcolor == legendItem.color == true)
				{
					_buildPiece(legendItem, x, y, parentScene);
				}
			}

		}
	}

	SDL_UnlockSurface(blueprintSurface);
}

void CompositeComponent::_buildPiece(CompositeLegendItem legendItem, int xPos, int yPos, Scene* parentScene)
{
	float xOffset, yOffset;
	GameObjectPiece piece = {};

	/*
	Build the game objects off screen. They will be placed in expect location during update loop
	*/
	const auto& pieceObject = std::make_shared<GameObject>(legendItem.gameObjectId, -5.f, -5.f, 0.f, parentScene, LAYER_FOREGROUND_2);
	piece.pieceObject = pieceObject;

	//calculate the X,Y offset position in relating to the base object
	const auto& pieceTransformComponent = pieceObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	xOffset = xPos * pieceTransformComponent->size().x;
	yOffset = yPos * pieceTransformComponent->size().y;

	piece.parentPositionOffset.x = xOffset;
	piece.parentPositionOffset.y = yOffset;

	m_pieces.push_back(piece);

}

void CompositeComponent::_updatePieces()
{
	for (auto& pieceObject : m_pieces)
	{

		//Update the position of the piece for composites where pieces are not
		//welded on
		if (m_physicsWeldPiecesOn == false) {
			_updatePiecePosition(pieceObject);
		}

		//The piece object itself is a gameObject that should have its update called
		pieceObject.pieceObject->update();

	}

}

void CompositeComponent::_updatePiecePosition(GameObjectPiece& piece)
{

	b2Vec2 piecePosition{ 0,0 };
	b2Vec2 adjustment{ 0,0 };

	const auto& pieceTransformComponent = piece.pieceObject->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);
	const auto& piecePhysicsComponent = piece.pieceObject->getComponent<PhysicsComponent>(ComponentTypes::PHYSICS_COMPONENT);
	const auto& parentTransformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

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
