#include "pch.h"
#include "SFML_GameWorld.h"

SFML_GameWorld::SFML_GameWorld(int windowWidth, int windowHeight, sf::RenderWindow* parentWindow)
	: m_windowWidth(windowWidth)
	, m_windowHeight(windowHeight)
	, m_camera(windowWidth,windowHeight)
{
	//Create a game world layer object using the wall.png texture
	SFML_GameWorldLayer* background = new SFML_GameWorldLayer("Media/Textures/dirt.png", 1.0f, windowWidth, windowHeight);
	SFML_GameWorldLayer* fog = new SFML_GameWorldLayer("Media/Textures/fog.png", 0.8f, windowWidth, windowHeight);
	fog->setScrollSpeed(sf::Vector2f(100, 100));
	SFML_GameWorldLayer* foreground = new SFML_GameWorldLayer("Media/Textures/Wire.png", 0.3f, windowWidth, windowHeight);

	//Insert it to the array
	m_gameWorldLayerList.push_back(background);
	m_gameWorldLayerList.push_back(foreground);
	m_gameWorldLayerList.push_back(fog);

	//Sort the game world in descending orderS
	sort(m_gameWorldLayerList.begin(), m_gameWorldLayerList.end(), layerSortFunction);

	//Set the camera's initial position to (0,0)
	m_cameraPosition = sf::Vector2f(0, 0);

	//Store the address of the parent render window in m_parentWindow
	m_parentWindow = parentWindow;
	
	//Set the number of zombies
	m_numberofZombies = 10;

	//Initialise the NPC's in a loop
	for (int i = 0; i < m_numberofZombies; i++)
	{
		SFML_NPCSpriteObject* npcSprite = new SFML_NPCSpriteObject();
		npcSprite->setIdleAnimation("Media/Textures/zombie-idle.png", "Media/SpriteInfo/zombie-idle.txt", 1.0f);
		npcSprite->setWalkingAnimation("Media/Textures/zombie-move.png", "Media/SpriteInfo/zombie-move.txt", 1.0f);
		npcSprite->setAttackingAnimation("Media/Textures/zombie-attack.png", "Media/SpriteInfo/zombie-attack.txt", 1.0f);

		npcSprite->setPosition(rand() % 4000 - 2000, rand() % 4000 - 2000);
		npcSprite->setTargetLocation(sf::Vector2f(rand() % 4000 - 2000, rand() % 4000 - 2000));
		m_zombieCharacterList.push_back(npcSprite);
	}

	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		m_zombieCharacterList[counter]->setAlive(true);
		m_zombieCharacterList[counter]->setHealth(3);
	}

	m_playerCharacter = new SFML_PlayableSpriteObject();
	m_playerCharacter->setFlashlightIdleAnimation("Media/Textures/hero_flashlight_idle.png", "Media/SpriteInfo/hero_flashlight_idle.txt", 1.0f);
	m_playerCharacter->setFlashlightWalkingAnimation("Media/Textures/hero_flashlight_move.png", "Media/SpriteInfo/hero_flashlight_move.txt", 1.0f);
	m_playerCharacter->setFlashlightMeleeAnimation("Media/Textures/hero_flashlight_melee.png", "Media/SpriteInfo/hero_flashlight_melee.txt", 1.0f);

	m_playerCharacter->setKnifeIdleAnimation("Media/Textures/hero_knife_idle.png", "Media/SpriteInfo/hero_knife_idle.txt", 1.0f);
	m_playerCharacter->setKnifeWalkingAnimation("Media/Textures/hero_knife_move.png", "Media/SpriteInfo/hero_knife_move.txt", 1.0f);
	m_playerCharacter->setKnifeMeleeAnimation("Media/Textures/hero_knife_melee.png", "Media/SpriteInfo/hero_knife_melee.txt", 1.0f);

	m_playerCharacter->setRifleIdleAnimation("Media/Textures/hero_rifle_idle.png", "Media/SpriteInfo/hero_rifle_idle.txt", 1.0f);
	m_playerCharacter->setRifleWalkingAnimation("Media/Textures/hero_rifle_move.png", "Media/SpriteInfo/hero_rifle_move.txt", 1.0f);
	m_playerCharacter->setRifleMeleeAnimation("Media/Textures/hero_rifle_melee.png", "Media/SpriteInfo/hero_rifle_melee.txt", 1.0f);
	m_playerCharacter->setRifleShootAnimation("Media/Textures/hero_rifle_shoot.png", "Media/SpriteInfo/hero_rifle_shoot.txt", 1.0f);


	m_playerCharacter->setPistolIdleAnimation("Media/Textures/hero_pistol_idle.png", "Media/SpriteInfo/hero_pistol_idle.txt", 1.0f);
	m_playerCharacter->setPistolWalkingAnimation("Media/Textures/hero_pistol_move.png", "Media/SpriteInfo/hero_pistol_move.txt", 1.0f);
	m_playerCharacter->setPistolMeleeAnimation("Media/Textures/hero_pistol_melee.png", "Media/SpriteInfo/hero_pistol_melee.txt", 1.0f);
	m_playerCharacter->setPistolShootAnimation("Media/Textures/hero_pistol_shoot.png", "Media/SpriteInfo/hero_pistol_shoot.txt", 1.0f);
	   

	m_playerCharacter->equipFlashlight();
	m_playerCharacter->toIdleState();

	//Set player position
	m_playerCharacter->setPosition(0, 0);

}

