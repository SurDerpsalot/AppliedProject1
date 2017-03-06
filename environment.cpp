#include "environment.hpp"

Environment::Environment() 
{
	std::map<std::string, Expression> Dictionary;
	double store;
	store = atan2(0, -1);
	Expression pie(store);
	std::string pi;
	pi = "pi";
	Dictionary.emplace(pi, pie);
	EnviroBuild();
}

Expression Environment::Operations(Expression  Top)
{
	Expression temp;
	if (Top.Node.type == Symbol)
	{
		try {
			temp = ProType(Top);
		}
		catch (InterpreterSemanticError & ERR)
		{
			throw ERR;
		}
	}
	else if (Top.Node.type == Bool)
		temp = Top;
	else if (Top.Node.type == Value)
		temp = Top;
	return temp;
}

Expression Environment::ProType(Expression  Top)
{
	Expression temp;
	if (Top.Node.string_value == "begin")
	{
		for (size_t i = 0; i < Top.Node.Branch.size(); i++)
		{
			try 
			{
				temp = Operations(*Top.Node.Branch[i]);
			}
			catch (InterpreterSemanticError & ERR)
			{
				throw ERR;
			}
		}
	}
	else if (Top.Node.string_value == "define")
	{
		try {
			temp = EnvDef(Top);
		}
		catch (InterpreterSemanticError & ERR)
		{
			throw ERR;
		}
	}
	else if (Top.Node.string_value == "if")
	{
		try{
		temp = EnvIf(Top);
		}
		catch (InterpreterSemanticError & ERR)
		{
			throw ERR;
		}
	}
	else
	{
		try {
			temp = NonSpec(Top);
		}
		catch (InterpreterSemanticError & ERR)
		{
			throw ERR;
		}
	}
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
	bool output;
	if ((Target.Node.string_value == "begin") || (Target.Node.string_value == "define") || (Target.Node.string_value == "if"))
		output = true;
	else if ((Target.Node.string_value == "not") || (Target.Node.string_value == "and") || (Target.Node.string_value == "or"))
		output = true;
	else if ((Target.Node.string_value == "<") || (Target.Node.string_value == "<=") || (Target.Node.string_value == ">"))
		output = true;
	else if ((Target.Node.string_value == ">=") || (Target.Node.string_value == "=") || (Target.Node.string_value == "+"))
		output = true;
	else if ((Target.Node.string_value == "-") || (Target.Node.string_value == "*") || (Target.Node.string_value == "/"))
		output = true;
	else if ((Target.Node.string_value == "pi"))
		output = true;
	else
		output = false;
	return output;
}

Expression Environment::EnvDef(Expression Top)
{
	Expression target;
	Expression value;
	Expression temp;
	if (Top.Node.Branch.size() == 2)
	{
		target = *Top.Node.Branch.at(0);
		value = *Top.Node.Branch.at(1);
		if (target.Node.type == Symbol)
		{
			if (!checkProcedure(target))
			{
				if (!checkProcedure(value))
				{
					temp = value;
					if (value.Node.type == Symbol)
					{
						found = Dictionary.find(value.Node.string_value);
						if (found != Dictionary.end())
							temp = Dictionary[value.Node.string_value];
						else
						{
							throw InterpreterSemanticError("Error: Symbol not found in dictionary");
							temp = Error;
						}
					}
					found = Dictionary.find(target.Node.string_value);
					if (found != Dictionary.end())
						Dictionary[target.Node.string_value] = temp;
					else
						Dictionary.emplace(target.Node.string_value, temp);
				}
				else
				{
					temp = ProType(value);
					Dictionary.emplace(target.Node.string_value, temp);
				}
			}
			else
			{
				throw InterpreterSemanticError("Error: Attempting to assign value to a procedure");
				temp = Error;
			}
		}
		else
		{
			throw InterpreterSemanticError("Error: 'Define' cannot store into a non-symbol");
			temp = Error;
		}
	}
	else
	{
		throw InterpreterSemanticError("Error: Not enough arguments in 'Define'");
		temp = Error;
	}
	return temp;
}

