#include "Node.h"


Node::Node()
{
	Start = false;
	Finish = false;
	Path = false;
	Obstacle = false;
	weight = 0;
}


Node::~Node(void)
{
}
	
void Node::setWeight(int new_Weight){weight = new_Weight;}
void Node::setStart(bool isStart){Start = isStart;}
void Node::setFinish(bool isFinish){Finish = isFinish;}
void Node::setObstacle(bool isObstacle){Obstacle = isObstacle;}
void Node::setPath(bool isPath){Path = isPath;}

int Node::getWeight(){return weight;}
bool Node::getStart(){return Start;}
bool Node::getFinish(){return Finish;}
bool Node::getObstacle(){return Obstacle;}
bool Node::getPath(){return Path;}
