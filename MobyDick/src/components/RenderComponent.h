#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include <memory>
#include <optional>

#include <SDL2/SDL.h>

#include "Component.h"
#include "../Texture.h"
#include "TransformComponent.h"
#include "AnimationComponent.h"
#include "../RendererSDL.h"

class AnimationComponent;
class PhysicsComponent;
class TransformComponent;

class RenderComponent : public Component
{

public:
	RenderComponent() = default;
	RenderComponent(Json::Value componentJSON);
	~RenderComponent();

	void update() override;
	void render();
	void render(SDL_FRect destQuad);
	void postInit() override;

	SDL_FRect getRenderDestRect();
	SDL_FRect getRenderDestRect(SDL_FRect& positionRect);
	void setColor(int red, int green, int blue, int alpha);
	void setTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

	void outlineObject(SDL_Color color);
	void setDependencyReferences(GameObject* gameObject);
	void applyDisplayOverlay(DisplayOverlay displayOverlay);
	void removeDisplayOverlay();


	SDL_Rect* getRenderTextureRect(std::shared_ptr<Texture> texture);
	std::shared_ptr<Texture> getRenderTexture();
	SDL_Surface* getRenderSurface();
	
	void setColor(SDL_Color color) { m_color = color; }
	void setColorAlpha(int alpha) {	m_color.a = alpha; }

	void setParallaxRate(float parallaxRate) { m_parallaxRate = parallaxRate; }
	std::optional<float> parallaxRate() { return m_parallaxRate; }

	//Accessors
	SDL_Color color() {	return m_color;	}
	std::shared_ptr<Texture> texture() { return m_texture; }
	RenderBlendMode textureRenderMode() { return m_textureBlendMode; }
	
private:

	std::shared_ptr<Texture> m_texture;
	SDL_Color m_color;
	SDL_Color m_outLineColor;
	bool m_renderOutline;
	std::string	m_textureId;
	RenderBlendMode	m_textureBlendMode;
	std::optional<float> m_parallaxRate{};

	std::optional<DisplayOverlay> m_displayOverlay;

};


#endif

