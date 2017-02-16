#include <iostream>
#include "interpreter.hpp"
//#include "expression.hpp"
#include "tokenize.hpp"

Interpreter::Interpreter()
{
	Root = NULL;
}

bool Interpreter::parse(std::istream & input) noexcept
{
	token UnformattedLine;
	token FormattedLine;
	std::vector<token> ParsedData;
	getline(input, UnformattedLine);
	FormattedLine = StringFormat(UnformattedLine);
	ParsedData = StringSplit(FormattedLine);
	if (ParsedData.size() == 0)
		return false;
	int ParaCount = 0;
	for (size_t i = 0; i < ParsedData.size(); i++)
	{
		if (ParsedData.at(i) == "(")
		{
			ParaCount++;
			if (ParsedData.at(i + 1) == ")" || ParsedData.at(i + 1) == "(")
				return false;
		}
		else if (ParsedData.at(i) == ")")
			ParaCount--;
	}
	if (ParaCount != 0)
		return false;
	BuildTree(ParsedData);
	return true;
}

void traversePost(Node * curLevel) {
	if (curLevel == nullptr) {
		return;
	}
	else {
		//this output being here is pre-order
		for (size_t childIndex = 0; childIndex < curLevel->Branch.size(); childIndex++) {
			traversePost(curLevel->Branch[childIndex]);
		}
		std::cout << curLevel->Data << std::endl;//if the output is being made here, it is post-order
	}
}

void Interpreter::BuildTree(std::vector<token> ParsedData)
{
	if (Root == NULL)
		Root = new Node;
	Node* NewNode;
	for (size_t i = 0; i < ParsedData.size(); i++)
	{
		if (ParsedData.at(i) != "(" || ParsedData.at(i) != ")")
		{
			if (ParsedData.at(i) == "begin")
			{
				Root->Data = ParsedData.at(i);
			}
			else
			{
				NewNode = new Node;
				NewNode->Data = ParsedData.at(i);
				Root->Branch.push_back(NewNode);
			}
		}
	}
	traversePost(Root);
}