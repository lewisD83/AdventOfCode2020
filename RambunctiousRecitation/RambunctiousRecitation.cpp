// RambunctiousRecitation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

	// Big enough to hold all turn numbers
	int numbersSize = (int)pow(2, 25);
	std::vector<int> spokenNumbers;
	spokenNumbers.resize(numbersSize, 0);

	// Get the turn we should stop on
	int turnToStop = std::stoi(argv[2]);

	// Read in the states of the seats in the waiting room
	for (std::string line; std::getline(source, line);)
	{
		int lastSpokenNumber = 0;
		int turnNumber = 0;

		// Make a stream for the line itself
		std::istringstream in(line);

		// Read in the starting input
		printf("Input = ");
		int input;
		while (in >> input)
		{
			turnNumber++;
			lastSpokenNumber = input;
			spokenNumbers[input] = turnNumber;
			printf("%d, ", input);
		}

		printf("\n");
		spokenNumbers[input] = 0;

		while (turnNumber != turnToStop)
		{
			// Get the turn this number was last spoken 
			int turnLastSpoken = spokenNumbers[lastSpokenNumber];

			// Before we are about to speak a new number log the last one
			spokenNumbers[lastSpokenNumber] = turnNumber;

			// If that was the first time the number has been spoken, the current player says 0.
			if (turnLastSpoken == 0)
			{
				lastSpokenNumber = 0;
			}
			// Otherwise, the number had been spoken before; the current player announces how many turns apart the number is from when it was previously spoken.
			else
			{
				lastSpokenNumber = turnNumber - turnLastSpoken;
			}

			turnNumber++;
		}

		printf("Turn %d: %d\n\n", turnNumber, lastSpokenNumber);
		spokenNumbers.clear();
		spokenNumbers.resize(numbersSize);
	}
}