// OperationOrder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>

long long DoMath(std::istringstream& in)
{
	long long sum = 0;
	char operation = '0';

	// Read in the starting input
	std::string input;
	while (in >> input)
	{
		if (input == "+")
		{
			operation = '+';
		}
		else if (input == "*")
		{
			operation = '*';
		}
		else if (input == "(")
		{
			if (operation == '+')
			{
				sum += DoMath(in);
			}
			else if (operation == '*')
			{
				sum *= DoMath(in);
			}
			else
			{
				sum = DoMath(in);
			}
		}
		else if (input == ")")
		{
			return sum;
		}
		else if (operation == '0')
		{
			sum = std::stoi(input.c_str());
		}
		else
		{
			if (operation == '+')
			{
				sum += std::stoi(input.c_str());
			}
			else if (operation == '*')
			{
				sum *= std::stoi(input.c_str());
			}
		}

	}

	return sum;
}

long long ResolveBracket(std::stringstream& in, long long sum)
{
	long long answer = sum;
	char operation = '0';

	std::string input;

	while (in >> input)
	{
		if (input == "+")
		{
			operation = '+';
		}
		else if (input == "*")
		{
			operation = '*';
		}
		else
		{
			long long currentNumber = std::stoi(input.c_str());
			if (answer == 0)
			{
				answer = currentNumber;
			}

			// Always resolve adds immediately
			if (operation == '+')
			{
				answer += currentNumber;
			}
			// For multiplication we recurse in to make sure we do any adds on the other side of this multiplication
			else if(operation=='*')
			{
				answer *= ResolveBracket(in, currentNumber); 
			}
		}
	}

	return answer;

}
/*
To make this simpler i will go through the whole string and resolve each part of parenthesis
With each iteration we will end up with one whole string with no brackets and we can resolve the more simplified string e.g.
going from right to left you can see the string gets simpler and is much easier to debug than using recursion

( 5 + 7 ) * 2 * 4 + 6 + 7 + ( 2 + 2 + 6 + ( 9 + 3 + 7 * 3 ) )

( 5 + 7 ) * 2 * 4 + 6 + 7 + ( 2 + 2 + 6 + 57 )

( 5 + 7 ) * 2 * 4 + 6 + 7 + 67

12 * 2 * 4 + 6 + 7 + 67

12 * 2 * 84

2016

*/
long long DoMath2(std::string line, long long sum)
{
	while(true)
	{
		// Find the start of the left most set of parenthesis
		int start = line.find_last_of("(");

		// If we didn't find any brackets then we have fully got rid of all brackets and the string thats left is the final sum
		if (start < 0)
		{
			std::stringstream in2(line);
			long long bracketSum = ResolveBracket(in2, 0);
			return bracketSum;
		}

		// Strip out everythig starting at this '(' and go to the end of the line
		// We need to do this as there could be a situation where there could be multiple closing brackets beside each other
		std::string replaceString2 = line.substr(start, line.length());

		// Find the matching bracket position
		int end = start + replaceString2.find_first_of(")");

		// adjust the start and end to only get whats inside the brackets and not the brackets themselves
		start += 1;
		end += -1;
		size_t length = end - start + 1;

		// Grab the string with no brackets
		std::string replaceString = line.substr(start, length);

		// Resolve the bracket and get the answer
		std::stringstream in2(replaceString);
		long long bracketSum = ResolveBracket(in2, 0);

		// Unadjust the start and end to now include the brackets
		start += -1;
		end += 1;
		length = end - start + 1;

		// We now have an answer for the sum inside these brackets so replace the whole bit with the answer
		line.replace(start, length, std::to_string(bracketSum));
	}

	return sum;
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

	long long answer = 0;
	long long answer2 = 0;

	// Read in the states of the seats in the waiting room
	for (std::string line; std::getline(source, line);)
	{
		// Do answer to part 1
		std::istringstream in(line);
		answer += DoMath(in);

		// Do answer to part 2
		answer2 += DoMath2(line, 0);
	}

	printf("Answer = %I64d\n", answer);
	printf("Answer2 = %I64d\n", answer2);
}