// EncodingError.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <deque>
#include <algorithm>

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

	// Keep track of the pre amble queue
	std::deque<long long> preRambleQueue;

	int lineCount = 0;

	// Log the number which is invalid
	long long invalidNumber = 0;
	
	// Part 1
	for (std::string line; std::getline(source, line);)
	{
		bool isValid = false;
		lineCount++;

		// Make a stream for the line itself
		std::istringstream in(line);

		// Get the current number we are checking
		long long currentNum = 0;
		in >> currentNum;

		// We are now dealing with non preamble numbers so need to check them
		if (lineCount > 25)
		{
			// For all numbers in the preamble queue, check them against all other numbers in the queue to check if any two add to give the current number
			for (int i = 0; i < (int)preRambleQueue.size(); i++)
			{
				long long firstNum = preRambleQueue.at(i);

				for (int j = 0; j < (int)preRambleQueue.size(); j++)
				{
					// Don't check the same number against itself
					if (i == j)
					{
						continue;
					}

					long long secondNum = preRambleQueue.at(j);

					// Have we found two numbers that add to the current number, if so it is a valid number so update our queue and break out the for loops
					if (firstNum + secondNum == currentNum)
					{
						printf("%lld is the sum of %lld + %lld.\n", currentNum, firstNum, secondNum);
						preRambleQueue.pop_front();
						preRambleQueue.push_back(currentNum);
						isValid = true;
						goto validNumber;
					}
				}
			}

			// GOTO statement cause fuck it
			validNumber:;
			if (isValid == false)
			{
				invalidNumber = currentNum;
				goto end;
			}
		}
		else
		{
			preRambleQueue.push_back(currentNum);
		}		
	}

end:;
	printf("Invalid Number: %lld", invalidNumber);

	// Part 2
	source.clear();
	source.seekg(0);

	std::deque<long long> contiguousQueue;
	lineCount = 0;

	for (std::string line; std::getline(source, line);)
	{
		bool isValid = false;
		lineCount++;

		// Make a stream for the line itself
		std::istringstream in(line);

		// Get the current number we are checking
		long long currentNum = 0;
		in >> currentNum;

		// Don't include the invalid number in our calculations
		if(currentNum != invalidNumber)
		{
			// Only start adding if there are mor than 1 number to add
			if (contiguousQueue.size() > 1)
			{
				long long sumOfQueue = 0;

				// Add up every number in the contiguous queue
				for (int i = 0; i < (int)contiguousQueue.size(); i++)
				{
					sumOfQueue += contiguousQueue.at(i);
				}

				// If we have added up past the invalid number then pop the front of the queue and check the sum until we are back in range
				while (sumOfQueue > invalidNumber)
				{
					long long popped = contiguousQueue.front();
					sumOfQueue -= popped;
					contiguousQueue.pop_front();
				}

				// Have we found the set of contiguous numbers that add to give the invalid number
				if (sumOfQueue == invalidNumber)
				{
					isValid = true;
				}
			}

			if (isValid == true)
			{
				break;
			}

			contiguousQueue.push_back(currentNum);
		}
		else
		{
			// We've gone passed the invalid number so everything in the queue is not contiguous so clear it and start again
			contiguousQueue.clear();
		}
	}
	
	long long minNumber = (*std::min_element(contiguousQueue.begin(), contiguousQueue.end()));
	long long maxNumber = (*std::max_element(contiguousQueue.begin(), contiguousQueue.end()));

	long long answer = minNumber + maxNumber;
	printf("Answer: %lld.", answer);


}