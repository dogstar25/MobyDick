#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <map>

#include <json/json.h>
#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdlrenderer.h"



/*
Overloaded operators used throught the game
*/
static bool operator ==(SDL_Color a, SDL_Color b)
{
	return (a.r == b.r) && (a.g == b.g) && (a.b == b.b);
}

static bool operator !=(SDL_Color a, SDL_Color b)
{
	return (a.r != b.r) || (a.g != b.g) || (a.b != b.b);
}

static bool operator <(SDL_Color a, SDL_Color b)
{
	return (a.r < b.r) || (a.g < b.g) || (a.b < b.b);
}

namespace util
{
	const int generateRandomNumber(int min, int max);
	const float generateRandomNumber(float min, float max);
	const SDL_Color generateRandomColor();
	const SDL_Color generateRandomColor(SDL_Color beginRange, SDL_Color endRange);
	const float radiansToDegrees(float angleInRadians);
	const float degreesToRadians(float angleInDegrees);
	const std::string floatToString(float x, int decDigits);
	const SDL_Color JsonToColor(Json::Value JsonColor);
	float normalizeRadians(float angleInRadians);
	float normalizeDegrees(float angleInDegrees);
	glm::vec2 glNormalizeTextureCoords(glm::vec2 textureCoords, glm::vec2 textureSize);
	glm::vec4 glNormalizeColor(const SDL_Color& color);
	glm::vec4 glNormalizeColor(const glm::uvec4& color);
	const ImVec4 JsonToImVec4Color(Json::Value JsonColor);
	const ImVec4 SDLColorToImVec4(SDL_Color sdlColor);
	b2Vec2& toBox2dPoint(b2Vec2& point);
	b2Vec2& toRenderPoint(b2Vec2& point);
	glm::vec2& toRenderPoint(glm::vec2& point);
	Json::Value getComponentConfig(Json::Value definitionJSON, int);
	float calculateDistance(SDL_FPoint location1, SDL_FPoint location2);
	float calculateDistance(glm::vec2 location1, glm::vec2 location2);
	bool fileExists(const std::string& filename);
	void colorApplyAlpha(SDL_Color& color, int alpha);
	void colorApplyAlpha(ImVec4& color, int alpha);

	b2Vec2 matchParentRotation(SDL_FPoint childPosition, SDL_FPoint parentPosition, float);

};


#endif