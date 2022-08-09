#include "Util.h"

#include "EnumMap.h"
#include <random>
#include <format>
#include "GameConfig.h"
#include <iostream>
#include "Scene.h"
#include "Game.h"

extern std::unique_ptr<Game> game;

namespace util
{

	//bool isMouseButtonPressed(uint8 button) {

	//	int mouseX, mouseY;
	//	auto mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);
	//	if (mouseButtons & button) {
	//		return true;
	//	}
	//	else {
	//		return false;
	//	}

	//	

	//}

	void sendSceneEvent(const int sceneActionCode, const std::string& sceneActionCodeId)
	{

		SDL_Event event;

		SceneAction* sceneAction = new SceneAction();
		sceneAction->actionCode = sceneActionCode;
		sceneAction->actionId = sceneActionCodeId;

		event.type = SDL_USEREVENT;
		event.user.data1 = sceneAction;
		SDL_PushEvent(&event);


	}

	const int generateRandomNumber(int min, int max)
	{

		if (min == max) {
			return min;
		}


		/*srand((unsigned)time(0));
		int randomNumber;
		randomNumber = (rand() % max) + min;*/


		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(min, max);

		return (int)dist(mt);
	}

	const float generateRandomNumber(float min, float max)
	{
		if (min == max) {
			return min;
		}

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(min, max);

		return (float)dist(mt);
	}

	const SDL_Color generateRandomColor()
	{
		Uint8 red, green, blue;

		red = generateRandomNumber(0, 255);
		green = generateRandomNumber(0, 255);
		blue = generateRandomNumber(0, 255);
		SDL_Color color;
		color.a = 255;
		color.r = red;
		color.g = green;
		color.b = blue;

		return color;

	}

	const SDL_Color generateRandomColor(SDL_Color beginRange, SDL_Color endRange)
	{
		Uint8 red, green, blue, alpha;

		red = generateRandomNumber(beginRange.r, endRange.r);
		green = generateRandomNumber(beginRange.g, endRange.g);
		blue = generateRandomNumber(beginRange.b, endRange.b);
		alpha = generateRandomNumber(beginRange.a, endRange.a);
		SDL_Color color = {red, green, blue, alpha};

		return color;

	}

	const float radiansToDegrees(float angleInRadians)
	{
		float angle = angleInRadians / b2_pi * 180;
		return angle;

	}

	const float degreesToRadians(float angleInDegrees)
	{
		float angle = angleInDegrees * 0.0174532925199432957f;
		return angle;

	}

	const std::string floatToString(float x, int decDigits)
	{
		//std::stringstream ss;
		//ss << std::fixed;
		//ss.precision(decDigits); // set # places after decimal
		//ss << x;
		//return ss.str();

		auto formattedValue = std::format("{:.{}f}", x, decDigits);
		return formattedValue;

	}

	const SDL_Color JsonToColor(Json::Value JsonColor) 
	{

		SDL_Color color;
		color.r = JsonColor["red"].asInt();
		color.b= JsonColor["blue"].asInt();
		color.g = JsonColor["green"].asInt();
		color.a = JsonColor["alpha"].asInt();

		return color;

	}

	void colorApplyAlpha(SDL_Color& color, int alpha)
	{
		color.a = alpha;
	}

	void colorApplyAlpha(ImVec4& color, int alpha)
	{
		color.w = ((float)alpha/255);
	}

	const ImVec4 JsonToImVec4Color(Json::Value JsonColor) 
	{

		SDL_Color sdlColor = JsonToColor(JsonColor);

		ImVec4 color = { (float)sdlColor.r / 255, (float)sdlColor.g / 255, (float)sdlColor.b / 255, (float)sdlColor.a / 255 };

		return color;

	}

	const ImVec4 SDLColorToImVec4(SDL_Color sdlColor) 
	{

		ImVec4 color = { (float)sdlColor.r / 255, (float)sdlColor.g / 255, (float)sdlColor.b / 255, (float)sdlColor.a / 255 };

		return color;

	}

