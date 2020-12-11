// AdapterArray.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

	std::vector<int> adapaters;
	std::vector<int> differences;

	for (std::string line; std::getline(source, line);)
	{
		// Make a stream for the line itself
		std::istringstream in(line);

		// Get the current number we are checking
		int joltage = 0;
		in >> joltage;

		// adapters is empty so push the first element on to the vector
		if (adapaters.empty())
		{
			adapaters.push_back(joltage);
		}
		else
		{
			// As we read in input its much easier to work out if we sort it into ascending order
			for (std::vector<int>::iterator iter = adapaters.begin(); ; )
			{
				int currentJoltage = *iter;

				// If the joltage we read in is higher than current joltage then move up in the vector till we find a place to put it 
				if (joltage > currentJoltage)
				{
					iter++;
				}
				else
				{
					adapaters.emplace(iter, joltage);
					break;
				}

				// If we reach the end then this is now the new highest number in the vector so just push it to the back
				if (iter == adapaters.end())
				{
					adapaters.push_back(joltage);
					break;
				}
			}
		}
	}

	int ones = 0, twos = 0, threes = 1;

	// Now lets work out the differences in joltage between all the adapters
	for (int i = 0; i < (int)adapaters.size(); i++)
	{
		// If w are on the first adapter then the difference is just the adpaters joltage
		// Otherwise the differece is the current one minus the one before it
		int difference = (i==0) ? adapaters[0] : (adapaters[i] - adapaters[i - 1]);

		// Count up the differences of one
		if (difference == 1)
		{
			ones++;
		}
		// Count up the differences of two
		else if (difference == 2)
		{
			twos++;
		}

		// Count up the differences of three
		else if(difference == 3)
		{
			threes++;
		}
		else
		{
			// Too big a difference adapter won't work
		}

		// Store the differences
		printf("%d\n", difference);
		differences.push_back(difference);
	}

	printf("\n");
	printf("%d differences of 1\n", ones);
	printf("%d differences of 2\n", twos);
	printf("%d differences of 3\n", threes);




	// So after hacking a solution and getting the answer it turns out that i didn't realise i was working out the tribonacci sequence
	// I started by working out how many unique paths there would be for a certain number of consecutive ones. I only went as far as 4 because i had my code output the differences so i was sort of cheating
	// Below is the number of unique paths for a given input
	// 1,2				= 1 unique paths 
	// 1,2,3			= 2 unique paths
	// 1,2,3,4			= 4 unique paths
	// 1,2,3,4,5		= 7 unique paths
	// 1,2,3,4,5,6		= 13 unique paths

	// So the solution is to go through the differences and count up the number of conseuctive ones and plug them into a formula using tribonnaci

	// Hold some tribonacci numbers, definitely won't go as high as 149, but popped it in anyway
	int tribonacci[10] = { 1, 1, 2, 4, 7, 13, 24, 44, 81, 149 };

	std::vector<int> chainCounts(10, 0);
	int currentCount = 0;

	// For all the differences go through and count the occurrences of n number of consecuitve 1's
	for (int i = 0; i < (int)differences.size(); i++)
	{
		if (differences[i] == 1)
		{
			currentCount++;
		}

		// If we reach a 3 or the end of the list then we have a chain of 1's
		// Incremement the correct count of this number of 1's
		if (differences[i] == 3 || i == adapaters.size() - 1)
		{
			if (currentCount < (int)chainCounts.size())
			{
				chainCounts[currentCount] += 1;;
			}

			// Reset our count
			currentCount = 0;
		}
	}

	double answer = 1;

	// Using tribonacci we can work out the answer
	/*
		This is the sample input sorted in ascending order and with the differences beside it
		[0]		1	 1
		[1]		4	 3
		[2]		5	 1
		[3]		6	 1
		[4]		7	 1
		[5]		10	 3
		[6]		11	 1
		[7]		12	 1
		[8]		15	 3
		[9]		16	 1
		[10]	19	 3
	
	from this there are 
	2 occurrences of just a 1 on its own
	1 occurrence of two 1's beside each other
	1 occurrence of three 1's beside each other

	x = number of consecutive chain of two 1's
	y = number of consecutive chain of three 1's
	z = number of consecutive chain of four 1's

	answer = 2^x * 4^y * 7^z

	notice how we don't care about consecutive 1's as they don't lead to any more unique paths
	also notice the 2, 4, and 7 from the tribonnaci sequence
	
	*/

	for (int i = 0; i < (int)chainCounts.size(); i++)
	{
		// We don't care about consecutive 1's so just skip over
		if (i == 1)
		{
			continue;
		}

		answer *= std::pow(tribonacci[i], chainCounts[i]);
	}

	printf("\n");
	printf("Unique paths: %.0lf\n", answer);




}