Expression Environment::EnvIf(Expression Top)
{
	Expression boolExpress;
	Expression ex1;
	Expression ex2;
	Expression temp;
	Expression temp2;
	std::map<std::string, Expression>::iterator found;
	if (Top.Node.Branch.size() == 3)
	{
		boolExpress = *Top.Node.Branch.at(0);
		ex1 = *Top.Node.Branch.at(1);
		ex2 = *Top.Node.Branch.at(2);
		if (!checkProcedure(boolExpress))
		{
			if (boolExpress.Node.type == Bool)
			{
				if (boolExpress.Node.bool_value)
				{
					if (!checkProcedure(ex1))
					{
						if (ex1.Node.type == Symbol)
						{
							found = Dictionary.find(ex1.Node.string_value);
							if (found != Dictionary.end())
								temp2 = Dictionary.at(ex1.Node.string_value);
							else
							{
								throw InterpreterSemanticError("Error: Symbol does not exist");
								temp2 = Error;
							}
						}
						else
							temp2 = ex1;
					}
					else
					{
						temp2 = ProType(ex1);
					}
				}
				else
				{
					if (!checkProcedure(ex2))
					{
						if (ex2.Node.type == Symbol)
						{
							found = Dictionary.find(ex2.Node.string_value);
							if (found != Dictionary.end())
								temp2 = Dictionary.at(ex2.Node.string_value);
							else
							{
								throw InterpreterSemanticError("Error: Symbol does not exist");
								temp2 = Error;
							}
						}
						else
							temp2 = ex2;
					}
					else
					{
						temp2 = ProType(ex2);
					}
				}
			}
			else
			{
				throw InterpreterSemanticError("Error: expression1 does not result in a Boolean Atom");
				temp2 = Error;
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
					{
						if (ex1.Node.type == Symbol)
						{
							found = Dictionary.find(ex1.Node.string_value);
							if (found != Dictionary.end())
								temp2 = Dictionary.at(ex1.Node.string_value);
							else
							{
								throw InterpreterSemanticError("Error: Symbol does not exist");
								temp2 = Error;
							}
						}
						else
							temp2 = ex1;
					}
					else
					{
						temp2 = ProType(ex1);
					}
				}
				else
				{
					if (!checkProcedure(ex2))
					{
						if (ex2.Node.type == Symbol)
						{
							found = Dictionary.find(ex2.Node.string_value);
							if (found != Dictionary.end())
								temp2 = Dictionary.at(ex2.Node.string_value);
							else
							{
								throw InterpreterSemanticError("Error: Symbol does not exist");
								temp2 = Error;
							}
						}
						else
							temp2 = ex2;
					}
					else
					{
						temp2 = ProType(ex2);
					}
				}
			}
			else
			{
				throw InterpreterSemanticError("Error: expression1 does not resulf in a Boolean Atom");
				temp2 = Error;
			}
		}
	}
	return temp2;
}

Expression Environment::NonSpec(Expression Top)
{
	Expression temp;
	if (!checkProcedure(Top))
	{
		found = Dictionary.find(Top.Node.string_value);
		if(found != Dictionary.end())
			temp = Dictionary.at(Top.Node.string_value);
		else
		{
			throw InterpreterSemanticError("Error: Symbol not defined");
			temp = Error;
		}
	}
	else 
	{
		if (Top.Node.string_value == "pi")
		{
			Expression pie(atan2(0, -1));
			temp = pie;
		}
		else
		{
			pro = Express.find(Top.Node.string_value);
			auto mem = pro->second;
			try {
				temp = (this->*mem)(Top);
			}
			catch (InterpreterSemanticError & ERR)
			{
				throw ERR;
			}
		}
	}
	return temp;
}

