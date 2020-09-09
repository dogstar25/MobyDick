#ifndef RENDERER_SF_H
#define RENDERER_SF_H

#include <vector>
#include <map>
#include <optional>

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include <box2d/box2d.h>


class Renderer_SF
{
public:
	Renderer_SF();
	~Renderer_SF();

	static Renderer_SF& instance();

	void init();
	bool present();
	bool clear();

	sf::RenderWindow& window() {
		return m_windowSF;
	}

private:

	sf::RenderWindow m_windowSF;




};


#endif