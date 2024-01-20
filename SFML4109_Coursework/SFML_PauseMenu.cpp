#include "pch.h"
#include "SFML_PauseMenu.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

SFML_PauseMenu::SFML_PauseMenu(string menufilename, int windowWidth, int windowHeight)
{
	ifstream inputFile(menufilename);

	if (inputFile.is_open())
	{
		string stringLine;

		//Read The 1st Line - Background Texture
		getline(inputFile, stringLine);
		if (inputFile.good())
		{
			//Load The Texture File And Set It To The Sprite
			m_backgroundTexture.loadFromFile(stringLine);
			m_backgroundSprite.setTexture(m_backgroundTexture);

			//Set The Origin, Texture Rectangle, And Position Of The Sprite
			sf::Vector2u textureSize = m_backgroundTexture.getSize();
			m_backgroundSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
			m_backgroundSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
			m_backgroundSprite.setPosition(0, 0);
			//Set Origin To The Texture Centre But Placed It To The Top-Left Corner, Will Use The sf::transformable Class property Later On To Shift This To Centre

			//Set The Rectangle Texture Which Is Centered At 0,0
			m_backgroundRect.left = -static_cast<float> (textureSize.x / 2);
			m_backgroundRect.top = -static_cast<float> (textureSize.y / 2);
			m_backgroundRect.width = textureSize.x;
			m_backgroundRect.height = textureSize.y;

			//Read The 2nd Line - Game Logo Texture
			getline(inputFile, stringLine);
			if (inputFile.good())
			{
				//Load The Texture File And Set It To The Sprite
				m_logo.loadFromFile(stringLine);
				m_logoSprite.setTexture(m_logo);

				//Set The Origin, Texture Rectangle, and Position Of The Sprite
				sf::Vector2u textureSize = m_logo.getSize();
				m_logoSprite.setOrigin(textureSize.x / 2, textureSize.y / 2);
				m_logoSprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
				m_logoSprite.setPosition(0, m_backgroundTexture.getSize().y / 2 * 0.9f);
				//We Set The Position Of The Logo 90% Down The Screen Height

				//Read The 3rd Line - Font
				getline(inputFile, stringLine);
				if (inputFile.good())
				{
					m_gamefont.loadFromFile(stringLine);
				}
				string redString;
				string greenString;
				string blueString;

				int redInt;
				int greenInt;
				int blueInt;

				//Read The 4th Line - Normal Text Fill Colour (R,G,B)
				getline(inputFile, stringLine);

				if (inputFile.good())
				{
					//Separate them into three strings (assuming they are separated by a whitespace)
					istringstream stringStream(stringLine);
					stringStream >> redString;
					stringStream >> greenString;
					stringStream >> blueString;

					//Convert Them To Integers
					redInt = stoi(redString);
					greenInt = stoi(greenString);
					blueInt = stoi(blueString);

					//Set the value to m_normalTextColour
					m_normalTextColour = sf::Color(redInt, greenInt, blueInt);
				}

				//Read The 5th Line - Normal Text Outline Colour (R,G,B)
				getline(inputFile, stringLine);
				if (inputFile.good())
				{
					//Separate them into three strings (assuming they are separated by a whitespace)
					istringstream stringStream(stringLine);
					stringStream >> redString;
					stringStream >> greenString;
					stringStream >> blueString;

					//Convert Them To Integers
					redInt = stoi(redString);
					greenInt = stoi(greenString);
					blueInt = stoi(blueString);

					//Set the value to m_normalOutlineColour
					m_normalOutlineColour = sf::Color(redInt, greenInt, blueInt);
				}

				//Read the 6th Line - Highlight Text Fill Colour (R,G,B)
				getline(inputFile, stringLine);
				if (inputFile.good())
				{
					//Separate them into three strings (assuming they are separated by a whitespace)
					istringstream stringStream(stringLine);
					stringStream >> redString;
					stringStream >> greenString;
					stringStream >> blueString;

					//Convert Them To Integers
					redInt = stoi(redString);
					greenInt = stoi(greenString);
					blueInt = stoi(blueString);

					//Set the value to m_hightlightTextColour
					m_highlightTextColour = sf::Color(redInt, greenInt, blueInt);
				}

				//Read the 7th Line - highlighted outline Colour (R,G,B)
				getline(inputFile, stringLine);
				if (inputFile.good())
				{
					//Separate them into three strings (assuming they are separated by a whitespace)
					istringstream stringStream(stringLine);
					stringStream >> redString;
					stringStream >> greenString;
					stringStream >> blueString;

					//Convert Them To Integers
					redInt = stoi(redString);
					greenInt = stoi(greenString);
					blueInt = stoi(blueString);

					//Set the value to m_hightlightOutlineColour
					m_hightlightOutlineColour = sf::Color(redInt, greenInt, blueInt);
				}

				string xBorderString;
				string yBorderString;

				int xBorderInt;
				int yBorderInt;

				//Read The 8th Line - Border Size in Pixels
				getline(inputFile, stringLine);
				if (inputFile.good())
				{
					//Separate them into two strings
					istringstream stringStream(stringLine);
					stringStream >> xBorderString;
					stringStream >> yBorderString;

					//Convert Them tTo Integers
					xBorderInt = stoi(xBorderString);
					yBorderInt = stoi(yBorderString);

					//Set the values to m_menuItemBorder
					m_menuItemBorder.x = xBorderInt;
					m_menuItemBorder.y = yBorderInt;
				}

				//Read the 9th Line- Menu Text Position
				getline(inputFile, stringLine);
				if (inputFile.good())
				{
					if (!strcmp(stringLine.c_str(), "LEFT_JUSTIFIED"))
						m_menuAlignment = LEFT_JUSTIFIED;
					else if (!strcmp(stringLine.c_str(), "CENTRE_JUSTIFIED"))
						m_menuAlignment = CENTRE_JUSTIFIED;
					else if (!strcmp(stringLine.c_str(), "RIGHT_JUSTIFIED"))
						m_menuAlignment = RIGHT_JUSTIFIED;
				}

				sf::Text start;
				start.setFont(m_gamefont);
				start.setCharacterSize(50);
				start.setStyle(sf::Text::Bold);
				start.setFillColor(m_normalTextColour);
				start.setOutlineColor(m_normalOutlineColour);
				start.setOutlineThickness(1.0f);

				sf::Vector2f textpos;

				if (m_menuAlignment == CENTRE_JUSTIFIED)
				{
					textpos.x = m_backgroundRect.left + m_backgroundRect.width / 2;
					textpos.y = m_backgroundRect.top + m_menuItemBorder.y;
				}
				else if (m_menuAlignment == LEFT_JUSTIFIED)
				{
					textpos.x = m_backgroundRect.left + m_menuItemBorder.x;
					textpos.y = m_backgroundRect.top + m_menuItemBorder.y;
				}
				else if (m_menuAlignment == RIGHT_JUSTIFIED)
				{
					textpos.x = m_backgroundRect.left + m_backgroundRect.width - m_menuItemBorder.x;
					textpos.y = m_backgroundRect.top + m_menuItemBorder.y;
				}

				do
				{
					//Read Every Line for each menu item
					getline(inputFile, stringLine);
					if (inputFile.good())
					{
						//Set The Text's Position to textpos
						start.setPosition(textpos.x, textpos.y);

						//Set the text's string to string line (the one we just read)
						start.setString(stringLine);

						//Set the Text's Origin to Mid-Left Coordinate
						sf::FloatRect textbox = start.getLocalBounds();
						if (m_menuAlignment == CENTRE_JUSTIFIED)
						{
							start.setOrigin(textbox.width / 2, textbox.height / 2);
						}
						else if (m_menuAlignment == LEFT_JUSTIFIED)
						{
							start.setOrigin(0, textbox.height / 2);
						}
						else if (m_menuAlignment == RIGHT_JUSTIFIED)
						{
							start.setOrigin(textbox.width, textbox.height / 2);
						}

						//Store the Text to m_menuText Array
						m_menuText.push_back(start);

						//Calculate the next text position by incrementing its y-position
						textpos.y += start.getLocalBounds().height * 1.2f;
					}
					else
						break;
				} while (true);

				m_highlightIndex = 0;
				m_menuText[m_highlightIndex].setFillColor(m_highlightTextColour);
				m_menuText[m_highlightIndex].setOutlineColor(m_hightlightOutlineColour);

				m_prevDownKeyPressed = true;
				m_prevUpKeyPressed = true;
				m_prebReturnKeyPressed = true;
			}
		}

		inputFile.close();
	}
}

