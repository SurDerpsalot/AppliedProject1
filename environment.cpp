#include "environment.hpp"

void Environment::EnviroBuild(Node * Top)
{

}

Environment::Environment() 
{

}

Expression Environment::EnviroAccess(Node* Top)
{
	Node* temp;
	Expression held;
	if (Top->Data == "begin")
	{
		for (int childIndex; childIndex < Top->Branch.size(); childIndex++)
		{
			EnviroAccess(Top->Branch[childIndex]);
		}
	}
	else if (Top->Data == "define")
	{
		for (int childIndex; childIndex < Top->Branch.size(); childIndex++)
		{
			temp = Top->Branch[childIndex];
			if ((temp->Data == "define") || (temp->Data == "if") || (temp->Data == "begin"))
				held = EnviroAccess(Top->Branch[childIndex]);
		}
	}
	else if (Top->Data == "if")
	{
		for (int childIndex; childIndex < Top->Branch.size(); childIndex++)
		{
			temp = Top->Branch[childIndex];
			if ((temp->Data == "define") || (temp->Data == "if") || (temp->Data == "begin"))
				EnviroAccess(Top->Branch[childIndex]);
			else
			{
				//do the if action
			}
		}
	}
}
