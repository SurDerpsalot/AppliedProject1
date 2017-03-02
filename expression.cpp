#include "expression.hpp"

Expression::Expression()
{
	Node.type = None;
	Node.bool_value = false;
	Node.double_value = 0.0;
	Node.string_value = "";
}

Expression::Expression(bool value) {
	Node.type = Bool;
	Node.bool_value = value;
	Node.double_value = 0.0;
	Node.string_value = "";

}

Expression::Expression(double value) {
	Node.type = Value;
	Node.bool_value = false;
	Node.double_value = value;
	Node.string_value = "";
}

Expression::Expression(int value) {
	Node.type = Value;
	Node.bool_value = false;
	Node.double_value = value;
	Node.string_value = "";
}

Expression::Expression(const std::string & value) {
	Node.type = Symbol;
	Node.bool_value = false;
	Node.double_value = 0.0;
	Node.string_value = value;
}

bool Expression::operator==(const Expression & exp) const noexcept 
{
	return (Node.type == exp.Node.type) && (Node.Branch.size() == exp.Node.Branch.size());
}
/*
bool Expression::operator&&(const Expression & exp) const noexcept
{
	if ((Node.type == exp.Node.type) && (Node.bool_value == exp.Node.bool_value))
		return true;
	else
		return false;
}*/