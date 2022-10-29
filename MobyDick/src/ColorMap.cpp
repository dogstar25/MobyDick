#include "ColorMap.h"
#include <assert.h>
#include "BaseConstants.h"

ColorMap::ColorMap()
{

	m_colorMap["Colors::WHITE"] = Colors::WHITE;
	m_colorMap["Colors::BLACK"] = Colors::BLACK;
	m_colorMap["Colors::RED"] = Colors::RED;
	m_colorMap["Colors::GREEN"] = Colors::GREEN;
	m_colorMap["Colors::BLUE"] = Colors::BLUE;
	m_colorMap["Colors::YELLOW"] = Colors::YELLOW;
	m_colorMap["Colors::PURPLE"] = Colors::PURPLE;
	m_colorMap["Colors::ORANGE"] = Colors::ORANGE;
	m_colorMap["Colors::GREY"] = Colors::GREY;
	m_colorMap["Colors::CLOUD"] = Colors::CLOUD;

}

const SDL_Color ColorMap::toSDLColor(std::string name)
{
	assert(m_colorMap.find(name) != m_colorMap.end() && "Color Name wasnt found in ColorMap");

	return m_colorMap[name];
}

const ImVec4 ColorMap::toIMGUIColor(std::string name)
{
	assert(m_colorMap.find(name) != m_colorMap.end() && "Color Name wasnt found in ColorMap");

	SDL_Color sdlColor = m_colorMap[name];
	ImVec4 ImColor{ (float)sdlColor.r / 255, (float)sdlColor.g / 255, (float)sdlColor.b / 255, (float)sdlColor.a / 255};
	return ImColor;
}



