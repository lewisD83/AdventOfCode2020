// TobogganTrajectory.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

	int columnIndex = 0;
	int rowIndex = 0;
	int appendings = 0;
	int defaultRowLength = 0;
	int treesHit = 0;

	// Keep track of how many right and how many down the sled should go
	int right = std::stoi(argv[2]);
	int down = std::stoi(argv[3]);

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


	printf("Trees Hit %d \n", treesHit);
}