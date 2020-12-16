#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <regex>

// Change int to a binary string
std::string toBinary(uint64_t n)
{
	std::string r("000000000000000000000000000000000000");
	int index = r.length() - 1;
	while (n != 0) 
	{ 
		char c = (n % 2 == 0 ? '0' : '1');
		r[index] = c;
		n /= 2; 
		index--;
	}
	return r;
}

// Change a binary string to an int
uint64_t toDecimal(std::string str)
{
	long long n = 0;
	long long shift = 1;
	int index = str.length() - 1;

	while (index > -1)
	{
		int val = str[index] - '0';
		n += shift * val;

		index--;
		shift <<= 1;
	}

	return n;
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

	std::map<int, uint64_t> mem;
	std::string currentMask;

	// Read in the states of the seats in the waiting room
	for (std::string line; std::getline(source, line);)
	{
		if (line.find("X") != std::string::npos)
		{
			// Regular expression for grabbing the mask string
			std::regex regex("mask = (\\w+)");

			// Hold matches for searching with the above regex
			std::smatch res;

			// Get the mask that matches the search
			std::regex_search(line, res, regex);
			currentMask = res[1];
		}
		else
		{
			// Regular expression for grabbing the address to write to and the value to write
			std::regex regex("mem\\[(\\d+)\\] = (\\d+)");

			// Hold matches for searching with the above regex
			std::smatch res;

			std::regex_search(line, res, regex);
			int addr = std::stoi(res[1]);
			int val = std::stoi(res[2]);

			std::string valStr = toBinary(val);
			int maskIndex = currentMask.length() - 1;

			for (int i = valStr.length() - 1; i > -1; i--)
			{
				if (currentMask[maskIndex] != 'X')
				{
					valStr[i] = currentMask[maskIndex];
				}

				maskIndex--;
			}

			uint64_t newVal = toDecimal(valStr);
			mem[addr] = newVal;

		}
	}

	uint64_t totalMemoryUsed = 0;
	for (auto& iter : mem)
	{
		totalMemoryUsed += iter.second;
	}

	printf("Total memory used: %I64u", totalMemoryUsed);

	source.clear();
	source.seekg(0);

	std::map<uint64_t, uint64_t> mem2;

	// Read in the states of the seats in the waiting room
	for (std::string line; std::getline(source, line);)
	{
		if (line.find("X") != std::string::npos)
		{
			// Regular expression for grabbing anything that is a colour in the input text.
			std::regex regex("mask = (\\w+)");

			// Hold matches for searching with the above regex
			std::smatch res;

			// Get us the first bag in the line which holds rules for what it contains
			std::regex_search(line, res, regex);
			currentMask = res[1];
		}
		else
		{
			// Regular expression for grabbing anything that is a colour in the input text.
			std::regex regex("mem\\[(\\d+)\\] = (\\d+)");

			// Hold matches for searching with the above regex
			std::smatch res;

			// Get us the first bag in the line which holds rules for what it contains
			std::regex_search(line, res, regex);
			int addr = std::stoi(res[1]);
			int val = std::stoi(res[2]);

			std::string addrStr = toBinary(addr);
			std::string addStrWithX = addrStr;
			printf("%s = %d\n", addrStr.c_str(), val);
			printf("%s\n", currentMask.c_str());

			std::vector<int> xIndexes;

			int maskIndex = currentMask.length() - 1;
			for (int i = addrStr.length() - 1; i > -1; i--)
			{
				if (currentMask[maskIndex] == '1' || currentMask[maskIndex] == 'X')
				{
					addStrWithX[i] = currentMask[maskIndex];
					addrStr[i] = '1';
					if (currentMask[maskIndex] == 'X')
					{
						// Pop in a temp 0 for now and keep track
						addrStr[i] = '0';
						xIndexes.push_back(maskIndex);
					}
				}

				maskIndex--;
			}

			uint64_t firstCombination = toDecimal(addrStr);
			std::vector<uint64_t> allCombinations((size_t)pow(2, xIndexes.size()), firstCombination);

			// Calculate all possible combinations for floating bits
			for (int i = 0; i < xIndexes.size(); ++i)
			{
				int pattern = (int)pow(2, i); // Calc amount off, amount on
				uint64_t bit = (uint64_t)1 << (35 - xIndexes[i]); // What is the value of the floating bit

				for (int j = 0; j < allCombinations.size(); j++)
				{
					int bitFactor = ((j / pattern) % 2) == 1; // Should this bit be 0 or 1
					uint64_t bitValue = bit * bitFactor;
					allCombinations[j] += bitValue;
				}
			}

			for (auto& iter : allCombinations)
			{
				mem2[iter] = val;
			}

		}
	}

	totalMemoryUsed = 0;
	for (auto& iter : mem2)
	{
		totalMemoryUsed += iter.second;
	}

	printf("Total memory used: %I64u", totalMemoryUsed);

}