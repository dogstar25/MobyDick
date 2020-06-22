#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>
#include <map>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <Box2D/Box2D.h>
#include "texture.h"

//#include "GameObject.h" //added
//#include "CompositeObject.h" //added



class TextureManager
{

public:

	/*
	This is a singleton class using a local staic variable returned as a reference
	*/
	static TextureManager& instance();
	bool init(SDL_Window*);
	void render(SDL_Texture* texture, SDL_Color color, SDL_Rect* srcRect, SDL_Rect* destRect, float angle);
	void render(SDL_Rect* destRect, SDL_Color color);
	bool present();
	bool clear();
	std::shared_ptr<Texture> getTexture(std::string id);
	std::string getFont(std::string id);
	void drawPoly(b2Body* body);
	void drawPoints(SDL_Point*);
	void drawLine(b2Vec2, b2Vec2);
	void drawGlowLine(b2Vec2, b2Vec2, SDL_Color);
	void drawGlowLine2(b2Vec2, b2Vec2, SDL_Color);
	void outlineObject(std::vector<SDL_Point> points, float lineSize);
	SDL_Texture* createTextureFromSurface(SDL_Surface* surface);
	bool hasTexture(std::string textureId);
	void addOrReplaceTexture(std::string textureId, std::shared_ptr<Texture> texture);

private:
	TextureManager();
	~TextureManager();

	SDL_Renderer* m_Renderer;
	std::map<std::string, std::shared_ptr<Texture>> m_textureMap;
	std::map<std::string, std::string> m_fontMap;
	bool loadTextures();


	

};

#endif