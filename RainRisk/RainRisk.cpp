// RainRisk.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>

#define _USE_MATH_DEFINES
#include <math.h>

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

	enum Facing
	{
		North = 0,
		East = 90,
		South = 180,
		West = 270,
		Count = 360
	};

	Facing currentFacing = Facing::East;

	int vertDistance = 0;
	int horzDistance = 0;
	int currentDegree = (int)currentFacing;


	// Read in the states of the seats in the waiting room
	for (std::string line; std::getline(source, line);)
	{
		// Regular expression for grabbing anything that is a colour in the input text.
		std::regex regex("(\\w)(\\d+)");

		// Hold matches for searching with the above regex
		std::smatch res;

		// Get us the first bag in the line which holds rules for what it contains
		std::regex_search(line, res, regex);
		std::string operation = res[1];
		int number = std::stoi(res[2]);

		if (operation == "N")
		{
			vertDistance += number;
		}

		if (operation == "E")
		{
			horzDistance += number;
		}

		if (operation == "S")
		{
			vertDistance -= number;
		}

		if (operation == "W")
		{
			horzDistance -= number;
		}

		if (operation == "L")
		{
			currentDegree -= number;
			if (currentDegree < 0)
				currentDegree += 360;

			currentFacing = (Facing)currentDegree;
		}

		if (operation == "R")
		{
			currentDegree += number;
			if (currentDegree > 360)
				currentDegree -= 360;

			currentFacing = (Facing)currentDegree;
		}

		if (operation == "F")
		{
			if (currentFacing == Facing::North)
			{
				vertDistance += number;
			}
			if (currentFacing == Facing::East)
			{
				horzDistance += number;
			}
			if (currentFacing == Facing::South)
			{
				vertDistance -= number;
			}
			if (currentFacing == Facing::West)
			{
				horzDistance -= number;
			}
		}

		if (currentFacing == Facing::Count)
			currentFacing = Facing::North;
	}

	int answer = abs(vertDistance) + abs(horzDistance);
	printf("Distance traveled: %d\n", answer);

	source.clear();
	source.seekg(0);

	currentFacing = Facing::East;

	vertDistance = 0;
	horzDistance = 0;

	int waypointVertDistance = 1;
	int waypointHorzDistance = 10;
	currentDegree = (int)currentFacing;

	// Read in the states of the seats in the waiting room
	for (std::string line; std::getline(source, line);)
	{
		// Regular expression for grabbing anything that is a colour in the input text.
		std::regex regex("(\\w)(\\d+)");

		// Hold matches for searching with the above regex
		std::smatch res;

		// Get us the first bag in the line which holds rules for what it contains
		std::regex_search(line, res, regex);
		std::string operation = res[1];
		int number = std::stoi(res[2]);

		if (operation == "N")
		{
			waypointVertDistance += number;
		}

		if (operation == "E")
		{
			waypointHorzDistance += number;
		}

		if (operation == "S")
		{
			waypointVertDistance -= number;
		}

		if (operation == "W")
		{
			waypointHorzDistance -= number;
		}

		if (operation == "L")
		{
			// Rotate a point around the origin formula
			// xcos(theta) - ysin(theta)
			// xsin(theta) + ycos(theta)

			float rads = number * (float)M_PI / 180.0f * 1;
			float cosTheta = cos(rads);
			float sinTheta = sin(rads);

			float newX = (waypointHorzDistance * cosTheta) - (waypointVertDistance * sinTheta);
			float newY = (waypointHorzDistance * sinTheta) + (waypointVertDistance * cosTheta);

			waypointHorzDistance = round(newX);
			waypointVertDistance = round(newY);

		}

		if (operation == "R")
		{
			// Rotate a point around the origin formula
			// xcos(theta) - ysin(theta)
			// xsin(theta) + ycos(theta)

			float rads = number * (float)M_PI / 180.0f * -1;
			float cosTheta = cos(rads);
			float sinTheta = sin(rads);

			float newX = (waypointHorzDistance * cosTheta) - (waypointVertDistance * sinTheta);
			float newY = (waypointHorzDistance * sinTheta) + (waypointVertDistance * cosTheta);

			waypointHorzDistance = round(newX);
			waypointVertDistance = round(newY);
		}

		if (operation == "F")
		{
			vertDistance += number * waypointVertDistance;
			horzDistance += number * waypointHorzDistance;
		}

		if (currentFacing == Facing::Count)
			currentFacing = Facing::North;
	}

	answer = abs(vertDistance) + abs(horzDistance);
	printf("Distance traveled: %d\n", answer);

}
