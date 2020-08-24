#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include <string>
#include <json/json.h>
#include "Component.h"
#include "../TextureManager.h"
#include <SDL2/SDL.h>

class TransformComponent;
class RenderComponent;


class TextComponent : public Component
{
public:
	TextComponent();
	TextComponent(std::string gameObjectId, Json::Value definitionJSON);
	~TextComponent();

	void update() override;

	//Accessor functions
	bool        isDynamic()	{ return m_isDynamic; }
	bool        isDebugText() { return m_isDebugText; }
	std::string fontId() { return m_fontId; }
	std::string textValue() { return m_textValue; }
	std::string debugId() { return m_debugId; }
	int         fontSize() { return m_fontSize; }

private:

	std::shared_ptr<Texture> generateTextTexture();
	std::shared_ptr<Texture> updateDynamicTextTexture();

	void construct();

	int		    m_fontSize{ 0 };
	bool        m_isDynamic{ false };
	bool		m_isDebugText{ false };
	std::string	m_textureId{ "" };
	std::string m_fontId{ "" };
	std::string m_textValue{ "" };
	std::string m_debugId{ "" };

	//test
	TTF_Font* m_fontObject;

};

#endif

