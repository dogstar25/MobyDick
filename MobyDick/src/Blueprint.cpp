#include "Blueprint.h"

#include <fstream>

#include "game.h"

extern std::unique_ptr<Game> game;

Blueprint::Blueprint(std::string blueprintName)
{

	std::string definitionFilename = "assets/blueprint/" + blueprintName + "_definition.json";

	Json::Value root;
	std::ifstream definitionFile(definitionFilename);
	definitionFile >> root;

	//Build the legend
	 auto blueprintTextureName = root["blueprint"].asString();
	 auto surface = TextureManager::instance().getTexture(blueprintTextureName)->surface;
	 int bpp = surface->format->BytesPerPixel;
	 Uint8 red, green, blue, alpha;

	 //Log warning if the bluprint image size doesnt match what we ahve in config
	 int surfaceWidth = surface->w;
	 int surfaceHeight = surface->h;

	 //Build temporary location/color mapping map
	 auto& legendJSON = root["legend"];

	 auto colorLocationMap = _buildColorLocationMappingMap(legendJSON);
	 
	 game->_displayLoadingMsg();

	 SDL_LockSurface(surface);

	 //Allocate the 2 dimentional vector
	 m_plan.resize(surfaceWidth, std::vector<PlanItem>(surfaceHeight));

	 //Loop through the first row of the image which stores the legend related color data
	 for (int x = 0; x < surface->w; x++)
	 {
		 int y{ 0 };

		 Uint8* currentPixel = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
		 SDL_GetRGBA(*(Uint32*)currentPixel, surface->format, &red, &green, &blue, &alpha);
		 SDL_Color currentPixelcolor = { red, green, blue };

		 //Build the location string for the color legend map
		 std::stringstream colorlocationObjectType;
		 colorlocationObjectType << "LOC" << "_" << std::setw(4) << std::setfill('0') << x << "_";
		 colorlocationObjectType << std::setw(4) << std::setfill('0') << y;
		 std::string type = colorlocationObjectType.str();

		 //Get the gameObjectType from the color location map
		 std::string gameObjectType = colorLocationMap.at(type);

		 //Store the color/GameId blueprint entry
		 m_legend.emplace(currentPixelcolor, gameObjectType);


	 }





}

std::map<std::string, std::string> Blueprint::_buildColorLocationMappingMap(Json::Value legendDefinition)
{
	auto tempColorLocationMap = std::map<std::string, std::string>();

	for (auto legentItemJSON : legendDefinition) {

		auto gameObjectId = legentItemJSON["gameObjectId"].asString();
		auto locId = legentItemJSON["id"].asString();
		tempColorLocationMap[locId] = gameObjectId;

	}

	return tempColorLocationMap;

}


void Blueprint::_buildBlueprint(std::string definitionFilename, std::string blueprintTexture)
{




}



void Blueprint::_buildLegend(Json::Value legendJSON)
{


}