SFML_GameWorld::~SFML_GameWorld()
{
	//Clear the GameWorldLayer list
	for (vector<SFML_GameWorldLayer*>::iterator it = m_gameWorldLayerList.begin(); it != m_gameWorldLayerList.end(); it++)
	{
		delete(*it);
	}
	m_gameWorldLayerList.clear();

	//Clear the memory space used for the animated object
	delete m_animatedObject;
}

void SFML_GameWorld::update(float elapsedTime)
{
	processEvents(elapsedTime);
	m_playerCharacter->setColour(sf::Color(255, 255, 255));
	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		m_zombieCharacterList[counter]->setColour(sf::Color(255, 255, 255));
	}
	
	//Set first layer in the array to follow the camera
	for (size_t counter = 0; counter < m_gameWorldLayerList.size(); counter++)
	{
		m_gameWorldLayerList[counter]->followCamera(&m_camera);
		m_gameWorldLayerList[counter]->update(elapsedTime);
	}

	//Update playable character and set camera to follow
	m_playerCharacter->update(elapsedTime);
	m_camera.setPosition(m_playerCharacter->getWorldPosition());

	//Update each zombie object in the list
	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		m_zombieCharacterList[counter]->update(elapsedTime);

		if (!m_zombieCharacterList[counter]->getAlive())
		{
			m_zombieCharacterList.erase(m_zombieCharacterList.begin() + counter);
		}
	}

	//Update all bullets
	for (unsigned int counter = 0; counter < m_bulletList.size(); counter++)
	{
		m_bulletList[counter]->update(elapsedTime);
	}

	//Bullet Collision
	for (unsigned int zombie = 0; zombie < m_zombieCharacterList.size(); zombie++)
	{
		sf::FloatRect boundingBox = m_zombieCharacterList[zombie]->getGlobalBounds();
		for (unsigned int counter = 0; counter < m_bulletList.size(); counter++)
		{
			sf::Vector2f point = m_bulletList[counter]->getWorldPosition();

			if (boundingBox.contains(point))
			{
				m_zombieCharacterList[zombie]->setColour(sf::Color(128, 0, 0));
				m_zombieHealth = m_zombieCharacterList[zombie]->getHealth();
				m_zombieHealth = m_zombieHealth - 1;
				m_zombieCharacterList[zombie]->setHealth(m_zombieHealth);
				m_bulletList[counter]->isActive(false);
			}
		}
	}

	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		if (m_zombieCharacterList[counter]->getHealth() < 0)
		{
			m_zombieCharacterList[counter]->setAlive(false);
		}
	}

	//Player Melee Attack Collision
	for (unsigned int melee = 0; melee < m_zombieCharacterList.size(); melee++)
	{
		sf::Vector2f point = m_zombieCharacterList[melee]->getWorldPosition();

		sf::FloatRect boundingBox = m_playerCharacter->getGlobalBounds();

		if (m_playerCharacter->getIsMeleeAttacking())
		{
			if (boundingBox.contains(point))
			{
				m_playerCharacter->setIsMeleeAttacking();
				m_zombieCharacterList[melee]->setColour(sf::Color(128, 0, 0));
				m_zombieCharacterList[melee]->setHealth(m_zombieCharacterList[melee]->getHealth() - 1);
			}
		}	
	}

	//Zombie Attack Collision
	for (unsigned int attack = 0; attack < m_zombieCharacterList.size(); attack++)
	{
		sf::FloatRect boundingBox = m_zombieCharacterList[attack]->getGlobalBounds();

		sf::Vector2f point = m_playerCharacter->getWorldPosition();

		if (boundingBox.contains(point))
		{
			m_playerCharacter->setColour(sf::Color(128, 0, 0));
		}
	}
}

