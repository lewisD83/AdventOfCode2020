// BinaryBoarding.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <assert.h> 

int round_alt(double x)
{
	double ipart;

	// break into integer and fraction parts
	double fpart = modf(x, &ipart);
	if (fpart != 0.0) 
	{
		if (x >= 0.5) 
		{
			ipart += floor(fpart + 0.5);
		}
		else if (x <= -0.5) 
		{
			ipart += ceil(fpart - 0.5);
		}
	}

	return (int)ipart;
}

std::vector<int> boardingPasses(1023);

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

	std::string highestBoardingPass;
	std::string previousBoardingPass;

	int highestSeatID = 0;
	int previousSeatID = 0;

	// Part 1
	// Read stream line by line
	for (std::string line; std::getline(source, line);)
	{
		int upperRowLimit = 127;
		int lowerRowLimit = 0;

		int upperColumnLimit = 7;
		int lowerColumnLimit = 0;

		// Make a stream for the line itself
		std::istringstream in(line);

		// Read in the boarding pass
		std::string boardingPass;
		in >> boardingPass;

		bool needCheckColumns = true;
		bool newLeader = false;

		// Is this the first boarding pass we are reading, it is immediately the new leader
		if (highestBoardingPass.empty())
		{
			highestBoardingPass = boardingPass;
			newLeader = true;
		}
		else
		{
			// Attempt to find the new leader which will lead to the new highest seat ID
			// Check first 7 characters basically seeing which row is closest to BBBBBBB
			for (int i = 0; i < 7; i++)
			{
				if ((int)highestBoardingPass[i] == (int)boardingPass[i])
				{
					continue;
				}

				if ((int)boardingPass[i] < (int)highestBoardingPass[i])
				{
					previousBoardingPass = highestBoardingPass;
					highestBoardingPass = boardingPass;
					newLeader = true;
				}

				needCheckColumns = false;
				break;
			}

			if (needCheckColumns)
			{
				for (int i = 7; i < 10; i++)
				{
					if ((int)highestBoardingPass[i] == (int)boardingPass[i])
					{
						continue;
					}

					if ((int)boardingPass[i] > (int)highestBoardingPass[i])
					{
						previousBoardingPass = highestBoardingPass;
						highestBoardingPass = boardingPass;
						newLeader = true;
					}

					break;
				}
			}
		}

		if (newLeader)
		{
			// Check rows
			//printf("%d through %d \n", lowerRowLimit, upperRowLimit);
			for (int i = 0; i < 7; i++)
			{
				// Take lower half
				if (highestBoardingPass[i] == 'F')
				{
					upperRowLimit -= round_alt( (float)(upperRowLimit - lowerRowLimit) / 2);

				}

				// Take upper half
				if (highestBoardingPass[i] == 'B')
				{
					lowerRowLimit += round_alt( (float)(upperRowLimit - lowerRowLimit) / 2);
				}

				//printf("%d through %d \n", lowerRowLimit, upperRowLimit);
			}

			// Check colums
			//printf("%d through %d \n", lowerColumnLimit, upperColumnLimit);
			for (int i = 7; i < 10; i++)
			{
				// Take lower half
				if (highestBoardingPass[i] == 'L')
				{
					upperColumnLimit -= round_alt((float)(upperColumnLimit - lowerColumnLimit) / 2);

				}

				// Take upper half
				if (highestBoardingPass[i] == 'R')
				{
					lowerColumnLimit += round_alt((float)(upperColumnLimit - lowerColumnLimit) / 2);
				}

				//printf("%d through %d \n", lowerColumnLimit, upperColumnLimit);
			}

			//printf("lowerRowLimit: %d \n", lowerRowLimit);
			//printf("upperRowLimit: %d \n", upperRowLimit);
			//printf("lowerColumnLimit: %d \n", lowerColumnLimit);
			//printf("upperColumnLimit: %d \n", upperColumnLimit);

			assert(lowerRowLimit == upperRowLimit);
			assert(lowerColumnLimit == upperColumnLimit);

			previousSeatID = highestSeatID;
			highestSeatID = (lowerRowLimit * 8) + lowerColumnLimit;

			printf("New Leader with pass: %s and Unique Seat ID: %d \n", highestBoardingPass.c_str(), highestSeatID);
			//printf("Old Leader with pass: %s and Unique Seat ID: %d \n\n", previousBoardingPass.c_str(), previousSeatID);

		}
	}

	highestSeatID = 0;
	previousSeatID = 0;

	source.clear();
	source.seekg(0);

	// Part 2
	// Read stream line by line
	for (std::string line; std::getline(source, line);)
	{
		int upperRowLimit = 127;
		int lowerRowLimit = 0;

		int upperColumnLimit = 7;
		int lowerColumnLimit = 0;

		// Make a stream for the line itself
		std::istringstream in(line);

		// Read in the boarding pass
		std::string boardingPass;
		in >> boardingPass;

		// Check rows
		//printf("%d through %d \n", lowerRowLimit, upperRowLimit);
		for (int i = 0; i < 7; i++)
		{
			// Take lower half
			if (boardingPass[i] == 'F')
			{
				upperRowLimit -= round_alt((float)(upperRowLimit - lowerRowLimit) / 2);

			}

			// Take upper half
			if (boardingPass[i] == 'B')
			{
				lowerRowLimit += round_alt((float)(upperRowLimit - lowerRowLimit) / 2);
			}

			//printf("%d through %d \n", lowerRowLimit, upperRowLimit);
		}

		// Check colums
		//printf("%d through %d \n", lowerColumnLimit, upperColumnLimit);
		for (int i = 7; i < 10; i++)
		{
			// Take lower half
			if (boardingPass[i] == 'L')
			{
				upperColumnLimit -= round_alt((float)(upperColumnLimit - lowerColumnLimit) / 2);

			}

			// Take upper half
			if (boardingPass[i] == 'R')
			{
				lowerColumnLimit += round_alt((float)(upperColumnLimit - lowerColumnLimit) / 2);
			}

			//printf("%d through %d \n", lowerColumnLimit, upperColumnLimit);
		}

		//printf("lowerRowLimit: %d \n", lowerRowLimit);
		//printf("upperRowLimit: %d \n", upperRowLimit);
		//printf("lowerColumnLimit: %d \n", lowerColumnLimit);
		//printf("upperColumnLimit: %d \n", upperColumnLimit);

		assert(lowerRowLimit == upperRowLimit);
		assert(lowerColumnLimit == upperColumnLimit);

		previousSeatID = highestSeatID;
		highestSeatID = (lowerRowLimit * 8) + lowerColumnLimit;

		printf("New Leader with pass: %s and Unique Seat ID: %d \n", boardingPass.c_str(), highestSeatID);
		//printf("Old Leader with pass: %s and Unique Seat ID: %d \n\n", previousBoardingPass.c_str(), previousSeatID);

		boardingPasses[highestSeatID] = highestSeatID;
	}

	for (int i = 0; i < 1023; i++)
	{
		if (boardingPasses[i] == 0)
		{
			if (i < 1023 && i > 0 && boardingPasses[i - 1] != 0 && boardingPasses[i + 1] != 0)
			{
				printf("Missing pass %d \n", i);
			}
		}

	}






	printf("Highest Seat ID: %d", highestSeatID);

}
