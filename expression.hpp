#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <vector>
#include <string>
#include <iostream>
enum AtomType { None, Bool, Symbol, Value };

class Expression{
public:

	struct Variant {
		AtomType type;
		bool bool_value;
		double double_value;
		std::string string_value;
		std::vector<Expression* > Branch;
	};
	Variant Node;
	// Default construct an Expression of type None
	Expression();

	// Construct an Expression with a single Boolean atom with value
	Expression(bool value);

	// Construct an Expression with a single Number atom with value
	Expression(double value);
	Expression(int value);

	// Construct an Expression with a single Symbol atom with value
	Expression(const std::string & value);

	// Equality operator for two Expressions, two expressions are equal if the have the same 
	// type, atom value, and number of arguments
	bool operator==(const Expression & exp) const noexcept;
	//bool operator&&(const Expression & exp) const noexcept;
private:

};
#endif