#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include <memory>
#include <SDL2/SDL.h>

#include "Component.h"
#include "../Texture.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"

class RenderComponent : public Component
{

public:
	RenderComponent();
	RenderComponent(std::string gameObjectId);
	RenderComponent(Json::Value& componentDetailsJSON);
	RenderComponent(RenderComponent* componentDefinition);
	~RenderComponent();

	void update() override;
	void render();

	SDL_FRect  getRenderDestRect();
	void setColor(SDL_Color color) {
		m_color = color;
	}
	void setColorAlpha(int alpha) {
		m_color.a = alpha;
	}
	void setColor(int red, int green, int blue, int alpha);
	void setTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

	void outlineObject(float lineSize);

	SDL_Rect* getRenderTextureRect();
	SDL_Texture* getRenderTexture();
	SDL_Surface* getRenderSurface();
	

	//void render(SDL_FRect* destRect, SDL_Color color);

	//Accessors
	SDL_Color color() {
		return m_color;
	}
	Texture* texture() {
		return m_texture.get();
	}



private:
	// References to ther componets of the same gameObject
	std::shared_ptr<TransformComponent> m_transformComponent;
	std::shared_ptr<AnimationComponent> m_animationComponent;

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

