#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <regex>

// Hold details about a ticket field
class TicketField
{
public:
	TicketField() {};
	TicketField(std::string name, int limit1, int limit2, int limit3, int limit4)
		: m_fieldName(name)
		, m_lowerLimit1(limit1)
		, m_upperLimit1(limit2)
		, m_lowerLimit2(limit3)
		, m_upperLimit2(limit4)
		, m_fieldID(-1)
	{

	}

public:
	std::string		m_fieldName;
	int				m_lowerLimit1;
	int				m_upperLimit1;
	int				m_lowerLimit2;
	int				m_upperLimit2;
	int				m_fieldID;

};

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

	std::vector<int> numsToCheck;
	std::vector<TicketField*> ticketFields;

	for (std::string line; std::getline(source, line);)
	{
		// Regular expression for grabbing the fields and ranges string
		std::regex regex("(\\w+ \\w+): (\\d+)-(\\d+) or (\\d+)-(\\d+)");
		std::regex regex2("(\\w+): (\\d+)-(\\d+) or (\\d+)-(\\d+)");

		// Hold matches for searching with the above regex
		std::smatch res;
		bool match = false;

		// If there is a match then we are still reading in the ticket fields
		if (std::regex_search(line, res, regex) || std::regex_search(line, res, regex2))
		{
			std::string field = res[1];
			int lowerLimit1 = std::stoi(res[2]);
			int upperLimit1 = std::stoi(res[3]);
			int lowerLimit2 = std::stoi(res[4]);
			int upperLimit2 = std::stoi(res[5]);

			ticketFields.push_back(new TicketField(field, lowerLimit1, upperLimit1, lowerLimit2, upperLimit2));
			match = true;
		}

		// If no matches then we are onto the nearby tickets
		if (match == false)
		{
			int delimiter = 0;
			do
			{
				delimiter = (int)line.find(',');
				if (delimiter != -1)
				{
					std::string busNumStr = line.substr(0, delimiter);
					numsToCheck.push_back(std::stoi(busNumStr));
					line = line.substr(delimiter + 1);

					delimiter = (int)line.find(',');
					if (delimiter == -1)
					{
						numsToCheck.push_back(stoi(line));
					}

				}

			} while (delimiter != -1);

		}
	}

	std::vector<std::vector<int> > nearbyTickets;
	std::vector<int> tickets;
	int errorRate = 0;
	int col = 0;
	int row = 0;
	bool hasError = false;

	// Part 1 
	// Add up all the fields which are an error and don't fit in at all
	// Also store all valid tickets for part 2
	// For every number in the input
	for (auto& number : numsToCheck)
	{
		col++;
		bool errorNumber = true;

		// Check number against our ticket fields 
		for (auto& ticket : ticketFields)
		{
			if ( (number >= ticket->m_lowerLimit1 && number <= ticket->m_upperLimit1) || (number >= ticket->m_lowerLimit2 && number <= ticket->m_upperLimit2) )
			{
				errorNumber = false;
				break;
			}
		}

		tickets.push_back(number);

		// Have we found an error number?
		if (errorNumber)
		{
			errorRate += number;
			hasError = true;
		}

		// Bit of a cheat but i know that there are 20 numbers in a line
		// Once we reach the end of a linego back to the beginning
		if (col == 20 && tickets.size() == 20 && !hasError)
		{
			nearbyTickets.push_back(tickets);
			col = 0;
			tickets.clear();
			hasError = false;
		}
		else if (hasError && col == 20)
		{
			col = 0;
			tickets.clear();
			hasError = false;
		}

	}

	printf("\nError rate: %d", errorRate);

	col = 0;
	row = 0;

	int foundFields = 0;
	while (foundFields != ticketFields.size())
	{
		// Loop to the beginning
		if (col == 20)
			col = 0;

		std::vector<bool> possibilities(ticketFields.size(), true);
		int leftToCheck = ticketFields.size() - foundFields;


		// Depth first search of all tickets in the first position and so on
		for (row = 1; row < (int)nearbyTickets.size(); row++)
		{
			int number = nearbyTickets[row][col];

			for (int i = 0; i < (int)ticketFields.size(); i++)
			{
				TicketField* ticket = ticketFields[i];
				if (ticket->m_fieldID == -1)
				{
					// If this number doens't fit into this range then it can't be this field so we can rule it out
					if ((number < ticket->m_lowerLimit1 || number > ticket->m_upperLimit1) && (number < ticket->m_lowerLimit2 || number > ticket->m_upperLimit2))
					{
						leftToCheck--;
						possibilities[i] = false;
					}
				}
				else
				{

					possibilities[i] = false;
				}
			}
		}

		// After going through all the ticket fields if we are left with only 1 of them that all the numbers fit into then we have found the position of a field
		if (leftToCheck == 1)
		{
			for (int i = 0; i < (int)possibilities.size(); i++)
			{
				bool possibility = possibilities[i];
				TicketField* ticket = ticketFields[i];

				if (ticket->m_fieldID == -1 && possibility == true)
				{
					ticket->m_fieldID = col;
					break;
				}
			}

			foundFields++;
		}

		col++;
	}

	long long answer = 1;

	for (int i = 0; i < (int)ticketFields.size(); i++)
	{
		TicketField* ticket = ticketFields[i];

		printf("\n%s: %d", ticket->m_fieldName.c_str(), ticket->m_fieldID);

		if (ticket->m_fieldName.find("departure") == 0)
		{
			answer *= nearbyTickets[0][ticket->m_fieldID];
		}
	}

	printf("\nAnswer = %llu", answer);

}















