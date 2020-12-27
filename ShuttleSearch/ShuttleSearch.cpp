// ShuttleSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.


#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

int GetNextValidBus(int currentBus, const std::vector<int>& busNumbers)
{
	while (true)
	{
		currentBus++;
		if (currentBus == busNumbers.size())
		{
			break;
		}

		if (busNumbers[currentBus] != -1)
		{
			break;
		}
	}

	return currentBus;
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

	std::vector<int> busNumbers;

	std::string line;
	std::getline(source, line);
	int earliestDepartureTimeStamp = std::stoi(line);

	std::getline(source, line);
	int delimiter = 0;

	do
	{
		delimiter = (int)line.find(',');
		std::string busNumStr = line.substr(0, delimiter);
		if (busNumStr != "X" && busNumStr != "x")
		{
			int busNum = stoi(busNumStr);
			busNumbers.push_back(busNum);
		}
		else
		{
			busNumbers.push_back(-1);
		}


		line = line.substr(delimiter + 1);
		delimiter = (int)line.find(',');
		if (delimiter == -1 && line != "X" && line != "x")
		{
			int busNum = stoi(line);
			busNumbers.push_back(busNum);
		}

	} while (delimiter != -1);

	// Bus Numbers: 13,x,x,x,x,x,x,37,x,x,x,x,x,461,x,x,x,x,x,x,x,x,x,x,x,x,x,17,x,x,x,x,19,x,x,x,x,x,x,x,x,x,29,x,739,x,x,x,x,x,x,x,x,x,41,x,x,x,x,x,x,x,x,x,x,x,x,23
	/*
	The way i have understood the problem is that i started seeing patterns evolving when solving for just two numbers. I noticed that when two numbers met for the first time they didn't meet until a set amount of intervals later.
	So this code works by starting at the first bus and figuring out where it meets the next bus, then updating how much it increments the departure time as we know that these two buses won't meet again.

	so for the sample: 7,13,x,x,59,x,31,19

	Bus 7 meets 13 for the first time at timestamp 77, these two buses will then always meet every 91 minutes (7x13) so it increments by that now
	Now buses 7 and 13 meet bus 59 at time stamp 350, now all three buses will only meet every 5369 minutes (7x13x59)
	Now buses 7, 13 and 59 meet bus 31 at time stamp 70147, all 4 buses will only ever meet every 166439 (7x13x59x31)
	Finally 7, 13, 59 and 31 meet bus 19 at time stamp 1068788 which is the answer to the sample. If we were to go further and figure out other places they all meet then they would meet every 3162341  minutes (7x13x59x31x19)
	*/

	int firstBus = busNumbers[0];
	uint64_t departureTime = 0;
	uint64_t increment = firstBus;		// Go up in increments of the first bus

	// Get the next bus we will check to see if the departure times overlap
	int busToCheckID = GetNextValidBus(0, busNumbers);

	// keep on looping until we have synced up with each bus
	while (busToCheckID != busNumbers.size())
	{
		// Get the number of the bus we are currently checking for
		int busToCheck = busNumbers[busToCheckID];

		departureTime += increment;

		uint64_t busToCheckDepartureTime = (busToCheck * (departureTime / busToCheck)) + busToCheck;

		// Some buses can fit multiple arrivals inside the firs bus so check multiple times at different intervals.
		// We know to only check a certain number of times before this bus couldn't be within the correct interval
		for(int i = 0; i < busToCheckID; i++)
		{
			// Get the departure time of the bus we are checking when its near our best guess estimate of the departure time that all buses leave around the same time
			bool firstBusArrived = departureTime % firstBus == 0;
			bool busToCheckArrived = busToCheckDepartureTime % busToCheck == 0;

			// If the best guess departure time + the offset of the index of the bus in the vector is the departure time of the current b
			if (firstBusArrived && busToCheckArrived && busToCheckDepartureTime == (departureTime + busToCheckID) )
			{
				// We can now go up in higher increments as we know these two buses only meet at certain intervals now
				increment *= busToCheck;

				// Get the next bus to check
				busToCheckID = GetNextValidBus(busToCheckID, busNumbers);
				break;
			}

			busToCheckDepartureTime += busToCheck;
		}
	}

	printf("Time that all buses meet correctly is: %I64d\n", departureTime);
}
