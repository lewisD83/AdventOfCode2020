// CustomCustoms.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <assert.h> 

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

	// Hold the current groups answers
	std::vector<std::string> currentGroupAnswers;

	// A key of unique answers made by this group
	std::string uniqueAnswersKey;

	// Total yes answers
	int totalYesAnswers = 0;

	// Total number of answers answered by all members in a group
	int totalCommonYesAnswers = 0;

	// Read stream line by line
	for (std::string line; std::getline(source, line);)
	{
		// Double check if we are at the end of the file
		// The last entry in the list does not have a line break at the end so need to catch it
		bool endOfFile = source.eof();

		// Make a stream for the line itself
		std::istringstream in(line);

		// Push this persons answers
		if (line.empty() == false)
		{
			currentGroupAnswers.push_back(line);
		}
		
		// If we have an empty line or the end of file then we have collected a full groups answers, lets process it 
		if (line.empty() || endOfFile)
		{
			// For all answers in this group
			for (auto& iter : currentGroupAnswers)
			{
				std::string answers = iter;

				// Only look for answers that haven't been used yet
				while (iter.find_first_not_of(uniqueAnswersKey.c_str()) != std::string::npos)
				{
					// Find the first answer that doens't appear in our key
					int index = iter.find_first_not_of(uniqueAnswersKey.c_str());

					// Now add it so we don't add it again for any other person who answered the same
					uniqueAnswersKey.append(1, answers[index]);

					// We have a new unique answer so increment our count
					totalYesAnswers++;
				}
			}

			// If this is the only person in the group then all there answers are unique
			if (currentGroupAnswers.size() == 1)
			{
				totalCommonYesAnswers += uniqueAnswersKey.length();
			}
			else
			{
				// We now need to check every character in the unique key 
				for (auto& c : uniqueAnswersKey)
				{
					int numberOfPeopleAnswered = 0;

					// For every person in the group check if they answered the current question
					for (int i = 0; i < (int)currentGroupAnswers.size(); i++)
					{
						std::string answers = currentGroupAnswers[i];

						// This person answered yes to the current question
						if (answers.find_first_of(c) != std::string::npos)
						{
							numberOfPeopleAnswered++;
						}
					}

					// If everyone in the group answered yes to the current question then incrememt the common yes count
					if (numberOfPeopleAnswered == currentGroupAnswers.size())
					{
						totalCommonYesAnswers++;
					}
				}
			}

			// this group has been processed so lets reset
			uniqueAnswersKey.clear();
			currentGroupAnswers.clear();
		}
	}

	printf("Total Yes Answers: %d \n", totalYesAnswers);
	printf("Total Common Yes Answers: %d \n", totalCommonYesAnswers);

}
