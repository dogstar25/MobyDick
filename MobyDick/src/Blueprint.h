#pragma once


#include <SDL2/SDL.h>

#include <map>

#include "Util.h"
#include "BaseConstants.h"

struct PlanItem
{
	std::string gameObjectType{};
	int layer{ 0 };
	int angleAdjustment{};


};


class Blueprint
{
public:
	Blueprint(std::string blueprintName);

	std::map<SDL_Color, std::string> legend() { return m_legend; }
	std::vector<std::vector<PlanItem>> plan() { return m_plan; }

private:
	std::map<SDL_Color, std::string> m_legend;
	std::vector<std::vector<PlanItem>> m_plan;

	std::string m_blueprintTextureName{};

	void _buildLegend(Json::Value legend);
	void _buildBlueprint(std::string definitionFilename, std::string blueprintTexture);
	std::map<std::string, std::string> _buildColorLocationMappingMap(Json::Value legendDefinition);

};

