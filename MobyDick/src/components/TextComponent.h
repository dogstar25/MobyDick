#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include <string>
#include "Component.h"
class TextComponent : public Component
{
public:
	TextComponent();
	TextComponent(std::string gameObjectId);
	~TextComponent();

	void update() override;


private:

	bool 
		m_isDynamic;
	std::string
		fontId,
		textValue;


};

#endif

