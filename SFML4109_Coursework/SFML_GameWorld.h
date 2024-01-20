#pragma once
#include <SFML/Graphics.hpp>
#include "SFML_GameWorldLayer.h"
#include "SFML_SpriteObject.h"
#include "SFML_AnimatedSpriteObject.h"
#include "SFML_NPCSpriteObject.h"
#include "SFML_BulletSpriteObject.h"
#include "SFML_PlayableSpriteObject.h"
using namespace std;

class SFML_GameWorld : public sf::Drawable
{
public:
	SFML_GameWorld(int windowWidth, int windowHeight, sf::RenderWindow* parentWindow);
	~SFML_GameWorld();
	void update(float elapsedTime);

private:
	void processEvents(float elapsedTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//Member attributes to store render window dimension
	int m_windowWidth;
	int m_windowHeight;

	//The array to store the game world layers
	vector<SFML_GameWorldLayer*> m_gameWorldLayerList;

	//Member attribute to store the camera object
	SFML_2DCamera m_camera;

	//Member attribute to store the camera position
	sf::Vector2f m_cameraPosition;

	//Member attribute to store the sprite object
	SFML_SpriteObject m_spriteObject;

	//Member attribute to store pointer to the parent render window
	sf::RenderWindow* m_parentWindow;

	//Static function to be used to sort SFML_GameWorldLayer array
	static bool layerSortFunction(SFML_GameWorldLayer* a, SFML_GameWorldLayer* b);

	SFML_AnimatedSpriteObject* m_animatedObject;
	int m_idleAnimationID;

	vector<SFML_NPCSpriteObject*> m_zombieCharacterList;
	int m_numberofZombies;

	SFML_PlayableSpriteObject* m_playerCharacter;

	vector<SFML_BulletSpriteObject*> m_bulletList;

	int m_zombieHealth;
};

