#include "environment.hpp"

Environment::Environment() 
{
	std::map<std::string, Expression> Dictionary;
	double store;
	store = atan2(0, -1);
	std::string pi;
	pi = "pi";
	Dictionary.emplace(pi, store);
	EnviroBuild();
}

Expression Environment::Operations(Expression  Top)
{
	Expression temp;
	if (Top.Node.type == Symbol)
		temp = ProType(Top);
	else if (Top.Node.type == Bool)
		return Top;
	else if (Top.Node.type == Value)
		return Top;
	return temp;
}

Expression Environment::ProType(Expression  Top)
{
	Expression temp;
	if (Top.Node.string_value == "begin")
	{
		for (size_t i = 0; i < Top.Node.Branch.size(); i++)
			temp = Operations(Top.Node.Branch[i]);
	}
	else if (Top.Node.string_value == "define")
		temp = EnvDef(Top);
	else if (Top.Node.string_value == "if")
		temp = EnvIf(Top);
	else
		temp = NonSpec(Top);
	return temp;
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

bool Environment::checkProcedure(Expression Target)
{
	if ((Target.Node.string_value == "begin") || (Target.Node.string_value == "define") || (Target.Node.string_value == "if"))
		return true;
	else if ((Target.Node.string_value == "not") || (Target.Node.string_value == "and") || (Target.Node.string_value == "or"))
		return true;
	else if ((Target.Node.string_value == "<") || (Target.Node.string_value == "<=") || (Target.Node.string_value == ">"))
		return true;
	else if ((Target.Node.string_value == ">=") || (Target.Node.string_value == "=") || (Target.Node.string_value == "+"))
		return true;
	else if ((Target.Node.string_value == "-") || (Target.Node.string_value == "*") || (Target.Node.string_value == "/"))
		return true;
	else if ((Target.Node.string_value == "pi"))
		return true;
	else
		return false;
}

Expression Environment::EnvDef(Expression Top)
{
	Expression target;
	Expression value;
	Expression temp;
	if (Top.Node.Branch.size() == 2)
	{
		target = Top.Node.Branch.at(0);
		value = Top.Node.Branch.at(1);
		if (target.Node.type == Symbol)
		{
			if (!checkProcedure(target))
			{
				if (!checkProcedure(value))
				{
					Dictionary.emplace(target.Node.string_value, value);
					return value;
				}
				else
				{
					temp = ProType(value);
					Dictionary.emplace(target.Node.string_value, temp);
					return temp;
				}
			}
			else
			{
				InterpreterSemanticError("Error: Attempting to assign value to a procedure");
				return Error;
			}
		}
		else
		{
			InterpreterSemanticError("Error: 'Define' cannot store into a non-symbol");
			return Error;
		}
	}
	else
	{
		InterpreterSemanticError("Error: Not enough arguments in 'Define'");
		return Error;
	}
}

Expression Environment::EnvIf(Expression Top)
{
	Expression boolExpress;
	Expression ex1;
	Expression ex2;
	Expression temp;
	Expression temp2;
	if (Top.Node.Branch.size() == 3)
	{
		boolExpress = Top.Node.Branch.at(0);
		ex1 = Top.Node.Branch.at(1);
		ex2 = Top.Node.Branch.at(2);
		if (!checkProcedure(boolExpress))
		{
			if (boolExpress.Node.type == Bool)
			{
				if (boolExpress.Node.bool_value)
				{
					if (!checkProcedure(ex1))
						return ex1;
					else
					{
						temp = ProType(ex1);
						return temp;
					}
				}
				else
				{
					if (!checkProcedure(ex2))
						return ex2;
					else
					{
						temp = ProType(ex2);
						return temp;
					}
				}
			}
			else
			{
				InterpreterSemanticError("Error: expression1 does not result in a Boolean Atom");
				return Error;
			}
		}
		else
		{
			temp = ProType(boolExpress);
			if (temp.Node.type == Bool)
			{
				if (temp.Node.bool_value)
				{
					if (!checkProcedure(ex1))
						return ex1;
					else
					{
						temp2 = ProType(ex1);
						return temp2;
					}
				}
				else
				{
					if (!checkProcedure(ex2))
						return ex2;
					else
					{
						temp2 = ProType(ex2);
						return temp2;
					}
				}
			}
			else
			{
				InterpreterSemanticError("Error: expression1 does not resulf in a Boolean Atom");
				return Error;
			}
		}
	}
}

Expression Environment::NonSpec(Expression Top)
{
	Expression temp;
	if (!checkProcedure)
	{
		temp = Dictionary.at(Top.Node.string_value);
		return temp;
	}
	else 
	{
		pro = Express.find(Top.Node.string_value);
		auto mem = pro->second;
		temp = (this->*mem)(Top);
		return temp;
	}
}

Expression EnvNot(Expression Top) 
{

}

Expression EnvMinus(Expression Top) 
{

}

Expression EnvMult(Expression Top)
{

}

Expression EnvAnd(Expression Top)
{

}

Expression EnvAdd(Expression Top)
{

}

Expression EnvOr(Expression Top)
{

}

Expression EnvLes(Expression Top)
{

}

Expression EnvLeq(Expression Top)
{

}

Expression EnvGrt(Expression Top)
{

}

Expression EnvGeq(Expression Top)
{

}

Expression EnvEq(Expression Top)
{

}
Expression EnvDiv(Expression Top)
{

}