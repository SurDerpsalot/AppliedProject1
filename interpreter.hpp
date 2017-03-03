#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "tokenize.hpp"
#include "environment.hpp"


class Interpreter {
public:
	// Default construct an Interpreter with the default environment and an empty AST
	Interpreter();
	void destroyTree(Expression * curLevel);

	// Given a vtscript program as a std::istream, attempt to parse into an internal AST
	// return true on success, false on failure

	bool parse(std::istream & input) noexcept;

	// Evaluate the current AST and return the resulting Expression
	// throws InterpreterSemanticError if a semantic error is encountered
	// the exception message string should document the nature of the semantic error 
	Expression eval();

	bool checkBasicInput(std::vector<std::string> & input);
	bool checkNumInput(std::vector<std::string> & input);

	bool BuildTree(std::vector<std::string> ParsedData, size_t &i, Expression * currentLevel);
	void StoreNum(std::string input, Expression * node);
	void Store(std::string input, Expression * node);
	void StoreSymbol(std::string input, Expression *node);
	void resetEnviro();

private:
	Expression * Root;
	Expression * CurLvl;
	Environment Enviro;
};

#endif