Expression Environment::EnvNot(Expression Top) 
{
	Expression Temp;
	Expression Done;
	if (Top.Node.Branch.size() == 1)
	{
		Temp = *Top.Node.Branch.at(0);
		if (Temp.Node.type == Symbol)
			Temp = ProType(Temp);

		if (Temp.Node.type == Bool)
		{
			Done = Temp;
			if (Temp.Node.bool_value)
				Done.Node.bool_value = false;
			else
				Done.Node.bool_value = true;
		}
		else if (Temp.Node.type == Value)
		{
			throw InterpreterSemanticError("Error: Values cannot be logically negated");
			Done = Error;
		}
	}
	else
	{
		throw InterpreterSemanticError("Error: Too many/too few arguments for 'not' procedure");
		Done = Error;
	}
	return Done;
}

Expression Environment::EnvMinus(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() == 1)
	{
		Ex1 = *Top.Node.Branch.at(0);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex1.Node.type == Bool)
		{
			throw InterpreterSemanticError("Error: Cannot make a bool negative in this procedure");
			done = Error;
		}
		else if (Ex1.Node.type == Value)
		{
			done = Ex1;
			done.Node.double_value = 0 - done.Node.double_value;
		}
	}
	else if (Top.Node.Branch.size() == 2)
	{
		Ex1 = *Top.Node.Branch.at(0);
		Ex2 = *Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			throw InterpreterSemanticError("Error: Cannot subtract bools");
			done = Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				throw InterpreterSemanticError("Error: Cannot subtract bools");
				done =  Error;
			}
			else if (Ex2.Node.type == Value)
			{
				done = Ex1.Node.double_value - Ex2.Node.double_value;
			}
		}
	}
	else
	{
		throw InterpreterSemanticError("Error: Too many arguments");
		done = Error;
	}
	return done;
}

Expression Environment::EnvMult(Expression Top)
{
	Expression Ex1;
	Expression Product(1.0);
	for (size_t i = 0; i < Top.Node.Branch.size(); i++)
	{
		Ex1 = *Top.Node.Branch.at(i);
		if (Ex1.Node.type == Symbol)
		{
			Ex1 = ProType(Ex1);
		}
		if (Ex1.Node.type == Bool)
		{
			throw InterpreterSemanticError("Error: cannot multiply a Bool");
			Product = Error;
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
		throw InterpreterSemanticError("Error: To few arguments. Must have a minimum of two");
		Final = Error;
	}
	Ex1 = *Top.Node.Branch.at(0);
	for (size_t i = 1; i < Top.Node.Branch.size(); i++)
	{
		Ex2 = *Top.Node.Branch.at(i);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Value)
		{
			throw InterpreterSemanticError("Error: Doesn't accept number arguments");
			Final = Error;
		}
		else if (Ex1.Node.type == Bool)
		{
			if (Ex2.Node.type == Value)
			{
				throw InterpreterSemanticError("Error: Doesn't accept number arguments");
				Final = Error;
			}
			else if (Ex2.Node.type == Bool)
				Final.Node.bool_value = Ex2.Node.bool_value && Ex2.Node.bool_value && Final.Node.bool_value;
		}
	}
	return Final;
}

Expression Environment::EnvOr(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression Final(true);
	if (Top.Node.Branch.size() < 2)
	{
		throw InterpreterSemanticError("Error: To few arguments. Must have a minimum of two");
		return Error;
	}
	Ex1 = *Top.Node.Branch.at(0);
	for (size_t i = 1; i < Top.Node.Branch.size(); i++)
	{
		Ex2 = *Top.Node.Branch.at(i);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Value)
		{
			throw InterpreterSemanticError("Error: Doesn't accept number arguments");
			return Error;
		}
		else if (Ex1.Node.type == Bool)
		{
			if (Ex2.Node.type == Value)
			{
				throw InterpreterSemanticError("Error: Doesn't accept number arguments");
				return Error;
			}
			else if (Ex2.Node.type == Bool)
				Final.Node.bool_value = Ex2.Node.bool_value || Ex2.Node.bool_value || Final.Node.bool_value;
		}
	}
	return Final;
}

