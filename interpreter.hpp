#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "tokenize.hpp"
struct Node {
	std::string Data;
	std::vector<Node *> Branch;

};

class Interpreter {
public:
	// Default construct an Interpreter with the default environment and an empty AST
	Interpreter();

	// Given a vtscript program as a std::istream, attempt to parse into an internal AST
	// return true on success, false on failure

	bool parse(std::istream & expression) noexcept;

	// Evaluate the current AST and return the resulting Expression
	// throws InterpreterSemanticError if a semantic error is encountered
	// the exception message string should document the nature of the semantic error 
	//Expression eval();

	bool checkBasicInput(std::vector<std::string> & input);
	bool checkNumInput(std::vector<std::string> & input);

	bool BuildTree(std::vector<std::string> ParsedData, size_t &i, Node * currentLevel);

private:
	Node* Root;
	Node* CurLvl;
};

#endif