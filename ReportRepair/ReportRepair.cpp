// ReportRepair.cpp : This file contains the 'main' function. Program execution begins and ends there.
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


	// Vector of Numbers
	std::vector<int> numbers;

	// Read stream line by line
	for (std::string line; std::getline(source, line);)
	{
		// Make a stream for the line itself
		std::istringstream in(line);

		// Read in the x and y coords
		int x = 0;
		in >> x;

		numbers.push_back(x);
	}

	int answer = 0;

	for (std::vector<int>::iterator iter1 = numbers.begin(); iter1 != numbers.end(); iter1++)
	{
		int testNumber1 = (*iter1);

		for (std::vector<int>::iterator iter2 = numbers.begin(); iter2 != numbers.end(); iter2++)
		{
			int testNumber2 = (*iter2);

			for (std::vector<int>::iterator iter3 = numbers.begin(); iter3 != numbers.end(); iter3++)
			{
				int testNumber3 = (*iter3); 
				
				int addition = testNumber1 + testNumber2 + testNumber3;

				if (addition == 2020)
				{
					answer = testNumber1 * testNumber2 * testNumber3;
				}
			}
			
		}
	}
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
