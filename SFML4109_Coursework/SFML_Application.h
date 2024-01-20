#pragma once
#include "pch.h"
#include <SFML/Graphics.hpp>
#include "SFML_TitleScreen.h"
#include "SFML_GameMenu.h"
#include "SFML_GameWorld.h"
#include "SFML_PauseMenu.h"
using namespace std;

class SFML_Application : public sf::NonCopyable
{
public:
	SFML_Application(int windowWidth, int windowHeight);
	~SFML_Application();

	void run();

private:
	void update(sf::Time elapsedTime);
	void render();

	//True if the application had been successfully initialized
	bool m_initialised;
	bool m_previousDownKeyPressed;
	bool m_previousUpKeyPressed;
	bool m_PreviousEscapeKeyPressed;
	bool m_PreviousTabKeyPressed;
	bool m_currentTabKeyPressed;
	bool displayFPS;

	//Member attributes for graphics/window
	sf::RenderWindow m_window;
	size_t m_screenWidth;
	size_t m_screenHeight;
	sf::Font m_textFont;
	sf::Text m_messageText;

	//Member attribute for capturing events
	sf::Event m_event;

	//Application State Enumeration
	enum AppState
	{
		TITLESCREEN,
		MAINMENU,
		GAMEPLAYING,
		PAUSE,
		EXITING,
		OPTIONMENU,
	};

	//Member attribute for AppState
	AppState m_appState;

	//Member attribute for SFML_Titlescreen object
	SFML_TitleScreen m_titleScreen;

	//Member attribute for SFML_Mainmenu object
	SFML_GameMenu m_mainMenu;

	//Member attribute for SFML_Pausemenu object
	SFML_PauseMenu m_pauseMenu;

	//Member attributes for FPS data
	sf::Time m_timeCounter;
	sf::Time m_desiredUpdateTime;
	size_t m_FPSCounter;

	//Function to update FPS data
	void updateFPS(sf::Time elapsedTime);

	//Game world object that contains m_gameWorldLayerList and m_Camera
	SFML_GameWorld m_gameWorld;
};

