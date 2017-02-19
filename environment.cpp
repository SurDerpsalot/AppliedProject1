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

Expression Environment::EnvNot(Expression Top) 
{
	Expression Temp;
	Expression Done;
	if (Top.Node.Branch.size() == 1)
	{
		Temp = Top.Node.Branch.at(0);
		if (Temp.Node.type == Symbol)
			Temp = ProType(Temp);

		if (Temp.Node.type == Bool)
		{
			Done = Temp;
			if (Temp.Node.bool_value)
				Done.Node.bool_value = false;
			else
				Done.Node.bool_value = true;
			return Done;
		}
		else if (Temp.Node.type == Value)
		{
			InterpreterSemanticError("Error: Values cannot be logically negated");
			return Error;
		}
	}
	else
	{
		InterpreterSemanticError("Error: Too many/too few arguments for 'not' procedure");
		return Error;
	}
}

Expression Environment::EnvMinus(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() == 1)
	{
		Ex1 = Top.Node.Branch.at(0);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex1.Node.type == Bool)
		{
			InterpreterSemanticError("Error: Cannot make a bool negative in this procedure");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			done = Ex1;
			done.Node.double_value = 1 - done.Node.double_value;
			return done;
		}
	}
	else if (Top.Node.Branch.size() == 2)
	{
		Ex1 = Top.Node.Branch.at(0);
		Ex2 = Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			InterpreterSemanticError("Error: Cannot subtract bools");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				InterpreterSemanticError("Error: Cannot subtract bools");
				return Error;
			}
			else if (Ex2.Node.type == Value)
			{
				done = Ex1.Node.double_value - Ex2.Node.double_value;
				return done;
			}
		}
	}

}

Expression Environment::EnvMult(Expression Top)
{
	Expression Ex1;
	Expression Product(1.0);
	for (size_t i = 0; i < Top.Node.Branch.size(); i++)
	{
		Ex1 = Top.Node.Branch.at(i);
		if (Ex1.Node.type == Symbol)
		{
			Ex1 = ProType(Ex1);
		}
		if (Ex1.Node.type == Bool)
		{
			InterpreterSemanticError("Error: cannot multiply a Bool");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			Product.Node.double_value = Product.Node.double_value * Ex1.Node.double_value;
		}
	}
	return Product;
}

Expression Environment::EnvAnd(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression Final(true);
	if (Top.Node.Branch.size() < 2)
	{
		InterpreterSemanticError("Error: To few arguments. Must have a minimum of two");
		return Error;
	}
	Ex1 = Top.Node.Branch.at(0);
	for (size_t i = 1; i < Top.Node.Branch.size(); i++)
	{
		Ex2 = Top.Node.Branch.at(i);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Value)
		{
			InterpreterSemanticError("Error: Doesn't accept number arguments");
			return Error;
		}
		else if (Ex1.Node.type == Bool)
		{
			if (Ex2.Node.type == Value)
			{
				InterpreterSemanticError("Error: Doesn't accept number arguments");
				return Error;
			}
			else if (Ex2.Node.type == Bool)
				Final.Node.bool_value = Ex2.Node.bool_value && Ex2.Node.bool_value && Final.Node.bool_value;
		}
	}
}

Expression Environment::EnvOr(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression Final(true);
	if (Top.Node.Branch.size() < 2)
	{
		InterpreterSemanticError("Error: To few arguments. Must have a minimum of two");
		return Error;
	}
	Ex1 = Top.Node.Branch.at(0);
	for (size_t i = 1; i < Top.Node.Branch.size(); i++)
	{
		Ex2 = Top.Node.Branch.at(i);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Value)
		{
			InterpreterSemanticError("Error: Doesn't accept number arguments");
			return Error;
		}
		else if (Ex1.Node.type == Bool)
		{
			if (Ex2.Node.type == Value)
			{
				InterpreterSemanticError("Error: Doesn't accept number arguments");
				return Error;
			}
			else if (Ex2.Node.type == Bool)
				Final.Node.bool_value = Ex2.Node.bool_value || Ex2.Node.bool_value || Final.Node.bool_value;
		}
	}
}

Expression Environment::EnvAdd(Expression Top)
{
	Expression Ex1;
	Expression Sum(0);
	for (size_t i = 0; i < Top.Node.Branch.size(); i++)
	{
		Ex1 = Top.Node.Branch.at(i);
		if (Ex1.Node.type == Symbol)
		{
			Ex1 = ProType(Ex1);
		}
		if (Ex1.Node.type == Bool)
		{
			InterpreterSemanticError("Error: cannot add a Bool");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			Sum.Node.double_value = Sum.Node.double_value + Ex1.Node.double_value;
		}
	}
	return Sum;
}

