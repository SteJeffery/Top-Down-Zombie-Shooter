#pragma once
#include <SFML/Graphics.hpp>
#include "SFML_2DCamera.h"
using namespace std;

class SFML_GameWorldLayer : public sf::Drawable
{
public:
	SFML_GameWorldLayer(string textureFileName, float parallaxFactor, int viewportWidth, int viewportHeight);
	~SFML_GameWorldLayer();

	void followCamera(SFML_2DCamera* camera);

	//Member function to set parallax factor after object creation
	void setParallaxFactor(float parallaxFactor);

	//Member function to get the layer's current parallax factor
	float getparallaxFactor();

	//Member function to set the scroll speed
	void setScrollSpeed(sf::Vector2f scrollSpeed);

	void update(float elapsedtime);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//Member attribute to store texture object
	sf::Texture m_layerTexture;

	//Member attribute to store the window vertices
	sf::VertexArray m_layerVertices;

	//Member attribute to store the later's parallax factor
	float m_parallaxFactor;

	//Member attributes to store offset and scroll speed
	sf::Vector2f m_offset;
	sf::Vector2f m_scrollSpeed;

};