	b2Vec2 matchParentRotation(SDL_FPoint childPosition, SDL_FPoint parentPosition, float parentAngle)
	{
		b2Vec2 adjustment;

		//calculate radius of circle defined by parent and initial child position
		//This is the hypotenuse
		float radius = 0;
		radius = sqrt(powf((childPosition.x - parentPosition.x), 2) + powf((childPosition.y - parentPosition.y), 2));

		//calculate the angle of where child is at
		float y = childPosition.y - parentPosition.y;
		float x = childPosition.x - parentPosition.x;
		float childAngle = atan2(childPosition.y - parentPosition.y, childPosition.x - parentPosition.x);

		//add parent angle
		float newAngle = childAngle + util::degreesToRadians(parentAngle);
		b2Vec2 newCenterPosition{};
		newCenterPosition.x = (radius * cos(newAngle));
		newCenterPosition.y = (radius * sin(newAngle));

		newCenterPosition.x += parentPosition.x;
		newCenterPosition.y += parentPosition.y;

		adjustment.x = newCenterPosition.x - childPosition.x;
		adjustment.y = newCenterPosition.y - childPosition.y;

		return adjustment;
	}

	float normalizeRadians(float angleInRadians)
	{

		while (angleInRadians < degreesToRadians(0)) {
			angleInRadians += degreesToRadians(360);
		}

		while (angleInRadians > degreesToRadians(360)) {
			angleInRadians -= degreesToRadians(360);
		}

		return angleInRadians;

	}

	float normalizeDegrees(float angleInDegrees)
	{

		while (angleInDegrees < -180) {
			angleInDegrees += 360;
		}

		while (angleInDegrees > 180) {
			angleInDegrees -= 360;
		}

		return angleInDegrees;
	}

	glm::vec2 glNormalizeTextureCoords(glm::vec2 textureCoords, glm::vec2 textureSize)
	{
		//glm::vec2 textureCoordNormalized = { (textureCoords.x + .5) / textureSize.x, (textureCoords.y +.5) / textureSize.y };
		glm::vec2 textureCoordNormalized = { (textureCoords.x ) / textureSize.x, (textureCoords.y ) / textureSize.y };
		//glm::vec2 adjustment = { (float)1 / (2 * textureSize.x), (float)1 / (2 * textureSize.y) };

		return textureCoordNormalized;
	}

	glm::vec4 glNormalizeColor(const glm::uvec4 &color)
	{
		glm::vec4 colorNormalized = { (float)color.r / 255, (float)color.g / 255, (float)color.b / 255, (float)color.a / 255 };
		return colorNormalized;
	}

	glm::vec4 glNormalizeColor(const SDL_Color& color)
	{
		glm::vec4 colorNormalized = { (float)color.r, (float)color.g, (float)color.b, (float)color.a };
		return glNormalizeColor(colorNormalized);
	}

	b2Vec2& toBox2dPoint(b2Vec2& point)
	{
		point.x /= (float)GameConfig::instance().scaleFactor();
		point.y /= (float)GameConfig::instance().scaleFactor();

		return point;
	}

	b2Vec2& toRenderPoint(b2Vec2& point)
	{
		point.x *= (float)25;
		point.y *= (float)25;

		return point;
	}

	glm::vec2& toRenderPoint(glm::vec2& point)
	{
		point.x *= (float)25;
		point.y *= (float)25;

		return point;
	}

	Json::Value getComponentConfig(Json::Value definitionJSON, int componentType)
	{
		for (Json::Value componentJSON : definitionJSON["components"]) {

			std::string id = componentJSON["id"].asString();
			int type = game->enumMap()->toEnum(id);
			if (type == componentType) {
				return componentJSON;
			}

		}

		std::cout << "ComponentType " << componentType << "not found" << std::endl;
		assert(true && "Component not found");
		return Json::Value();
	}

	b2JointDef* createJoint(b2JointType jointType)
	{
		b2JointDef* jointDef = nullptr;

		if (jointType == b2JointType::e_weldJoint) {

			jointDef = new b2WeldJointDef();
		}
		else if (jointType == b2JointType::e_motorJoint) {

			jointDef = new b2MotorJointDef();
		}

		return jointDef;
		
	}

	float calculateDistance(SDL_FPoint location1, SDL_FPoint location2)
	{

		auto distance = std::powf((location1.x - location2.x), 2) + std::powf((location1.y - location2.y), 2);
		distance = std::sqrtf(distance);

		return distance;
	}

	float calculateDistance(glm::vec2 location1, glm::vec2 location2)
	{

		auto distance = glm::distance(location1, location2);

		return distance;
	}

	bool fileExists(const std::string& filename)
	{
		struct stat buf;
		if (stat(filename.c_str(), &buf) != -1)
		{
			return true;
		}
		return false;
	}


}







