#include "environment.hpp"

Environment::Environment() 
{
	std::map<std::string, double> Dictionary;
	double store;
	store = atan2(0, -1);
	std::string pi;
	pi = "pi";
	Dictionary.emplace(pi, store);
	EnviroBuild();
}

Expression Environment::Operations(Expression * Top)
{
	Expression temp;
	if (Top->Node.type == Symbol)
		temp = ProType(Top);
	else if (Top->Node.type == Bool)
		return Top;
	else if (Top->Node.type == Value)
		return Top;
	return temp;
}

Expression Environment::ProType(Expression * Top)
{
	Expression temp;
	if (Top->Node.string_value == "begin")
	{
		for (size_t i = 0; i < Top->Node.Branch.size(); i++)
			temp = Operations(Top->Node.Branch[i]);
	}
	else if (Top->Node.string_value == "define")
		temp = EnvDef(Top);
	else if (Top->Node.string_value == "if")
		temp = EnvIf(Top);
	else
		temp = NonSpec(Top);
	return temp;
}

Expression Environment::EnviroAccess(Expression* Top, size_t childIndex)
{
	Expression* temp;
	Expression held;
	if (Top->Node.string_value == "begin")
	{
		for (childIndex; childIndex < Top->Node.Branch.size(); childIndex++)
		{
			EnviroAccess(Top->Node.Branch[childIndex],childIndex);
		}
	}
	else if (Top->Node.string_value == "define")
	{
		for (childIndex; childIndex < Top->Node.Branch.size(); childIndex++)
		{
			temp = Top->Node.Branch[childIndex];
			if ((temp->Node.string_value == "define") || (temp->Node.string_value == "if") || (temp->Node.string_value == "begin"))
				held = EnviroAccess(Top->Node.Branch[childIndex],childIndex);
		}
	}
	else if (Top->Node.string_value == "if")
	{
		for (childIndex; childIndex < Top->Node.Branch.size(); childIndex++)
		{
			temp = Top->Node.Branch[childIndex];
			if ((temp->Node.string_value == "define") || (temp->Node.string_value == "if") || (temp->Node.string_value == "begin"))
				EnviroAccess(Top->Node.Branch[childIndex],childIndex);
			else
			{
				//do the if action
			}
		}
	}
	return true;
}

void Environment::EnviroBuild()
{
	Express.emplace("-", &Environment::EnvMinus);
	Express.emplace("not", &Environment::EnvNot);
	Express.emplace("and", &Environment::EnvAnd);
	Express.emplace("or", &Environment::EnvOr);
	Express.emplace("<", &Environment::EnvLes);
	Express.emplace("<=", &Environment::EnvLeq);
	Express.emplace(">", &Environment::EnvGrt);
	Express.emplace(">=", &Environment::EnvGeq);
	Express.emplace("=", &Environment::EnvEq);
	Express.emplace("+", &Environment::EnvAdd);
	Express.emplace("*", &Environment::EnvMult);
	Express.emplace("/", &Environment::EnvDiv);
}