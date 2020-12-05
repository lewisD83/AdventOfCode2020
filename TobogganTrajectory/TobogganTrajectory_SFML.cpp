// TobogganTrajectory.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>


int main(int argc, char *argv[])
{
	// build a read-Stream
	std::ifstream source;

	// Open the sample problem
	source.open(argv[1], std::ios_base::in);

	// Check if there was a problem opening the file
	if (!source)
	{
		std::cout << "Can't open file!";
	}

	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	// Prepare window
	sf::RenderWindow window(sf::VideoMode(1900, 1000), "Toboggan Trajectory");
	window.setFramerateLimit(60);

	// Main game camera
	sf::View gameCam(sf::Vector2f(150.f, 200.f), sf::Vector2f(1900, 1000));

	// Load textures
	sf::Texture snowTexture;
	sf::Texture treeTexture;
	sf::Texture santaTexture;
	snowTexture.loadFromFile("SnowTexture.jpg");
	treeTexture.loadFromFile("TreeTexture.png");
	santaTexture.loadFromFile("SantaTexture.png");

	// Create a graphical treesHitText to display
	sf::Font font;
	if (!font.loadFromFile("DroidSans.ttf"))
		return EXIT_FAILURE;

	// Set the text which will display how many trees we've hit
	sf::Text treesHitText("Trees Hit: ", font, 50);
	treesHitText.setFillColor(sf::Color::Red);

	// This is our Santa sprite 
	sf::Sprite santaSprite;
	santaSprite.setTexture(santaTexture);

	// This is the background behind the TreesHit text so we we can read it better
	sf::RectangleShape textBackGround(sf::Vector2f(300, 70));
	textBackGround.setFillColor(sf::Color::Black);

	// Keep track of how many right and how many down the sled should go
	int right = std::stoi(argv[2]);
	int down = std::stoi(argv[3]);

	// Santas sleigh position
	float santaPositionX = 0;
	float santaPositionY = 0;

	// Position of the text which displays the number of trees hit
	float treesHitPositionX = -350;
	float treesHitPositionY = -200;

	// This is how far the sled travels right before it falls down the slope
	int maxSledXDistance = right * 73;

	// Based on the way the sled moves the sled will drop down after so many in game coords
	int checkPosForward = maxSledXDistance;
	int checkPosBackward = 0;

	int gameCamHalfWidth = (int)gameCam.getSize().x / 2;
	int gameCamHalfHeight = (int)gameCam.getSize().y / 2;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// up, down, left and right controls the game camera to follow the action
		// Also make sure the trees hit text follows the camera, basically becomes a HUD
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			gameCam.move(-4.f, 0.f);
			treesHitPositionX -= 4.0f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			gameCam.move(4.f, 0.f);
			treesHitPositionX += 4.0f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			gameCam.move(0.f, -4.f);
			treesHitPositionY -= 4.0f;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			gameCam.move(0.f, 4.f);
			treesHitPositionY += 4.0f;
		}

		// Move santa down the slope, falling down at the correct moments
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			santaPositionX += 4.0f;
			treesHitPositionX += 4.0f;
			treesHitPositionY += 1.0f;
			gameCam.move(4.0f, 1.0f);

			if ((int)santaPositionX > checkPosForward)
			{
				santaPositionX = checkPosForward;
				santaPositionY += 73;
				checkPosForward += maxSledXDistance;
				checkPosBackward += maxSledXDistance;
			}
		}

		// Reverse santa to go back up the hill 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			santaPositionX -= 4.0f;
			treesHitPositionX -= 4.0f;
			treesHitPositionY -= 1.0f;
			gameCam.move(-4.0f, -1.0f);

			if ((int)santaPositionX < checkPosBackward)
			{
				santaPositionX = checkPosBackward;
				santaPositionY -= 73;
				checkPosForward -= maxSledXDistance;
				checkPosBackward -= maxSledXDistance;
			}
		}

		window.clear();
		window.setView(gameCam);

		int columnIndex = 0;
		int rowIndex = 0;
		int appendings = 0;
		int defaultRowLength = 0;
		int treesHit = 0;

		float posY = 1;
		float testPosY = 1;

		// Read stream line by line
		for (std::string line; std::getline(source, line);)
		{
			float posX = 0;
			float testPosX = 0;

			// Make a stream for the line itself
			std::istringstream in(line);

			// Read in the current row string
			std::string row;
			in >> row;

			defaultRowLength = row.length();

			// If we have reached the end of the row append a new line and put you back to where you should be
			if (columnIndex >= defaultRowLength)
			{
				columnIndex -= defaultRowLength;
				appendings++;
			}

			// Print out the appended lines
			for (int i = 0; i < appendings; i++)
			{
				for (auto c : row)
				{
					if (c == '.')
					{
						if (posX < gameCam.getCenter().x + gameCamHalfWidth + 73  && posX > gameCam.getCenter().x - 73 - gameCamHalfWidth &&
							posY < gameCam.getCenter().y + gameCamHalfHeight + 73 && posY > gameCam.getCenter().y - 73 - gameCamHalfHeight)
						{
							sf::Sprite snowSprite;
							snowSprite.setTexture(snowTexture);
							snowSprite.setPosition(posX, posY);
							window.draw(snowSprite);
						}
					}

					if (c == '#')
					{
						if (posX < gameCam.getCenter().x + gameCamHalfWidth + 73  && posX > gameCam.getCenter().x - 73 - gameCamHalfWidth &&
							posY < gameCam.getCenter().y + gameCamHalfHeight + 73 && posY > gameCam.getCenter().y - 73 - gameCamHalfHeight)
						{
							sf::Sprite snowSprite;
							snowSprite.setTexture(snowTexture);
							snowSprite.setPosition(posX, posY);

							sf::Sprite treeSprite;
							treeSprite.setTexture(treeTexture);
							treeSprite.setPosition(posX, posY);

							window.draw(snowSprite);
							window.draw(treeSprite);
						}
					}

					posX += 73;
				}
			}

			// Only check every row after the first
			// And make sure the current row is the one the sled should be on
			if (rowIndex > 0 && rowIndex % down == 0)
			{
				// If the one we are on is a tree then increment tree hits and mark it as a tree
				if (row[columnIndex] == '#')
				{
					row[columnIndex] = 'X';
				}
				// Otherwise its a safe space and mark it with a circle
				else
				{
					row[columnIndex] = 'O';

				}
			}

			for (auto c : row)
			{
				if (c == '.' || c == 'O')
				{
					if (posX < gameCam.getCenter().x + gameCamHalfWidth + 73  && posX > gameCam.getCenter().x - 73 - gameCamHalfWidth &&
						posY < gameCam.getCenter().y + gameCamHalfHeight + 73 && posY > gameCam.getCenter().y - 73 - gameCamHalfHeight)
					{
						sf::Sprite snowSprite;
						snowSprite.setTexture(snowTexture);
						snowSprite.setPosition(posX, posY);
						window.draw(snowSprite);
					}
				}

				if (c == '#' || c == 'X')
				{
					if (posX < gameCam.getCenter().x + gameCamHalfWidth + 73  && posX > gameCam.getCenter().x - 73 - gameCamHalfWidth &&
						posY < gameCam.getCenter().y + gameCamHalfHeight + 73 && posY > gameCam.getCenter().y - 73 - gameCamHalfHeight)
					{
						sf::Sprite snowSprite;
						snowSprite.setTexture(snowTexture);
						snowSprite.setPosition(posX, posY);

						sf::Sprite treeSprite;
						treeSprite.setTexture(treeTexture);
						treeSprite.setPosition(posX, posY);

						if (c == 'X' && santaPositionX > posX)
						{
							treeSprite.setColor(sf::Color::Red);
						}

						window.draw(snowSprite);
						window.draw(treeSprite);
					}

					if (c == 'X' && santaPositionX > posX)
					{
						treesHit++;
					}
				}

				posX += 73;
			}

			testPosX = posX;
			testPosY = posY;

			for(int i = 0; i < 5; i++)
			{
				for (auto c : row)
				{
					if (c == '.' || c == 'O')
					{
						if (testPosX < gameCam.getCenter().x + gameCamHalfWidth + 73  && testPosX > gameCam.getCenter().x - 73 - gameCamHalfWidth &&
							testPosY < gameCam.getCenter().y + gameCamHalfHeight + 73 && testPosY > gameCam.getCenter().y - 73 - gameCamHalfHeight)
						{
							sf::Sprite snowSprite;
							snowSprite.setTexture(snowTexture);
							snowSprite.setPosition(testPosX, testPosY);
							window.draw(snowSprite);
						}
					}

					if (c == '#' || c == 'X')
					{
						if (testPosX < gameCam.getCenter().x + gameCamHalfWidth + 73  && testPosX > gameCam.getCenter().x - 73 - gameCamHalfWidth &&
							testPosY < gameCam.getCenter().y + gameCamHalfHeight + 73 && testPosY > gameCam.getCenter().y - 73 - gameCamHalfHeight)
						{
							sf::Sprite snowSprite;
							snowSprite.setTexture(snowTexture);
							snowSprite.setPosition(testPosX, testPosY);

							sf::Sprite treeSprite;
							treeSprite.setTexture(treeTexture);
							treeSprite.setPosition(testPosX, testPosY);

							window.draw(snowSprite);
							window.draw(treeSprite);
						}
					}

					testPosX += 73;
				}
			}

			// Only travel right once we have gone down the correct nummber of rows
			if (rowIndex % down == 0)
			{
				// We then sled across right
				columnIndex += right;
			}

			// We sled down 1
			rowIndex += 1;

			posY += 73;

		}

		source.clear();
		source.seekg(0);

		char buff[100];
		snprintf(buff, sizeof(buff), "Trees Hit: %d", treesHit);

		santaSprite.setPosition(santaPositionX, santaPositionY);
		textBackGround.setPosition(treesHitPositionX, treesHitPositionY);
		treesHitText.setPosition(treesHitPositionX, treesHitPositionY);
		treesHitText.setString(buff);
		window.draw(santaSprite);
		window.draw(textBackGround);
		window.draw(treesHitText);

		window.display();
	}

	int columnIndex = 0;
	int rowIndex = 0;
	int appendings = 0;
	int defaultRowLength = 0;
	int treesHit = 0;

	// Read stream line by line
	for (std::string line; std::getline(source, line);)
	{
		// Make a stream for the line itself
		std::istringstream in(line);

		// Read in the current row string
		std::string row;
		in >> row;

		defaultRowLength = row.length();

		// If we have reached the end of the row append a new line and put you back to where you should be
		if (columnIndex >= defaultRowLength)
		{
			columnIndex -= defaultRowLength;
			appendings++;
		}

		float posX = 0;

		// Print out the appended lines
		for (int i = 0; i < appendings; i++)
		{
			printf("%s", row.c_str());
		}

		// Only check every row after the first
		// And make sure the current row is the one the sled should be on
		if (rowIndex > 0 && rowIndex % down == 0)
		{
			// If the one we are on is a tree then increment tree hits and mark it as a tree
			if (row[columnIndex] == '#')
			{
				treesHit++;
				row[columnIndex] = 'X';
			}
			// Otherwise its a safe space and mark it with a circle
			else
			{
				row[columnIndex] = 'O';

			}
		}

		// Only travel right once we have gone down the correct nummber of rows
		if (rowIndex % down == 0)
		{
			// We then sled across right
			columnIndex += right;
		}

		// We sled down 1
		rowIndex += 1;

		// Print the row we are on
		printf("%s\n", row.c_str());
	}

	source.clear();
	source.seekg(0);

	printf("Trees Hit %d \n", treesHit);




	
}