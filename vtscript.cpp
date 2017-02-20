#include <iostream>
#include "interpreter.hpp"
#include <fstream>

using std::cout;

int main(int argc, char*argv[])
{
	if (argc < 2) 
	{
		cout << "No input file selected" << std::endl;
		return EXIT_FAILURE;
	}
	else if (argc > 2)
	{
		cout << "Too many arguments. Please pass in a single file to read from." << std::endl;
		return EXIT_FAILURE;
	}
	else
	{
		std::ifstream in(argv[1]);
		if (!in.is_open()) 
		{
			cout << "The input file cout not be opened" << std::endl;
			return EXIT_FAILURE;
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
				return EXIT_SUCCESS;
			}
			if (output.Node.type == Bool)
				cout << "(" << output.Node.bool_value << ")" << std::endl;
			else if (output.Node.type == Value)
				cout << "(" << output.Node.double_value << ")" << std::endl;
			in.close();
			return EXIT_SUCCESS;
		}
	}
}