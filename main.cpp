#include <iostream>
#include <queue>
#include <array>
#include <cstdlib>
#include "Node.h"

//Global Variables
int state = 1;
const int WIDTH = 10;
Node Map[WIDTH][WIDTH];

typedef struct Coords{int x, y;} Coords;
Coords StartCoords;
Coords FinishCoords;
Coords ActiveCoords;
Coords* Obstacle = NULL;
std::vector<Coords*> Obstacles;
std::queue<Coords> working_set;
std::queue<Coords> neighbour_set;
std::queue<Coords> path;
std::vector<Coords> invalid_set;

void setMapStart(int x, int y){Map[y][x].setStart(true);}
void setMapFinish(int x, int y){Map[y][x].setFinish(true);}
void setMapObstacle(int x, int y){Map[y][x].setObstacle(true);}

void printMap()
{
	system("CLS");
	for (int y = 0; y < WIDTH; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (Map[x][y].getStart())
				std::cout << "Start\t";
			else if (Map[x][y].getFinish())
				std::cout << "Goal\t";
			else if (Map[x][y].getObstacle())
				std::cout << "O\t";
			else if (Map[x][y].getPath())
				std::cout << "P\t";
			else
			std::cout << Map[x][y].getWeight() << "\t";
		} 
		std::cout << "\n";
	}
}

void PopulateMap()
{
	setMapStart(StartCoords.y, StartCoords.x);
	working_set.push(StartCoords);

	setMapFinish(FinishCoords.y, FinishCoords.x);

	for (int i = 0; i < 3; i++)
	{
		Obstacle = new Coords;
		Obstacle->y = 2+i;
		Obstacle->x = 1;

		setMapObstacle(Obstacle->y, Obstacle->x);
		Obstacles.push_back(Obstacle);
	}
	Obstacle = NULL;
}

bool CheckInvalid(Coords toCheck)
{
	//True for Valid, False for Invalid node.


	//Check if out of bounds
	if (toCheck.x >= 10)
		return false;
	if (toCheck.y >= 10)
		return false;
	
	if (state != 2)
	{
		//Check for Weight
		if (Map[toCheck.x][toCheck.y].getWeight() != 0)
			return false;
	}

	//Check if Obstacle
	for (int i = 0; i < Obstacles.size(); i++)
	{
		if (toCheck.x == Obstacles[i]->x)
		{
			if (toCheck.y == Obstacles[i]->y)
			{
				return false;
			}
		}
	}

	if (state != 2)
	{
		//Check invalid list
		for (int i = 0; i < invalid_set.size(); i++)
		{
			if (toCheck.x == invalid_set[i].x)
			{
				if (toCheck.y == invalid_set[i].y)
				{
					return false;
				}
			}
		}
	}

	//If the node being checked made it this far it means it's valid so add it to invalid_set and return true.
	invalid_set.push_back(toCheck);
	return true;
}

void FindNeighbours()
{
	Coords Neighbour;

	if ((ActiveCoords.x >= 0) && (ActiveCoords.x <= 9))
	{
		if (ActiveCoords.x > 0) //Has a Left Neighbour
		{
			Neighbour.x = ActiveCoords.x - 1;
			Neighbour.y = ActiveCoords.y;

			if (CheckInvalid(Neighbour))
				neighbour_set.push(Neighbour);
		}

		if (ActiveCoords.x < 9) //Has a Right Neighbour
		{
			Neighbour.x = ActiveCoords.x + 1;
			Neighbour.y = ActiveCoords.y;

			if (CheckInvalid(Neighbour))
				neighbour_set.push(Neighbour);
		}
	}

	if ((ActiveCoords.y >= 0) && (ActiveCoords.y <= 9))
	{
		if (ActiveCoords.y > 0) //Has Top Neighbour
		{
			Neighbour.x = ActiveCoords.x;
			Neighbour.y = ActiveCoords.y + 1;

			if (CheckInvalid(Neighbour))
				neighbour_set.push(Neighbour);
		}

		if (ActiveCoords.y <= 9) //Has a Bottom Neighbour
		{
			Neighbour.x = ActiveCoords.x;
			Neighbour.y = ActiveCoords.y - 1;

			if (CheckInvalid(Neighbour))
				neighbour_set.push(Neighbour);
		}
	}
}

void LeePopulate()
{
	int ActiveWeight = 1;
	while (!Map[FinishCoords.x][FinishCoords.y].getWeight())
	{
		printMap();
		ActiveCoords = working_set.front();
		working_set.pop();

		FindNeighbours();
		Map[ActiveCoords.x][ActiveCoords.y].setWeight(ActiveWeight);
	
		if (working_set.empty())
		{
			ActiveWeight++;
			working_set = neighbour_set;
			while (!neighbour_set.empty())
				neighbour_set.pop();
		}
	}
	state = 2;
}

void LeeBacktrace()
{
	//Some housekeeping so that nothing bad carries over.
	if (!working_set.empty())
	{
		while (!working_set.empty())
			working_set.pop();
	}

	int CurrentWeight = 0;
	Coords Neighbour;

	//Starting from Finish, add all the path nodes leading to goal
	ActiveCoords = FinishCoords;
	CurrentWeight = Map[FinishCoords.x][FinishCoords.y].getWeight();

	while (!((ActiveCoords.x == StartCoords.x) && (ActiveCoords.y == StartCoords.y)))
	{
		FindNeighbours();
		do
		{
			Neighbour = neighbour_set.front();
			neighbour_set.pop();
		}
		while (Map[Neighbour.x][Neighbour.y].getWeight() > CurrentWeight);

		CurrentWeight = Map[Neighbour.x][Neighbour.y].getWeight();
		path.push(Neighbour);
	
		ActiveCoords = Neighbour;

		while(!neighbour_set.empty())
			neighbour_set.pop();
	}
	
	state = 3;		
}

void LeeShowPath()
{
	Coords Pathnode;
	while (!path.empty())
	{
		Pathnode = path.front();
		path.pop();
		Map[Pathnode.x][Pathnode.y].setPath(true);
	}
}

void main()
{
	StartCoords.x = 1;
	StartCoords.y = 5;

	FinishCoords.x = 9;
	FinishCoords.y = 9;

	PopulateMap(); //Initial map population
	printMap();

	while (true)
	{
		switch (state)
		{
		case 1:
			{
				LeePopulate();
			}
		case 2:
			{
				LeeBacktrace();
			}
		case 3:
			{
				LeeShowPath();
				break;
			}
		}
		printMap();
		break;
	}
}