Expression Environment::EnvLes(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() < 2)
	{
		InterpreterSemanticError("Error: To few Arguments");
		return Error;
	}
	else if (Top.Node.Branch.size() >2)
	{
		InterpreterSemanticError("Error: To many Arugments");
		return Error;
	}
	else
	{
		Ex1 = Top.Node.Branch.at(0);
		Ex2 = Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
				return Error;
			}
			else if (Ex2.Node.type == Value)
			{
				if (Ex1.Node.double_value < Ex2.Node.double_value)
				{
					done.Node.bool_value = true;
					done.Node.type = Bool;
				}
				else
				{
					done.Node.bool_value = false;
					done.Node.type = Bool;
				}
				return done;
			}
		}
	}
}

Expression Environment::EnvLeq(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() < 2)
	{
		InterpreterSemanticError("Error: To few Arguments");
		return Error;
	}
	else if (Top.Node.Branch.size() >2)
	{
		InterpreterSemanticError("Error: To many Arugments");
		return Error;
	}
	else
	{
		Ex1 = Top.Node.Branch.at(0);
		Ex2 = Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
				return Error;
			}
			else if (Ex2.Node.type == Value)
			{
				if (Ex1.Node.double_value < Ex2.Node.double_value)
				{
					done.Node.bool_value = true;
					done.Node.type = Bool;
				}
				else if (Ex1.Node.double_value == Ex2.Node.double_value)
				{
					done.Node.bool_value = true;
					done.Node.type = Bool;
				}
				else
				{
					done.Node.bool_value = false;
					done.Node.type = Bool;
				}
				return done;
			}
		}
	}
}

Expression Environment::EnvGrt(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() < 2)
	{
		InterpreterSemanticError("Error: To few Arguments");
		return Error;
	}
	else if (Top.Node.Branch.size() >2)
	{
		InterpreterSemanticError("Error: To many Arugments");
		return Error;
	}
	else
	{
		Ex1 = Top.Node.Branch.at(0);
		Ex2 = Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
				return Error;
			}
			else if (Ex2.Node.type == Value)
			{
				if (Ex1.Node.double_value < Ex2.Node.double_value)
				{
					done.Node.bool_value = false;
					done.Node.type = Bool;
				}
				else
				{
					done.Node.bool_value = true;
					done.Node.type = Bool;
				}
				return done;
			}
		}
	}
}

Expression Environment::EnvGeq(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() < 2)
	{
		InterpreterSemanticError("Error: To few Arguments");
		return Error;
	}
	else if (Top.Node.Branch.size() >2)
	{
		InterpreterSemanticError("Error: To many Arugments");
		return Error;
	}
	else
	{
		Ex1 = Top.Node.Branch.at(0);
		Ex2 = Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
				return Error;
			}
			else if (Ex2.Node.type == Value)
			{
				if (Ex1.Node.double_value > Ex2.Node.double_value)
				{
					done.Node.bool_value = true;
					done.Node.type = Bool;
				}
				else if (Ex1.Node.double_value == Ex2.Node.double_value)
				{
					done.Node.bool_value = true;
					done.Node.type = Bool;
				}
				else
				{
					done.Node.bool_value = false;
					done.Node.type = Bool;
				}
				return done;
			}
		}
	}
}

Expression Environment::EnvEq(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() < 2)
	{
		InterpreterSemanticError("Error: To few Arguments");
		return Error;
	}
	else if (Top.Node.Branch.size() >2)
	{
		InterpreterSemanticError("Error: To many Arugments");
		return Error;
	}
	else
	{
		Ex1 = Top.Node.Branch.at(0);
		Ex2 = Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
				return Error;
			}
			else if (Ex2.Node.type == Value)
			{
				else if (Ex1.Node.double_value == Ex2.Node.double_value)
				{
					done.Node.bool_value = true;
					done.Node.type = Bool;
				}
				else
				{
					done.Node.bool_value = false;
					done.Node.type = Bool;
				}
				return done;
			}
		}
	}
}

Expression Environment::EnvDiv(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() == 2)
	{
		Ex1 = Top.Node.Branch.at(0);
		Ex2 = Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			InterpreterSemanticError("Error: Cannot divide bools");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				InterpreterSemanticError("Error: Cannot divide bools");
				return Error;
			}
			else if (Ex2.Node.type == Value)
			{
				done = Ex1.Node.double_value - Ex2.Node.double_value;
				return done;
			}
		}
	}
	else
	{
		InterpreterSemanticError("Error: Too few/Too many arguments");
		return Error;
	}
}