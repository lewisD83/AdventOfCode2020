// HandyHaversacks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <assert.h> 
#include <regex>
#include <unordered_map>

// Structure to hold the contents of a bag
// Quantity and colour
struct Content
{
	Content() {};
	Content(const int quantity, std::string colour)
		: m_quantity(quantity)
		, m_colour(colour)
	{

	}

	const int GetQuantity()
	{
		return m_quantity;
	}

	const std::string GetColour()
	{
		return m_colour;
	}

private:
	int m_quantity;
	std::string m_colour;
};

// Map to hold all the combinations of coloured bags and the colours they hold
std::unordered_map<std::string, std::vector<std::string> > coloursMap;

// Map to hold all the combinations of coloured bags, the colours they hold and the quantities
std::unordered_map<std::string, std::vector<Content*> > coloursQuantityMap;

// Find the bag(searchstr) starting at key
bool FindBag(std::string key, std::string searchStr)
{
	// Have we found the bag we are looking for?
	if (key.compare(searchStr.c_str()) != 0)
	{
		// We haven't found the bag we are looking for but it could be deeper in the map
		if (coloursMap[key].empty() == false  )
		{
			std::vector<std::string> containedColours = coloursMap[key];

			// Check all bags in this current bags contained colours
			for (auto& iter : containedColours)
			{
				std::string colour = iter;
				if (FindBag(colour, searchStr))
				{
					// We found the shiny gold bag so return true all the way up the recursive function
					return true;
				}
			}
		}
	}
	else
	{
		return true;
	}

	return false;
}

// Part two which counts up the quantities of bags under the bag starting at key
int FindBag2(std::string key, int multiplier)
{
	int quantity = 0;

	// Have we reached a bag with no other contents?
	if (coloursMap[key].empty() == false)
	{
		std::vector<Content*> contents = coloursQuantityMap[key];

		// For all other bags under the current one add them all up 
		for (auto& iter : contents)
		{
			std::string colour = iter->GetColour();
			quantity += iter->GetQuantity();
			quantity += FindBag2(colour, iter->GetQuantity());
		}
	}
	else
	{
		// yes return 0 for no other bags
		return 0;
	}

	// We have added up all the bags under the current one so now the answer will be if there are multiple of the current one * everything underneath it
	return multiplier * quantity;
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

	for (std::string line; std::getline(source, line);)
	{
		// Double check if we are at the end of the file
		// The last entry in the list does not have a line break at the end so need to catch it
		bool endOfFile = source.eof();

		// Make a stream for the line itself
		std::istringstream in(line);

		// Regular expression for grabbing anything that is a colour in the input text.
		std::regex regex("(\\w+ \\w+[^ ]?) bag[s]?");

		// Regular expression for grabbing everything that is a quantity of bag
		std::regex regex2("(\\d+) (\\w+ \\w+) bag[s]?");

		// Hold matches for searching with the above regex
		std::smatch res;
		std::smatch res2;

		// Get us the first bag in the line which holds rules for what it contains
		std::regex_search(line, res, regex);
		std::string key = res[1];

		// Grab the rest of the colours in this list
		line = res.suffix();
		while (std::regex_search(line, res, regex) && std::regex_search(line, res2, regex2))
		{
			Content* content = new Content(std::stoi(res2[1]), res[1]);
			coloursMap[key].push_back(res[1]);
			coloursQuantityMap[key].push_back(content);
			line = res.suffix();
		};
	}

	int totalPathsToBag = 0;

	// Find all the paths that lead to shiny gold
	for (auto& item : coloursMap)
	{
		std::string key = item.first;
		std::vector<std::string> containedColours = item.second;

		for (auto& iter : containedColours)
		{
			std::string colour = iter;
			if (FindBag(colour, "shiny gold"))
			{
				totalPathsToBag++;
				break;
			}
		}
	}

	int totalBagsUnderBag = 0;
	std::vector<Content*> contents = coloursQuantityMap["shiny gold"];

	// Find the total number of bags underneath shiny gold
	for (auto& iter : contents)
	{
		Content* content = iter;
		int quantity = content->GetQuantity();
		std::string colour = content->GetColour();

		totalBagsUnderBag += quantity;
		totalBagsUnderBag += FindBag2(colour, quantity);
		
	}
}