//#include <iostream>
//#include <sstream>
//#include <fstream>
//#include <vector>
//#include <unordered_map>
//#include <regex>
//
//int main(int argc, char *argv[])
//{
//	// build a read-Stream
//	std::ifstream source;
//
//	// Open the sample problem
//	source.open(argv[1], std::ios_base::in);
//
//	// Check if there was a problem opening the file
//	if (!source)
//	{
//		std::cout << "Can't open file!";
//	}
//
//	std::vector<int> numsToCheck;
//	std::unordered_map<int, int> ranges;
//
//	for (std::string line; std::getline(source, line);)
//	{
//		// Regular expression for grabbing the mask string
//		std::regex regex("(\\d+)-(\\d+)");
//
//		// Hold matches for searching with the above regex
//		std::smatch res;
//		bool match = false;
//
//		while (std::regex_search(line, res, regex))
//		{
//			int lowerLimit = std::stoi(res[1]);
//			int upperLimit = std::stoi(res[2]);
//
//			if (ranges.find(lowerLimit) != ranges.end())
//			{
//				int limit = ranges[lowerLimit];
//				if (upperLimit > limit)
//				{
//					ranges[lowerLimit] = upperLimit;
//				}
//			}
//			else
//			{
//				ranges[lowerLimit] = upperLimit;
//			}
//
//			
//
//
//			line = res.suffix();
//			match = true;
//		};
//
//		if(match == false)
//		{
//			int delimiter = 0;
//			do
//			{
//				delimiter = (int)line.find(',');
//				if (delimiter != -1)
//				{
//					std::string busNumStr = line.substr(0, delimiter);
//					numsToCheck.push_back(std::stoi(busNumStr));
//					line = line.substr(delimiter + 1);
//
//					delimiter = (int)line.find(',');
//					if (delimiter == -1)
//					{
//						numsToCheck.push_back(stoi(line));
//					}
//
//				}
//
//			} while (delimiter != -1);
//
//		}
//	}
//
//	std::vector<int> errorNumbers;
//	int errorRate = 0;
//	for (auto& number : numsToCheck)
//	{
//		bool errorNumber = true;
//
//		for (auto& range : ranges)
//		{
//			if (number >= range.first && number <= range.second)
//			{
//				errorNumber = false;
//				break;
//			}
//		}
//
//		if (errorNumber)
//		{
//			errorNumbers.push_back(number);
//			errorRate += number;
//			printf("%d\n", number);
//		}
//	}
//
//	printf("\nError rate: %d", errorRate);
//}