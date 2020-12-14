#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

// Global length and breadth of the waiting room
int waitingRoomLength = 0;
int waitingRoomBreadth = 0;

// Checks to see if a seat in the specified direction is occupied
bool IsOccupiedInDireciton(const std::vector<std::string>& seats, int row, int col, int rowDelta, int colDelta, bool checkAllInDirection)
{
	// Figure out the current row and column we are on
	int currentRow = row + rowDelta;
	int currentCol = col + colDelta;

	// Check every seat in the specified direction
	if (checkAllInDirection)
	{
		while (true)
		{
			// If we have the reached the bounds bail out
			if (currentRow == -1 || currentRow == waitingRoomBreadth)
				break;


			// If we have the reached the bounds bail out
			if (currentCol == -1 || currentCol == waitingRoomLength)
				break;

			// If we have reached an occupied seat return true
			if (seats[currentRow][currentCol] == '#')
				return true;

			// If we reach an un-occupied seat we return false
			if (seats[currentRow][currentCol] == 'L')
				return false;

			// If we haven't found a seat yet then keep going in the same direction
			currentRow += rowDelta;
			currentCol += colDelta;
		}
	}
	else
	{
		// If we have the reached the bounds bail out
		if (currentRow == -1 || currentRow == waitingRoomBreadth)
			return false;


		// If we have the reached the bounds bail out
		if (currentCol == -1 || currentCol  == waitingRoomLength)
			return false;


		// If we have reached an occupied seat return true
		if (seats[currentRow][currentCol] == '#')
			return true;
	}

	return false;
}

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
	sf::RenderWindow window(sf::VideoMode(1900, 1000), "Seating System");
	window.setFramerateLimit(60);


	// Keep track of the various states of the seating plan
	std::vector<std::string> seatingPlan;
	std::vector<std::string> prevSeatingPlan;
	std::vector<std::string> emptySeatingPlan;

	// Read in the states of the seats in the waiting room
	for (std::string line; std::getline(source, line);)
	{
		waitingRoomLength = line.length();
		seatingPlan.push_back(line);
	}

	waitingRoomBreadth = seatingPlan.size();

	bool anyChangesMade = true;
	int numOccupiedSeats = 0;
	prevSeatingPlan = seatingPlan;
	emptySeatingPlan = seatingPlan;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		// Part 2
		if(anyChangesMade == true)
		{
			anyChangesMade = false;
			numOccupiedSeats = 0;

			for (int row = 0; row < waitingRoomBreadth; row++)
			{
				for (int col = 0; col < waitingRoomLength; col++)
				{
					int numAdjacent = 0;
					bool isCurrentSeatOccupied = (prevSeatingPlan[row][col] == '#') && (prevSeatingPlan[row][col] != '.') ? true : false;
					bool isFloor = prevSeatingPlan[row][col] == '.' ? true : false;

					if (isFloor == false)
					{
						// Check up-left
						numAdjacent += IsOccupiedInDireciton(prevSeatingPlan, row, col, -1, -1, true) ? 1 : 0;

						// Check up
						numAdjacent += IsOccupiedInDireciton(prevSeatingPlan, row, col, 0, -1, true) ? 1 : 0;

						// Check up-right
						numAdjacent += IsOccupiedInDireciton(prevSeatingPlan, row, col, 1, -1, true) ? 1 : 0;

						// Check right
						numAdjacent += IsOccupiedInDireciton(prevSeatingPlan, row, col, 1, 0, true) ? 1 : 0;

						// Check down-right
						numAdjacent += IsOccupiedInDireciton(prevSeatingPlan, row, col, 1, 1, true) ? 1 : 0;

						// Check down
						numAdjacent += IsOccupiedInDireciton(prevSeatingPlan, row, col, 0, 1, true) ? 1 : 0;

						// Check down-left
						numAdjacent += IsOccupiedInDireciton(prevSeatingPlan, row, col, -1, 1, true) ? 1 : 0;

						// Check left
						numAdjacent += IsOccupiedInDireciton(prevSeatingPlan, row, col, -1, 0, true) ? 1 : 0;

						if (isCurrentSeatOccupied == false && numAdjacent == 0)
						{
							seatingPlan[row][col] = '#';
							anyChangesMade = true;
						}

						if (isCurrentSeatOccupied == true && numAdjacent >= 5)
						{
							seatingPlan[row][col] = 'L';
							anyChangesMade = true;
						}

						if (seatingPlan[row][col] == '#')
						{
							sf::RectangleShape chair(sf::Vector2f(8, 8));
							chair.setFillColor(sf::Color(145, 28, 28));
							chair.setPosition(col * 9.f, row * 9.f);
							window.draw(chair);
						}

						if (seatingPlan[row][col] == 'L')
						{
							sf::RectangleShape chair(sf::Vector2f(8, 8));
							chair.setFillColor(sf::Color(102, 102, 102));
							chair.setPosition(col * 9.f, row * 9.f);
							window.draw(chair);
						}
					}

					if (prevSeatingPlan[row][col] == '#')
					{
						numOccupiedSeats++;
					}
				}
			}

			prevSeatingPlan = seatingPlan;
		}
		else
		{
			for (int row = 0; row < waitingRoomBreadth; row++)
			{
				for (int col = 0; col < waitingRoomLength; col++)
				{
					if (seatingPlan[row][col] == '#')
					{
						sf::RectangleShape chair(sf::Vector2f(8, 8));
						chair.setFillColor(sf::Color(145, 28, 28));
						chair.setPosition(col * 9.f, row * 9.f);
						window.draw(chair);
					}

					if (seatingPlan[row][col] == 'L')
					{
						sf::RectangleShape chair(sf::Vector2f(8, 8));
						chair.setFillColor(sf::Color(102, 102, 102));
						chair.setPosition(col * 9.f, row * 9.f);
						window.draw(chair);
					}
				}
			}
		}

		window.display();
	}
}