void SFML_GameWorld::processEvents(float elapsedTime)
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
	{
		m_camera.zoomUp(elapsedTime);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
	{
		m_camera.zoomDown(elapsedTime);
	}

	//Get camera transform (world -> screen)
	sf::Transform transform;
	transform = m_camera.getProjTransform() * m_camera.getViewTransform();

	//Take inverse (screen -> world)
	sf::Transform inv = transform.getInverse();

	//Get mouse position (in screen space)
	sf::Vector2f mouseScreenPosition = sf::Vector2f(sf::Mouse::getPosition(*m_parentWindow));

	//Convert world space coordinate
	sf::Vector2f wp = inv.transformPoint(mouseScreenPosition);

	//Get sprite global bounding rectangle
	sf::FloatRect rect = m_spriteObject.getGlobalBounds();

	//Update each zombie objects vision cone
	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		if (m_zombieCharacterList[counter]->isInVision(m_playerCharacter->getWorldPosition()))
		{
			//The player character is within the npc's vision cone
			m_zombieCharacterList[counter]->setTargetLocation(m_playerCharacter->getWorldPosition());
			m_zombieCharacterList[counter]->toAttackingState();
		}
		else
		{
			//The player character is out of vision cone, return to walking state
			m_zombieCharacterList[counter]->toWalkingState();
		}
	}

	SFML_BulletSpriteObject* bullet = m_playerCharacter->processEvents(elapsedTime, transform, mouseScreenPosition);
	if (bullet)
		m_bulletList.push_back(bullet);

	bool currentNum1Pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1);
	bool currentNum2Pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Num2);
	bool currentNum3Pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Num3);
	bool currentNum4Pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Num4);

	if (currentNum1Pressed)
	{
		m_playerCharacter->equipFlashlight();
		m_playerCharacter->toIdleState();
	}
	else if (currentNum2Pressed)
	{
		m_playerCharacter->equipKnife();
		m_playerCharacter->toIdleState();
	}
	else if (currentNum3Pressed)
	{
		m_playerCharacter->equipRifle();
		m_playerCharacter->toIdleState();
	}
	else if (currentNum4Pressed)
	{
		m_playerCharacter->equipPistol();
		m_playerCharacter->toIdleState();
	}
}

void SFML_GameWorld::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	//Render each game world layer with parallax factor >= 1.0
	for (unsigned int counter = 0; counter < m_gameWorldLayerList.size(); counter++)
	{
		if (m_gameWorldLayerList[counter]->getparallaxFactor() >= 1.0f)
			target.draw(*m_gameWorldLayerList[counter]);
	}

	//Get the camera view-projection transform and set it to the render state
	sf::RenderStates renderState;
	renderState.transform = m_camera.getProjTransform() * m_camera.getViewTransform();

	//Draw each zombie in the list
	for (unsigned int counter = 0; counter < m_zombieCharacterList.size(); counter++)
	{
		if (m_zombieCharacterList[counter]->getAlive() == true)
		{
			target.draw(*m_zombieCharacterList[counter], renderState);
		}
	}

	//Draw each bullet object in the list
	for (unsigned int counter = 0; counter < m_bulletList.size(); counter++)
	{
		if (m_bulletList[counter]->getActive() == true)
		{
			target.draw(*m_bulletList[counter], renderState);
		}
	}

	//Draw the player character
	target.draw(*m_playerCharacter, renderState);

	//Render each game world layer with parallax factor < 1.0
	for (unsigned int counter = 0; counter < m_gameWorldLayerList.size(); counter++)
	{
		if (m_gameWorldLayerList[counter]->getparallaxFactor() < 1.0f)
			target.draw(*m_gameWorldLayerList[counter]);
	}
}

bool SFML_GameWorld::layerSortFunction(SFML_GameWorldLayer * a, SFML_GameWorldLayer * b)
{
	return (a->getparallaxFactor() > b->getparallaxFactor());
}
