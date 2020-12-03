// PasswordPhilosophy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

int main()
{
	// build a read-Stream
	std::ifstream source;

	// Open the sample problem
	source.open("Input2.txt", std::ios_base::in);

	// Check if there was a problem opening the file
	if (!source)
	{
		std::cout << "Can't open file!";
	}

	int validPasswords = 0;

	// Read stream line by line
	for (std::string line; std::getline(source, line);)
	{
		// Make a stream for the line itself
		std::istringstream in(line);

		// Read in the x and y coords
		std::string policy, checkLetter, password;
		in >> policy;
		in >> checkLetter;
		in >> password;

		size_t pos = policy.find("-");
		std::string lowerLimitStr = policy.substr(0, pos);
		std::string upperLimitStr = policy.substr(pos + 1);

		int lowerLimit = stoi(lowerLimitStr);
		int upperLimit = stoi(upperLimitStr);

		int count = 0;
		for (auto& c : password)
		{
			if (c == checkLetter[0])
			{
				count++;
			}
		}

		if (count >= lowerLimit && count <= upperLimit)
		{
			validPasswords++;
		}
	}

	printf("Valid Passwords %d", validPasswords);

	validPasswords = 0;
	source.clear(0);
	source.seekg(0);

	// Read stream line by line
	for (std::string line; std::getline(source, line);)
	{
		// Make a stream for the line itself
		std::istringstream in(line);

		// Read in the x and y coords
		std::string policy, checkLetter, password;
		in >> policy;
		in >> checkLetter;
		in >> password;

		size_t pos = policy.find("-");
		std::string lowerPositionStr = policy.substr(0, pos);
		std::string upperPositionStr = policy.substr(pos + 1);

		int lowerPosition = stoi(lowerPositionStr) - 1;
		int upperPosition = stoi(upperPositionStr) - 1;

		if (password[lowerPosition] == checkLetter[0] || password[upperPosition] == checkLetter[0])
		{
			if (password[lowerPosition] != password[upperPosition])
			{
				validPasswords++;
			}
		}
	}


	printf("Valid Passwords %d", validPasswords);

	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
