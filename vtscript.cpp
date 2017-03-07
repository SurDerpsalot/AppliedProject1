#include <iostream>
#include "interpreter.hpp"
#include <fstream>

using std::cout;
using std::cin;

int main(int argc, char*argv[])
{
	int Output;
	Expression Error("Error");
	if (argc == 1) 
	{
		Interpreter Interp;
		std::string input;
		Expression output;
		while (1)
		{
			cout << "vtscript> ";
			getline(cin, input);
			std::istringstream TokenStream(input);
			Interp.parse(TokenStream);
			try {
				output = Interp.eval();
				if (output.Node.type == Bool)
					if(output.Node.bool_value)
						cout << "(True)" << std::endl;
					else
						cout << "(False)" << std::endl;
				else if (output.Node.type == Value)
					cout << "(" << output.Node.double_value << ")" << std::endl;
			}
			catch (InterpreterSemanticError & ERR)
			{
				output = Error;
				cout << "Error" << std::endl;
				Interp.resetEnviro();
			}
			Output = EXIT_SUCCESS;
		}
	}
	else if (argc == 3)
	{
		std::string input = argv[2];
		Interpreter Interp;
		Expression output;
		std::istringstream TokenStream(input);
		Interp.parse(TokenStream);
		try {
			output = Interp.eval();
		}
		catch (InterpreterSemanticError & ERR)
		{
			cout << "Error" << std::endl;
			Output = EXIT_FAILURE;
			return Output;
		}
		if (output.Node.type == Bool)
			cout << "(" << output.Node.bool_value << ")" << std::endl;
		else if (output.Node.type == Value)
			cout << "(" << output.Node.double_value << ")" << std::endl;
		Output = EXIT_SUCCESS;
	}
	else if(argc == 2)
	{
		std::ifstream in(argv[1]);
		if (!in.is_open()) 
		{
			cout << "Error" << std::endl;
			Output = EXIT_FAILURE;
		}
		else
		{
			Interpreter Interp;
			Expression output;
			Interp.parse(in);
			try { 
				output = Interp.eval();
			}
			catch (InterpreterSemanticError & ERR)
			{
				cout << "Error" << std::endl;
				Output = EXIT_FAILURE;
			}
			if (output.Node.type == Bool)
				cout << "(" << output.Node.bool_value << ")" << std::endl;
			else if (output.Node.type == Value)
				cout << "(" << output.Node.double_value << ")" << std::endl;
			Output = EXIT_SUCCESS;
			in.close();
		}
	}
	return Output;
}