SFML_PauseMenu::~SFML_PauseMenu()
{
	//Clear the menu item list
	m_menuText.clear();
}

int SFML_PauseMenu::update(sf::Time elapsedTime)
{
	bool m_currentDownKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	bool m_currentUpKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	bool m_currentReturnKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Return);

	if (m_currentDownKeyPressed && !m_prevDownKeyPressed)
	{
		//Set the colour of the CURRENT highlighted menu item to normal colour
		m_menuText[m_highlightIndex].setFillColor(m_normalTextColour);
		m_menuText[m_highlightIndex].setOutlineColor(m_normalOutlineColour);

		//Increment the hightlighted index by one, modulated by the number of menu items
		m_highlightIndex++;
		m_highlightIndex = m_highlightIndex % m_menuText.size();

		//Set the colour of the NEW highlighted menu item to highlight colour
		m_menuText[m_highlightIndex].setFillColor(m_highlightTextColour);
		m_menuText[m_highlightIndex].setOutlineColor(m_hightlightOutlineColour);
	}
	else if (m_currentUpKeyPressed && !m_prevUpKeyPressed)
	{
		//Set the colour of the CURRENT highlighted menu item to normal colour
		m_menuText[m_highlightIndex].setFillColor(m_normalTextColour);
		m_menuText[m_highlightIndex].setOutlineColor(m_normalOutlineColour);

		//Increment the hightlighted index by one, modulated by the number of menu items
		m_highlightIndex--;
		if (m_highlightIndex < 0)
			m_highlightIndex = m_highlightIndex + m_menuText.size();

		//Set the colour of the NEW highlighted menu item to highlight colour
		m_menuText[m_highlightIndex].setFillColor(m_highlightTextColour);
		m_menuText[m_highlightIndex].setOutlineColor(m_hightlightOutlineColour);
	}
	m_prevDownKeyPressed = m_currentDownKeyPressed;
	m_prevUpKeyPressed = m_currentUpKeyPressed;

	bool hasSelected = false;

	if (m_currentReturnKeyPressed && !m_prebReturnKeyPressed)
		hasSelected = true;

	m_prebReturnKeyPressed = m_currentReturnKeyPressed;

	if (hasSelected)
		return m_highlightIndex;
	else
		return -1;
}

void SFML_PauseMenu::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_backgroundSprite, states);
	target.draw(m_logoSprite, states);

	for (size_t count = 0; count < m_menuText.size(); count++)
		target.draw(m_menuText[count], states);
}
