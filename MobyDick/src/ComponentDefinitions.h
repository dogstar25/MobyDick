#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>

struct vector2D {
	float x, y;
};

struct Animation2 {

	std::string m_id;
	float m_speed;
	int m_frameCount;
	int m_currentAnimFrame;
	b2Vec2 m_frameSize;
	std::chrono::steady_clock::time_point m_timeSnapshot;

	//a rectangle pointing to the animation textture of the animation frame to be displayed
	SDL_Rect* m_currentTextureAnimationSrcRect;

	//Array of all x,y coordinates of the top left corner of each animation frame in the texture
	std::vector<SDL_FPoint> m_animationFramePositions;

	SDL_Texture* m_texture;
};

struct ChildObject2 {

	std::string m_gameObjectId;
	short m_locationSlot;
	std::shared_ptr<GameObject> childObject;

};

struct GameObjectPiece2
{

	bool isDestroyed;
	b2Vec2 parentPositionOffset;
	short currentlevel;
	std::chrono::steady_clock::time_point time_snapshot;
	WorldObject* pieceObject;

};

struct CompositeLevel2
{
	short levelNum;
	short strength;
	SDL_Color color;
};

struct CompositeLegendItem2
{
	SDL_Color color;
	std::string gameObjectId;
};

/*
Texture file representing the maps of the composite object and the legend that has the info
Of how to decipher the pixels in the blueprint texture
*/
struct CompositeBlueprint2
{
	std::string textureId;
	std::vector<CompositeLegendItem2> legend;
};

struct GameObjectAttachment {
	std::string gameObjectId;
	vector2D anchorPoint;
	uint16 jointType;
};


/*
Base Composite Class
*/
struct Component {



};

/*
Created for now to sow what fields would live in the GameOBject base class
*/
struct GameObject_Example {


	bool m_removeFromWorld;
	std::string description;

	std::vector<std::shared_ptr<Component>> m_components;


};

struct TransformComponent : Component {

	float
		m_angle;
	vector2D
		m_position,
		m_size;
	bool
		m_absolutePositioning;
};

struct AnimationComponent : Component {

	std::string
		m_currentAnimationState;

	std::map<std::string, Animation2*> m_animations;

};

struct ChildrenComponent : Component {

	int 
		m_childCount;
	float
		m_childPadding;
	bool
		m_childPositionRelative;

	std::array<std::vector<ChildObject2>, constants::CHILD_POSITIONS> m_childObjects;

};

struct RenderComponent : Component {

	std::shared_ptr<Texture> m_texture;
	SDL_Color
		m_color;
	float
		m_xRenderAdjustment,
		m_yRenderAdjustment;
	bool
		m_renderOutline;

};

struct PhysicsComponent : Component {

	b2Body* m_physicsBody;

	float
		m_friction,
		m_restitution,
		m_density,
		m_linearDamping,
		m_collisionRadius,
		m_angularDamping;
	uint16
		m_collisionCategory, 
		m_collisionShape,
		m_physicsType;

};

struct VitalityComponent : Component {

	float
		m_speed,
		m_strength,
		m_health;

	std::chrono::duration<float, std::milli> m_lifetime;
	std::chrono::duration<float, std::milli> m_lifetimeRemaining;
	std::chrono::steady_clock::time_point time_snapshot;

	bool 
		isLifetimeAlphaFade,
		hasInfiniteLifetime;

};

struct WeaponComponent : Component {

	std::string
		m_gameObjectId;
	float
		m_level,
		m_fireOffset;
	vector2D
		m_weaponAnchorPoint;

};

struct TextComponent : Component {

	bool 
		m_isDynamic;
	std::string 
		fontId,
		textValue;

};

struct CompositeComponent : Component {

	short maxlevel;
	float levelUpSpeed;
	std::vector<CompositeLevel2> levels;
	CompositeBlueprint blueprint;

	std::vector<GameObjectPiece2>m_pieces;

};

struct ParticleComponent : Component {

	bool isAvailable;
	std::string poolId;

};

struct AttachmentsComponent : Component {

	std::vector<GameObjectAttachment> m_attachments;

};

