#include <iostream>
#include "interpreter.hpp"
#include <fstream>

using std::cout;
using std::cin;

int main(int argc, char*argv[])
{
	int output;
	if (argc == 1) 
	{
		Interpreter Interp;
		while (1)
		{
			cout << "vtscript> ";
			std::string input;
			getline(cin, input);
			Expression output;
			std::istringstream TokenStream(input);
			Interp.parse(TokenStream);
			try {
				output = Interp.eval();
			}
			catch (InterpreterSemanticError & ERR)
			{
				throw ERR;
				output = EXIT_FAILURE;
			}
			if (output.Node.type == Bool)
				cout << "(" << output.Node.bool_value << ")" << std::endl;
			else if (output.Node.type == Value)
				cout << "(" << output.Node.double_value << ")" << std::endl;
			output = EXIT_SUCCESS;
		}
	}
	else if (argc == 3)
	{
		if (argv[1] != "-e")
		{
			cout << "expected a -e as second input after vtscript" << std::endl;
			output = EXIT_FAILURE;
		}
		else
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
				throw ERR;
				output = EXIT_SUCCESS;
			}
			if (output.Node.type == Bool)
				cout << "(" << output.Node.bool_value << ")" << std::endl;
			else if (output.Node.type == Value)
				cout << "(" << output.Node.double_value << ")" << std::endl;
			output = EXIT_SUCCESS;
		}
	}
	else if(argc == 2)
	{
		std::ifstream in(argv[1]);
		if (!in.is_open()) 
		{
			cout << "The input file cout not be opened" << std::endl;
			output = EXIT_FAILURE;
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
				throw ERR;
				output = EXIT_SUCCESS;
			}
			if (output.Node.type == Bool)
				cout << "(" << output.Node.bool_value << ")" << std::endl;
			else if (output.Node.type == Value)
				cout << "(" << output.Node.double_value << ")" << std::endl;
			in.close();
			output = EXIT_SUCCESS;
		}
	}
	return output;
}