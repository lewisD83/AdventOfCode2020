// ConwayCubes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <windows.h>


// Hold details about a conway cube
class Cube
{
public:
	Cube() {};
	Cube(int xPos, int yPos, int zPos, int wPos)
		: x(xPos)
		, y(yPos)
		, z(zPos)
		, w(wPos)
	{
	}

public:
	int	x;
	int	y;
	int	z;
	int	w;
};

std::vector<Cube*> prevActiveCubes;
std::vector<Cube*> currActiveCubes;
std::vector<Cube*> checkedCubes;

bool HaveCheckedCoord(int x, int y, int z, int w)
{
	for (auto& cube : checkedCubes)
	{
		int x2 = cube->x;
		int y2 = cube->y;
		int z2 = cube->z;
		int w2 = cube->w;

		if (x == x2 && y == y2 && z == z2 && w == w2)
			return true;
	}

	return false;
}

// Find all the active neighbors next to the current cube
int ActiveNeighbours(int x, int y, int z, int w)
{
	// Don't bother checking this cube if we already have before
	if (HaveCheckedCoord(x, y, z, w))
		return 0;

	int activeNeighbours = 0;
	for (auto& cube : prevActiveCubes)
	{
		int x2 = cube->x;
		int y2 = cube->y;
		int z2 = cube->z;
		int w2 = cube->w;

		// We will check this cube so don't need to do it here;
		if (x == x2 && y == y2 && z == z2 && w == w2)
			continue;
		
		// If this cube is too far away then its not counted
		if (abs(x2-x) > 1)
			continue;

		if (abs(y2-y) > 1)
			continue;

		if (abs(z2-z) > 1)
			continue;

		if (abs(w2 - w) > 1)
			continue;

		activeNeighbours++;
	}

	return activeNeighbours;
}

// Check the 9 cubes in a 2D slice at zw
void CheckSlice(int x, int y, int z, int w)
{
	// Check current Cube
	if (ActiveNeighbours(x, y, z, w) == 3)
	{
		currActiveCubes.push_back(new Cube(x, y, z, w));
		checkedCubes.push_back(new Cube(x, y, z, w));
	}

	// Now lets check the neighbours around this current active cubes against other active cubes.
	// Check up-left
	if (ActiveNeighbours(x - 1, y - 1, z, w) == 3)
	{
		currActiveCubes.push_back(new Cube(x - 1, y - 1, z, w));
		checkedCubes.push_back(new Cube(x - 1, y - 1, z, w));
	}

	// Check up
	if (ActiveNeighbours(x, y - 1, z, w) == 3)
	{
		currActiveCubes.push_back(new Cube(x, y - 1, z, w));
		checkedCubes.push_back(new Cube(x, y - 1, z, w));
	}

	// Check up-right
	if (ActiveNeighbours(x + 1, y - 1, z, w) == 3)
	{
		currActiveCubes.push_back(new Cube(x + 1, y - 1, z, w));
		checkedCubes.push_back(new Cube(x + 1, y - 1, z, w));
	}

	// Check right
	if (ActiveNeighbours(x + 1, y, z, w) == 3)
	{
		currActiveCubes.push_back(new Cube(x + 1, y, z, w));
		checkedCubes.push_back(new Cube(x + 1, y, z, w));
	}

	// Check down-right
	if (ActiveNeighbours(x + 1, y + 1, z, w) == 3)
	{
		currActiveCubes.push_back(new Cube(x + 1, y + 1, z, w));
		checkedCubes.push_back(new Cube(x + 1, y + 1, z, w));
	}

	// Check down
	if (ActiveNeighbours(x, y + 1, z, w) == 3)
	{
		currActiveCubes.push_back(new Cube(x, y + 1, z, w));
		checkedCubes.push_back(new Cube(x, y + 1, z, w));
	}

	// Check down-left
	if (ActiveNeighbours(x - 1, y + 1, z, w) == 3)
	{
		currActiveCubes.push_back(new Cube(x - 1, y + 1, z, w));
		checkedCubes.push_back(new Cube(x - 1, y + 1, z, w));
	}

	// Check left
	if (ActiveNeighbours(x - 1, y, z, w) == 3)
	{
		currActiveCubes.push_back(new Cube(x - 1, y, z, w));
		checkedCubes.push_back(new Cube(x - 1, y, z, w));
	}
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

	{
		// Just say that the first state we read in is (0,0,0,0)
		int x = 0;
		int y = 0;
		int z = 0;
		int w = 0;

		// Every line is a down in the y direction
		for (std::string line; std::getline(source, line);)
		{
			// Every character in the line is right in the x direction
			for (int i = 0; i < (int)line.length(); i++)
			{
				if (line[i] == '#')
				{
					prevActiveCubes.push_back(new Cube(x, y, z, w));
				}

				// Go right
				x++;
			}

			// Go down
			y++;

			// Go back to start
			x = 0;
		}
	}

	int cycles = 0;
	while (cycles != 6)
	{
		// For all active cubes
		for (auto& cube : prevActiveCubes)
		{
			int x = cube->x;
			int y = cube->y;
			int z = cube->z;
			int w = cube->w;

			// Check around current cube
			int activeNeighbours = 0; activeNeighbours = ActiveNeighbours(x, y, z, w);

			// If a cube is active and exactly 2 or 3 of its neighbors are also active, the cube remains active so add it to the future list of active cubes
			if (activeNeighbours == 2 || activeNeighbours == 3)
			{
				currActiveCubes.push_back(cube);
			}
			checkedCubes.push_back(cube);

			// Part 1
			{
				CheckSlice(x, y, z, w);
				CheckSlice(x, y, z + 1, w);
				CheckSlice(x, y, z - 1, w);
			}

			// Part 2
			{
				CheckSlice(x, y, z, w);
				CheckSlice(x, y, z + 1, w);
				CheckSlice(x, y, z - 1, w);

				CheckSlice(x, y, z, w + 1);
				CheckSlice(x, y, z + 1, w + 1);
				CheckSlice(x, y, z - 1, w + 1);


				CheckSlice(x, y, z, w - 1);
				CheckSlice(x, y, z + 1, w - 1);
				CheckSlice(x, y, z - 1, w - 1);
			}
		}

		// Update our active cubes for next cycle and reset all the cubes we checked
		prevActiveCubes = currActiveCubes;
		currActiveCubes.clear();
		checkedCubes.clear();
		cycles++;
	}

	printf("Active Cubes = %d\n", prevActiveCubes.size());

}