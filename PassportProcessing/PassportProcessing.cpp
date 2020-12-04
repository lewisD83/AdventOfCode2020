// PassportProcessing.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

	std::vector<std::string> entries;
	
	// Keep track of valid passports
	int validPassports = 0;

	// Keep track of valid entries
	bool hasValidBYR = false;
	bool hasValidIYR = false;
	bool hasValidEYR = false;
	bool hasValidHGT = false;
	bool hasValidHCL = false;
	bool hasValidECL = false;
	bool hasValidPID = false;
	bool hasValidCID = false;

	// Read text file line by line
	for (std::string line; std::getline(source, line);)
	{
		bool endOfFile = source.eof();

		// There is another line in this current passport so lets grab all the entries in it
		if(line.empty() == false)
		{
			// Make a stream for the line itself
			std::istringstream in(line);

			// Read in every individual entry seperated by spaces
			std::string entry;
			while (in >> entry)
			{
				// All entries are made up of a key and value seperated by a ':'
				size_t pos = entry.find(":");
				std::string entryKey = entry.substr(0, pos);
				std::string entryValue = entry.substr(pos + 1);

				// byr (Birth Year) - four digits; at least 1920 and at most 2002.
				if (entryKey.compare("byr") == 0)
				{
					int bdayYear = std::stoi(entryValue);
					hasValidBYR = (1920 <= bdayYear && bdayYear <= 2002);
					
				}
				// iyr (Issue Year) - four digits; at least 2010 and at most 2020.
				else if (entryKey.compare("iyr") == 0)
				{
					int issueYear = std::stoi(entryValue);
					hasValidIYR = (2010 <= issueYear && issueYear <= 2020);
				}
				//eyr(Expiration Year) - four digits; at least 2020 and at most 2030.
				else if (entryKey.compare("eyr") == 0)
				{
					int expYear = std::stoi(entryValue);
					hasValidEYR = (2020 <= expYear && expYear <= 2030);
				}
				// hgt (Height) - a number followed by either cm or in:
				// If cm, the number must be at least 150 and at most 193.
				// If in, the number must be at least 59 and at most 76.
				else if (entryKey.compare("hgt") == 0)
				{
					size_t foundCM = entryValue.find("c");
					size_t foundIN = entryValue.find("i");

					if (foundCM != std::string::npos)
					{
						int height = std::stoi(entryValue.substr(0, pos) );
						hasValidHGT = (150 <= height && height <= 193);
					}

					if (foundIN != std::string::npos)
					{
						int height = std::stoi(entryValue.substr(0, pos));
						hasValidHGT = (59 <= height && height <= 76);
					}
				}
				// hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
				else if (entryKey.compare("hcl") == 0)
				{
					hasValidHCL =	entryValue[0] == '#' && 
									entryValue.length() == 7 && 
									entryValue.find_first_not_of("abcdefABCDEF0123456789", 1) == std::string::npos;
				}
				// ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
				else if (entryKey.compare("ecl") == 0)
				{
					hasValidECL =   entryValue.compare("amb") == 0 || 
									entryValue.compare("blu") == 0 || 
									entryValue.compare("brn") == 0 || 
									entryValue.compare("gry") == 0 || 
									entryValue.compare("grn") == 0 || 
									entryValue.compare("hzl") == 0 || 
									entryValue.compare("oth") == 0;
				}
				// pid (Passport ID) - a nine-digit number, including leading zeroes.
				else if (entryKey.compare("pid") == 0)
				{
					hasValidPID =	entryValue.length() == 9 &&
									entryValue.find_first_not_of("0123456789") == std::string::npos;
					
				}
				// cid(Country ID) - ignored, missing or not.
				else if (entryKey.compare("cid") == 0)
				{
					hasValidCID = true;
				}
				else
				{
					// Invalid Entry
				}

				// Store the entries
				entries.push_back(entry);
			} 
		}

		// The current line we are on is empty so the next one will be a new passport so we have read in a full passport, lets check it 
		if(line.empty() == true || endOfFile)
		{
			// We know that a passport with all 8 valid entries is correct
			if (entries.size() == 8 && 
				hasValidBYR &&
				hasValidIYR &&
				hasValidEYR &&
				hasValidHGT &&
				hasValidHCL &&
				hasValidECL &&
				hasValidPID &&
				hasValidCID)
			{
				validPassports++;
			}
			// A passport with 7 entries can be ok as long as it has  no CID
			else if (entries.size() == 7 &&
				hasValidBYR &&
				hasValidIYR &&
				hasValidEYR &&
				hasValidHGT &&
				hasValidHCL &&
				hasValidECL &&
				hasValidPID &&
				hasValidCID == false)
			{
				validPassports++;
			}
			// Anything else is an invalid passport, just do nothing
			else
			{
				// Invalid passport
			}

			// Clear all our entries and flags ready for the next passport
			hasValidBYR = false;
			hasValidIYR = false;
			hasValidEYR = false;
			hasValidHGT = false;
			hasValidHCL = false;
			hasValidECL = false;
			hasValidPID = false;
			hasValidCID = false;
			entries.clear();
		}
	}

	printf("ValidPassports %d", validPassports);
}
