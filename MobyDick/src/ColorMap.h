#ifndef COLOR_MAP_H
#define COLOR_MAP_H

#include "Util.h"
#include "BaseConstants.h"
#include <SDL2/SDL.h>

class ColorMap {

public:
	ColorMap();

	static ColorMap& instance();
	const SDL_Color toSDLColor(std::string name);
	const ImVec4 toIMGUIColor(std::string name);
	void addColor(std::string id, SDL_Color color) {

		m_colorMap[id] = color;
	}

private:
	std::map<std::string, SDL_Color> m_colorMap;

};

#endif

