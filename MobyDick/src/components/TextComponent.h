#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include <string>
#include "Component.h"
#include "../TextureManager.h"

class TransformComponent;
class RenderComponent;
class Texture;

class TextComponent : public Component
{
public:
	TextComponent();
	TextComponent(std::string gameObjectId);
	~TextComponent();

	void setDependencyReferences(
		std::shared_ptr<TransformComponent> transformComponent,
		std::shared_ptr<RenderComponent> renderComponent);

	void update() override;

	//Accessor functions
	bool isDynamic() {
		return m_isDynamic;
	}
	bool isDebugText() {
		return m_isDebugText;
	}
	std::string fontId() {
		return m_fontId;
	}
	std::string textValue() {
		return m_textValue;
	}
	std::string debugId() {
		return m_debugId;
	}
	int fontSize() {
		return m_fontSize;
	}

private:

	std::shared_ptr<Texture> generateTextTexture();
	std::shared_ptr<Texture> updateDynamicTextTexture();

	void construct();

	int
		m_fontSize;
	bool 
		m_isDynamic,
		m_isDebugText
		;

	std::string
		m_textureId,
		m_fontId,
		m_textValue,
		m_debugId
		;

	std::shared_ptr<RenderComponent> m_refRenderComponent;
	std::shared_ptr<TransformComponent> m_refTransformComponent;
	

};

#endif

