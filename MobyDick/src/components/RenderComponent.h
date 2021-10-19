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
	RenderComponent();
	RenderComponent(Json::Value definitionJSON, int layer);
	~RenderComponent();

	void update() override;
	void render();

	SDL_FRect  getRenderDestRect();
	void setColor(int red, int green, int blue, int alpha);
	void setTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

	void outlineObject(SDL_Color color);
	void setDependencyReferences(GameObject* gameObject);
	void applyDisplayOverlay(displayOverlay);
	void removeDisplayOverlay();

	SDL_Rect* getRenderTextureRect(Texture& texture);
	std::shared_ptr<Texture> getRenderTexture();
	SDL_Surface* getRenderSurface();
	

	void setColor(SDL_Color color) { m_color = color; }
	void setColorAlpha(int alpha) {	m_color.a = alpha; }

	//Accessors
	SDL_Color color() {	return m_color;	}
	std::shared_ptr<Texture> texture() { return m_texture; }
	


private:

	std::shared_ptr<Texture>    m_texture;
	SDL_Color                   m_color;
	SDL_Color                   m_outLineColor;
	float                       m_xRenderAdjustment;
	float                       m_yRenderAdjustment;
	int                         m_outlineThickness;
	bool                        m_renderOutline;
	std::string	                m_textureId;
	SDL_BlendMode				m_textureBlendMode;
	int							m_layer{};

	std::optional<displayOverlay> m_displayOverlay;

	/*AnimationComponent*           m_animationComponent;
	TransformComponent*           m_transformComponent;*/

};


#endif

