// HandheldHalting.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
#include <unordered_map>

struct Instruction
{
	Instruction() {};
	Instruction(std::string operation, std::string operatorStr, int accumulator)
		: m_operation(operation)
		, m_op(operatorStr)
		, m_accumulator(accumulator)
	{

	}

	std::string GetOperation()
	{
		return m_operation;
	}

	std::string GetOp()
	{
		return m_op;
	}

	int GetAccumulator()
	{
		return m_accumulator;
	}

private:
	std::string m_operation;
	std::string m_op;
	int m_accumulator;
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

	std::vector<Instruction*> instructions;
	
	// Process each line 
	for (std::string line; std::getline(source, line);)
	{
		// Regular expression for grabbing anything that is a colour in the input text.
		std::regex regex("(\\w+) (\\+|\\-)(\\d+)");

		// Hold matches for searching with the above regex
		std::smatch res;

		// Get us the first bag in the line which holds rules for what it contains
		std::regex_search(line, res, regex);
		std::string operation = res[1];
		std::string op = res[2];
		int accumulator = std::stoi(res[3]);

		// Hold the details about this instruction
		Instruction* instruction = new Instruction(operation, op, accumulator);
		instructions.push_back(instruction);
	}

	int instructionIndex = 0;
	int globalAccumulator = 0;

	std::vector<bool> instructionsVisited(instructions.size(), false);

	// Part 1
	while (true)
	{
		// If we have reached the end of the instructions then we have no infinite loop
		if (instructionIndex == instructions.size())
		{
			break;
		}

		// Get the current instruction
		Instruction* instruction = instructions[instructionIndex];

		// If we have visited this instruction before then we have an infinite loop, break out
		if (instructionsVisited[instructionIndex])
		{
			break;
		}
		else
		{
			// Mark this instruction as seen
			instructionsVisited[instructionIndex] = true;

			std::string operation = instruction->GetOperation();
			int op = instruction->GetOp() == "+" ? 1 : -1;		
			int accumulator = instruction->GetAccumulator();

			printf("%d: %s %s%d\n", instructionIndex, operation.c_str(), instruction->GetOp().c_str(), accumulator);

			// Accumulate the accumulator
			if (operation == "acc")
			{
				instructionIndex++;
				globalAccumulator += op * accumulator;
			}
			// Jump down or up accumulator number of instructions
			else if (operation == "jmp")
			{
				// accumulator == 0 check to account for the fact that jmp could be 0 mayyybe? so we still need to go down at least one instruction in this event//
				// this is more prevalent in part two when nops can become jumps
				instructionIndex += accumulator == 0 ? 1 : op * accumulator;
			}
			// Any other instruction just jump down 1
			else
			{
				instructionIndex++;
			}
		}
	}

	// Print our answer
	printf("\nAccumulator at point of infinite loop: %d \n\n", globalAccumulator);

	// Reset everything
	instructionIndex = 0;
	globalAccumulator = 0;
	instructionsVisited.clear();
	instructionsVisited.resize(instructions.size(), false);

	// Keep track of the instruction we are currently changing
	int currentIndexToChange = -1;

	// Mark that we are currently in the middle of a fix
	bool testingCurrentFix = false;

	// Keep track of previous instructions that we have changed so that we don't attempt to change them again
	std::vector<bool> instructionsChanged(instructions.size(), false);

	// Part 2
	while (true)
	{
		// If we reach the end then we don't have an infinite loop and our fix has worked
		if (instructionIndex == instructions.size())
		{
			break;
		}

		// Get the current instruction
		Instruction* instruction = instructions[instructionIndex];
		
		// Have we visited this instruction before? If yes then our fix didn't work lets reset and try again
		if (instructionsVisited[instructionIndex])
		{
			instructionIndex = 0;
			globalAccumulator = 0;
			instructionsVisited.clear();
			instructionsVisited.resize(instructions.size(), false);
			testingCurrentFix = false;
			printf("Fix at index %d didn't work trying again with new instruction. \n\n", currentIndexToChange);
		}
		else
		{
			// Mark current as visited
			instructionsVisited[instructionIndex] = true;

			std::string operation = instruction->GetOperation();
			int op = instruction->GetOp() == "+" ? 1 : -1;
			int accumulator = instruction->GetAccumulator();

			if (operation == "jmp" || operation == "nop")
			{
				// If we have a jmp or a nop and this current index is not one we have tested before then go ahead and start testing this one
				if (instructionsChanged[instructionIndex] == false && testingCurrentFix == false)
				{
					testingCurrentFix = true;
					instructionsChanged[instructionIndex] = true;
					currentIndexToChange = instructionIndex;
				}
			}

			// acc peforms the same as before, increment the accumulator 
			if (operation == "acc")
			{
				instructionIndex++;
				globalAccumulator += op * accumulator;
			}
			else if (operation == "jmp")
			{
				// if we are supposed to jmp but are testing a fix on this line then do what a nop would do instead
				if (instructionIndex == currentIndexToChange && testingCurrentFix)
				{
					instructionIndex++;
				}
				else
				{
					instructionIndex += accumulator == 0 ? 1 : op * accumulator;
				}
			}
			else
			{
				// if we are supposed to nop but are testing a fix on this line then do what a jmp would do instead
				if (instructionIndex == currentIndexToChange && testingCurrentFix)
				{
					instructionIndex += accumulator == 0 ? 1 : op * accumulator;
				}
				else
				{
					instructionIndex++;
				}
			}
		}
	}

	printf("\nAccumulator after fixing infinite loop: %d \n\n", globalAccumulator);

	// Free up Instructions
	for (Instruction* instr : instructions)
	{
		delete instr;
		instr = nullptr;
	}
	instructions.clear();
	instructionsVisited.clear();
	

}
