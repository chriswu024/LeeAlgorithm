#pragma once
class Node
{
private:
	unsigned int weight;
	bool Start, Finish, Path, Obstacle;

public:
	Node();
	~Node(void);

	void setWeight(int new_Weight);
	void setStart(bool isStart);
	void setFinish(bool isFinish);
	void setObstacle(bool isObstacle);
	void setPath(bool isPath);

	int getWeight();
	bool getStart();
	bool getFinish();
	bool getObstacle();
	bool getPath();
};

