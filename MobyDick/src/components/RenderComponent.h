#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include <memory>
#include <SDL2/SDL.h>

#include "Component.h"
#include "../Texture.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"

class AnimationComponent;
class PhysicsComponent;
class TransformComponent;


class RenderComponent : public Component
{

public:
	RenderComponent();
	RenderComponent(Json::Value definitionJSON);
	~RenderComponent();

	void update(std::shared_ptr<GameObject>gameObject) override;
	void render(std::shared_ptr<GameObject>gameObject);

	SDL_FRect  getRenderDestRect(std::shared_ptr<GameObject>gameObject);
	void setColor(SDL_Color color) {
		m_color = color;
	}
	void setColorAlpha(int alpha) {
		m_color.a = alpha;
	}
	void setColor(int red, int green, int blue, int alpha);
	void setTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

	void outlineObject(std::shared_ptr<GameObject>gameObject, float lineSize);
	void setDependencyReferences(std::shared_ptr<TransformComponent> transformComponent,
		std::shared_ptr<AnimationComponent> animationComponent,
		std::shared_ptr<PhysicsComponent> physicsComponent);

	SDL_Rect* getRenderTextureRect(std::shared_ptr<GameObject>gameObject);
	SDL_Texture* getRenderTexture(std::shared_ptr<GameObject>gameObject);
	SDL_Surface* getRenderSurface();
	

	//void render(SDL_FRect* destRect, SDL_Color color);

	//Accessors
	SDL_Color color() {
		return m_color;
	}
	std::shared_ptr<Texture> texture() {
		return m_texture;
	}
	


private:

	std::shared_ptr<Texture> m_texture;
	SDL_Color
		m_color;
	float
		m_xRenderAdjustment,
		m_yRenderAdjustment;
	bool
		m_renderOutline;
	std::string
		m_textureId;

};


#endif

