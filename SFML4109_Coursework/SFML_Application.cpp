#include "pch.h"
#include "SFML_Application.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

SFML_Application::SFML_Application(int windowWidth, int windowHeight)
	: m_window(sf::VideoMode(windowWidth, windowHeight), "SFML Application", sf::Style::Fullscreen)
	, m_initialised(true)
	, m_screenWidth(windowWidth)
	, m_screenHeight(windowHeight)
	, m_appState(TITLESCREEN)
	, m_previousDownKeyPressed(false)
	, m_previousUpKeyPressed(false)
	, m_PreviousEscapeKeyPressed(false)
	, m_PreviousTabKeyPressed(false)
	, m_currentTabKeyPressed(false)
	, m_titleScreen("Media/Textures/title-screen.png", windowWidth, windowHeight)
	, m_mainMenu("Media/MenuInfo/main-menu.txt",windowWidth,windowHeight)
	, m_pauseMenu("Media/MenuInfo/pause-menu.txt",windowWidth,windowHeight)
	, m_gameWorld(windowWidth,windowHeight, &m_window)
	, m_desiredUpdateTime(sf::seconds(1.f/60.f))
	, m_timeCounter(sf::seconds(0))
	, m_FPSCounter(0)
	, displayFPS(true)
{
	//Load fonts to be used for on-screen messages
	m_initialised = m_textFont.loadFromFile("Media/Fonts/SegoeMarker.ttf");

	//Set the message text object to use the font we just loaded
	m_messageText.setFont(m_textFont);

	//Set Scale Of The Menu to 85% and its position to the center of the screen
	m_mainMenu.setScale(0.85f, 0.85f);
	m_mainMenu.setPosition(m_screenWidth / 2, m_screenHeight / 2);

	//Set Scale Of The Menu to 85% and its position to the center of the screen
	m_pauseMenu.setScale(0.85f, 0.85f);
	m_pauseMenu.setPosition(m_screenWidth / 2, m_screenHeight / 2);
}

SFML_Application::~SFML_Application()
{
}

void SFML_Application::run()
{
	if (!m_initialised)
		return;

	sf::Clock clock;

	//Variable to track the time since update() was last executed
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (m_window.isOpen())
	{
		//Get the elapsed time since the last loop
		sf::Time elapsedTime = clock.restart();

		//Add to timeSinceLastUpdate
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate > m_desiredUpdateTime)
		{
			//Update application states
			update(m_desiredUpdateTime);

			//Subtract m_desiredUpdateTime from timeSinceLastUpdate
			timeSinceLastUpdate -= m_desiredUpdateTime;
		}
		
		//Calculate FPS
		updateFPS(elapsedTime);

		//Render result to screen
		render();
	}
}

void SFML_Application::update(sf::Time elapsedTime)
{
	//Check if the player has tried to close the window, if so close the application
	//This m_event triggers when 1) The x window button is pressed or 2) ALT-F4 keys are pressed
	bool validEventPoll = m_window.pollEvent(m_event);

	if (m_event.type == sf::Event::Closed && validEventPoll)
		m_window.close();

	bool currentDownKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	bool currentUpKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	bool currentEscapeKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
	int selection;
	
	m_PreviousTabKeyPressed = m_currentTabKeyPressed;
	m_currentTabKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Tab);

	if (m_currentTabKeyPressed && !m_PreviousTabKeyPressed)
	{
		displayFPS = !displayFPS;
	}

	//Prepare the message text and its position
	switch (m_appState)
	{
	case TITLESCREEN:
		if (m_event.type == sf::Event::KeyPressed)
			m_appState = MAINMENU;

		m_titleScreen.update(elapsedTime);

		break;
	case MAINMENU:
		selection = m_mainMenu.update(elapsedTime);
		if (selection >= 0)
		{
			if (selection == 0)
				m_appState = GAMEPLAYING;
			else if (selection == 1)
				m_appState = OPTIONMENU;
			else if (selection == 2)
				m_appState = EXITING;
		}
		break;
	case PAUSE:
		selection = m_pauseMenu.update(elapsedTime);
		if (selection >= 0)
		{
			if (selection == 0)
				m_appState = GAMEPLAYING;
			else if (selection == 1)
				m_appState = OPTIONMENU;
			else if (selection == 2)
				m_appState = EXITING;
		}
		break;
	case OPTIONMENU:
		if (currentEscapeKeyPressed && !m_PreviousEscapeKeyPressed)
			m_appState = MAINMENU;
		
		m_messageText.setString("Option Menu Screen");

		break;
		m_PreviousEscapeKeyPressed = currentEscapeKeyPressed;
	case GAMEPLAYING:
		if (currentEscapeKeyPressed && !m_PreviousEscapeKeyPressed)
			m_appState = PAUSE;

		m_gameWorld.update(elapsedTime.asSeconds());

		break;
	case EXITING:
		m_window.close();
		break;
	}
}

void SFML_Application::render()
{
	m_window.clear();

	

	switch (m_appState)
	{
	case TITLESCREEN:
	{
		m_window.draw(m_titleScreen);
	}
	break;
	case MAINMENU:
	{
		m_window.draw(m_mainMenu);
	}
	break;
	case PAUSE:
	{
		m_window.draw(m_pauseMenu);
	}
	break;
	case OPTIONMENU:
	{
		m_window.draw(m_messageText);
	}
	break;
	case GAMEPLAYING:
	{
		m_window.draw(m_gameWorld);

		if (displayFPS)
			m_window.draw(m_messageText);
	}
	break;
	case EXITING:
	{

	}
	break;
	}
	
	m_window.display();
}

void SFML_Application::updateFPS(sf::Time elapsedTime)
{
	//Increment the time counter by the time since the last render
	m_timeCounter += elapsedTime;

	//Increment FPS counter by one
	m_FPSCounter += 1;

	//Limits the FPS to 60
	m_window.setFramerateLimit(60);

	//If one second has passed
	if (m_timeCounter >= sf::seconds(1.0f))
	{
		//Set the message text
		m_messageText.setString(to_string(m_FPSCounter) + " fps \n"); //+ "Time between screen refreshes = " + to_string(m_timeCounter.asMicroseconds() / m_FPSCounter) + " microseconds \n");

		//Take away one second from the time counter
		m_timeCounter -= sf::seconds(1.0f);

		//Reset the FPS counter
		m_FPSCounter = 0;
	}
}
