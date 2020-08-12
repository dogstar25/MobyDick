#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include <memory>
#include <optional>

#include <SDL2/SDL.h>

#include "Component.h"
#include "../Texture.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "../Renderer.h"

class AnimationComponent;
class PhysicsComponent;
class TransformComponent;

class RenderComponent : public Component
{

public:
	RenderComponent();
	RenderComponent(Json::Value definitionJSON);
	~RenderComponent();

	void update() override;
	void render();

	SDL_FRect  getRenderDestRect();
	void setColor(int red, int green, int blue, int alpha);
	void setTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

	void outlineObject(float lineSize, SDL_Color color);
	void setDependencyReferences(GameObject* gameObject);
	void applyDisplayScheme(const uint8_t);
	void removeDisplayScheme();

	SDL_Rect* getRenderTextureRect();
	SDL_Texture* getRenderTexture();
	SDL_Surface* getRenderSurface();
	

	void setColor(SDL_Color color) {
		m_color = color;
	}
	void setColorAlpha(int alpha) {
		m_color.a = alpha;
	}

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
		m_color,
		m_outLineColor;
	float
		m_xRenderAdjustment,
		m_yRenderAdjustment;
	int
		m_outlineThickness;
	bool
		m_renderOutline;
	std::string
		m_textureId;

	std::optional<DisplayScheme> m_displayScheme;

	AnimationComponent* m_animationComponent;
	TransformComponent* m_transformComponent;

};


#endif

