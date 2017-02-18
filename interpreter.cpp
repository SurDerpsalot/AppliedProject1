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
	if (!checkBasicInput(ParsedData))
		return false;
	else
	{
		size_t i = 0;
		BuildTree(ParsedData, i, Root);
		return true;
	}
}

bool Interpreter::checkBasicInput(std::vector<token> & input) 
{
	if (input.size() <=2)
	{
		std::cout << "Encountered Parse Error." << std::endl;
		return false;
	}
	int ParaCount = 0;
	for (size_t i = 0; i < input.size(); i++)
	{
		if (input.at(i) == "(")
		{
			ParaCount++;
			if (input.at(i + 1) == ")" || input.at(i + 1) == "(")
			{
				std::cout << "Encountered Parse Error." << std::endl;
				return false;
			}
		}
		else if (input.at(i) == ")")
			ParaCount--;
	}
	if (ParaCount == 0)
	{
		return checkNumInput(input);
	}
	else
	{
		std::cout << "Encountered Parse Error." << std::endl;
		return false;
	}
}

bool Interpreter::checkNumInput(std::vector<std::string> & input) {
	token number;
	bool num;
	num = false;
	for (size_t i = 0; i < input.size(); i++)
	{
		number = input.at(i);
		for (size_t j = 0; j < number.length(); j++)
		{
			if (num)
			{
				if (!isdigit(number[j]) && number[j] != '.')
					return false;
			}
			if (isdigit(number[j]))
				num = true;
		}
		num = false;
	}
	return true;
}

bool Interpreter::BuildTree(std::vector<token> ParsedData, size_t & i, Expression * currentLevel)
{
	if (Root == NULL)
	{
		Root = new Expression;
		Root->Node.type = Symbol;
		Root->Node.string_value = ParsedData.at(i + 1);
		i = i + 2;
		currentLevel = Root;
	}
	Expression * NewNode;
	for (i; i < ParsedData.size(); i++)
	{
		if (ParsedData.at(i) == "(")
		{
			NewNode = new Expression;
			NewNode->Node.type = Symbol;
			NewNode->Node.string_value = ParsedData.at(i + 1);
			i = i + 2;
			currentLevel->Branch.push_back(NewNode);
			BuildTree(ParsedData, i, NewNode);
		}
		else if (ParsedData.at(i) == ")")
			return true;
		else
		{
			NewNode = new Node;
			NewNode->Data = ParsedData.at(i);
			currentLevel->Branch.push_back(NewNode);
		}
	}
	return true;
}

Expression eval() 
{

}