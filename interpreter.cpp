#include <iostream>
#include "interpreter.hpp"
#include "tokenize.hpp"

Interpreter::Interpreter()
{
	Root = NULL;
}

bool Interpreter::parse(std::istream & input) noexcept
{
	token UnformattedLine = "";
	token FormattedLine = "";
	token read;
	bool out;
	std::vector<token> ParsedData;
	while (getline(input, UnformattedLine))
	{
		read = StringFormat(UnformattedLine);
		FormattedLine = FormattedLine + read;
	}
		ParsedData = StringSplit(FormattedLine);
	if (!checkBasicInput(ParsedData))
		out = false;
	else
	{
		size_t i = 0;
		BuildTree(ParsedData, i, Root);
		out = true;
	}
	return out;
}

bool Interpreter::checkBasicInput(std::vector<token> & input) 
{
	if (input.size() <=2)
	{
		//std::cout << "Error: Encountered a Parse Error" << std::endl;
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
				//std::cout << "Error: Error with Paranthesis placement in statement" << std::endl;
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
		//std::cout << "Error: Too many Paranthesis" << std::endl;
		return false;
	}
}

bool Interpreter::checkNumInput(std::vector<std::string> & input) {
	token number;
	bool sci;
	bool num;
	num = false;
	sci = false;
	for (size_t i = 0; i < input.size(); i++)
	{
		number = input.at(i);
		for (size_t j = 0; j < number.length(); j++)
		{
			if (num)
			{
				if (!isdigit(number[j]) && number[j] != '.' && number[j] != 'e')
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
		if (ParsedData.at(i + 1) == "True")
		{
			Root->Node.type = Bool;
			Root->Node.bool_value = true;
		}
		else if (ParsedData.at(i + 1) == "False")
		{
			Root->Node.type = Bool;
			Root->Node.bool_value = false;
		}
		else
		{
			StoreNum(ParsedData.at(i + 1), Root);
			//Root->Node.type = Symbol;
			//Root->Node.string_value = ParsedData.at(i + 1);
		}
		i = i + 2;
		currentLevel = Root;
	}
	Expression * NewNode;
	for (i; i < ParsedData.size(); i++)
	{
		if (ParsedData.at(i) == "(")
		{
			NewNode = new Expression;
			Store(ParsedData.at(i + 1), NewNode);
			i = i + 2;
			currentLevel->Node.Branch.push_back(NewNode);
			
			BuildTree(ParsedData, i, NewNode);
		}
		else if (ParsedData.at(i) == ")")
			return true;
		else
		{
			NewNode = new Expression;
			Store(ParsedData.at(i), NewNode);
			currentLevel->Node.Branch.push_back(NewNode);
		}
	}
	return true;
}

void Interpreter::Store(std::string input, Expression * node)
{
	if (input == "True")
	{
		node->Node.type = Bool;
		node->Node.bool_value = true;
	}
	else if (input == "False")
	{
		node->Node.type = Bool;
		node->Node.bool_value = false;
	}
	else
		StoreNum(input, node);
}

void Interpreter::StoreNum(std::string input, Expression * node)
{
	bool num = false;
	for (size_t j = 0; j < input.length(); j++)
	{
		if (num)
		{
			if (!isdigit(input[j]) && input[j] != '.')
			{
				num = false;
				break;
			}
		}
		if (isdigit(input[j]))
			num = true;
	}
	if (num)
	{
		node->Node.type = Value;
		node->Node.double_value = std::stod(input);
	}
	else
	{
		StoreSymbol(input, node);
	}	
}

void Interpreter::StoreSymbol(std::string input, Expression *node)
{
	node->Node.type = Symbol;
	node->Node.string_value = input;
}

void Interpreter::destroyTree(Expression* curLevel) {
	if (curLevel != nullptr) {
		for (size_t childIndex = 0; childIndex < curLevel->Node.Branch.size(); childIndex++) 
		{
			destroyTree(curLevel->Node.Branch[childIndex]);
		}
		delete curLevel;
		curLevel = NULL;
	}
	return;
}

void Interpreter::resetEnviro()
{
	Environment newEnviro;
	Enviro = newEnviro;
}

Expression Interpreter::eval() 
{
	Expression result;
	try {
		result = Enviro.Operations(*Root);
		destroyTree(Root);
		Root = NULL;
	}
	catch (InterpreterSemanticError & ERR)
	{
		throw ERR;
	}
	
	return result;
}