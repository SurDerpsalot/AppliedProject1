#include "expression.hpp"

Expression::Expression()
{
	Node.type = None;
	Node.bool_value = NULL;
	Node.double_value = NULL;
	Node.string_value = "";
}

Expression::Expression(bool value) {
	Node.type = Bool;
	Node.bool_value = value;
	Node.double_value = NULL;
	Node.string_value = "";
	std::vector<Expression* > Branch;
}

Expression::Expression(double value) {
	Node.type = Value;
	Node.bool_value = NULL;
	Node.double_value = value;
	Node.string_value = "";
	std::vector<Expression* > Branch;
}

Expression::Expression(const std::string & value) {
	Node.type = Symbol;
	Node.bool_value = NULL;
	Node.double_value = NULL;
	Node.string_value = value;
	std::vector<Expression* > Branch;
}

bool Expression::operator==(const Expression & exp) const noexcept 
{

}