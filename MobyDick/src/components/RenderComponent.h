#ifndef RENDER_COMPONENT_H
#define RENDER_COMPONENT_H

#include <memory>
#include <SDL2/SDL.h>

#include "Component.h"
#include "Texture.h"

class RenderComponent : public Component
{

public:
	RenderComponent();
	~RenderComponent();

	void update() override;

	virtual SDL_FRect  getRenderDestRect();
	void setColor(SDL_Color color) {
		m_color = color;
	}
	void setColorAlpha(int alpha) {
		m_color.a = alpha;
	}
	void setColor(int red, int green, int blue, int alpha);
	void setTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }

	//Accessors
	SDL_Color color() {
		return m_color;
	}
	Texture* texture() {
		return m_texture.get();
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

	void outlineObject(float lineSize);

	SDL_Rect* getRenderTextureRect();
	SDL_Texture* getRenderTexture();
	SDL_Surface* getRenderSurface();

};


#endif