Expression Environment::EnvAdd(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression Zero;
	Ex1.Node.type = Value;
	Ex1.Node.double_value = 0;
		for (size_t i = 0; i < Top.Node.Branch.size(); i++)
		{
			Ex2 = *Top.Node.Branch.at(i);
			if (Ex2.Node.type == Symbol)
			{
				Ex2 = ProType(Ex2);
			}
			if (Ex2.Node.type == Bool)
			{
				throw InterpreterSemanticError("Error: cannot add a Bool");
				return Error;
			}
			else if (Ex2.Node.type == Value)
			{
				Ex1.Node.double_value = Ex2.Node.double_value + Ex1.Node.double_value;
			}
		}
	return Ex1;
}

Expression Environment::EnvLes(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() < 2)
	{
		throw InterpreterSemanticError("Error: To few Arguments");
		return Error;
	}
	else if (Top.Node.Branch.size() >2)
	{
		throw InterpreterSemanticError("Error: To many Arugments");
		return Error;
	}
	else
	{
		Ex1 = *Top.Node.Branch.at(0);
		Ex2 = *Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			throw InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				throw InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
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
	return Error;
}

Expression Environment::EnvLeq(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() < 2)
	{
		throw InterpreterSemanticError("Error: To few Arguments");
		return Error;
	}
	else if (Top.Node.Branch.size() >2)
	{
		throw InterpreterSemanticError("Error: To many Arugments");
		return Error;
	}
	else
	{
		Ex1 = *Top.Node.Branch.at(0);
		Ex2 = *Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			throw InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				throw InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
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
	return Error;
}

Expression Environment::EnvGrt(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() < 2)
	{
		throw InterpreterSemanticError("Error: To few Arguments");
		return Error;
	}
	else if (Top.Node.Branch.size() >2)
	{
		throw InterpreterSemanticError("Error: To many Arugments");
		return Error;
	}
	else
	{
		Ex1 = *Top.Node.Branch.at(0);
		Ex2 = *Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			throw InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				throw InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
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
	return Error;
}

Expression Environment::EnvGeq(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() < 2)
	{
		throw InterpreterSemanticError("Error: To few Arguments");
		return Error;
	}
	else if (Top.Node.Branch.size() >2)
	{
		throw InterpreterSemanticError("Error: To many Arugments");
		return Error;
	}
	else
	{
		Ex1 = *Top.Node.Branch.at(0);
		Ex2 = *Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			throw InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				throw InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
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
	return Error;
}

Expression Environment::EnvEq(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() < 2)
	{
		throw InterpreterSemanticError("Error: To few Arguments");
		return Error;
	}
	else if (Top.Node.Branch.size() >2)
	{
		throw InterpreterSemanticError("Error: To many Arugments");
		return Error;
	}
	else
	{
		Ex1 = *Top.Node.Branch.at(0);
		Ex2 = *Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			throw InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				throw InterpreterSemanticError("Error: Cannot accept Boolean Arguments");
				return Error;
			}
			else if (Ex2.Node.type == Value)
			{
				if (Ex1.Node.double_value == Ex2.Node.double_value)
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
	return Error;
}

Expression Environment::EnvDiv(Expression Top)
{
	Expression Ex1;
	Expression Ex2;
	Expression done;
	if (Top.Node.Branch.size() == 2)
	{
		Ex1 = *Top.Node.Branch.at(0);
		Ex2 = *Top.Node.Branch.at(1);
		if (Ex1.Node.type == Symbol)
			Ex1 = ProType(Ex1);
		if (Ex2.Node.type == Symbol)
			Ex2 = ProType(Ex2);
		if (Ex1.Node.type == Bool)
		{
			throw InterpreterSemanticError("Error: Cannot divide bools");
			return Error;
		}
		else if (Ex1.Node.type == Value)
		{
			if (Ex2.Node.type == Bool)
			{
				throw InterpreterSemanticError("Error: Cannot divide bools");
				return Error;
			}
			else if (Ex2.Node.type == Value)
			{
				done = Ex1.Node.double_value / Ex2.Node.double_value;
				return done;
			}
		}
	}
	else
	{
		throw InterpreterSemanticError("Error: Too few/Too many arguments");
		return Error;
	}
	return Error;
}