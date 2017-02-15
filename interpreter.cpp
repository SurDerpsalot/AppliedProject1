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

	return true;
}

void Interpreter::InsertNode(Node * curLevel)
{

}

void Interpreter::BuildTree(std::vector<token> ParsedData)
{
	Root = new Node;






}