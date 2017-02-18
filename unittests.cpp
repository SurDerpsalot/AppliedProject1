#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"
#include "interpreter.hpp"
#include "expression.hpp"

TEST_CASE("Test Interpreter parser with expected input", "[interpreter]") {

	std::string program = "(begin (define r 11) (* pi (* r true)))";

	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);

	REQUIRE(ok == true);
}

TEST_CASE("Test Expression overload", "[Expression]")
{
	std::string pass;
	pass = "false";
	Expression d;
	Expression e(4.0);
	Expression f(3);
	Expression g(true);
	Expression h(pass);
	REQUIRE((d == e) == false);
	REQUIRE((e == f) == true);
	REQUIRE((e == g) == false);
	REQUIRE((e == h) == false);
	REQUIRE((g == h) == false);
}

/*
TEST_CASE("Test Interpreter parser with truncated input", "[interpreter]") {

	{
		std::string program = "(f";
		std::istringstream iss(program);

		Interpreter interp;
		bool ok = interp.parse(iss);
		REQUIRE(ok == false);
	}

	{
		std::string program = "(begin (define r 10) (* pi (* r r";
		std::istringstream iss(program);

		Interpreter interp;
		bool ok = interp.parse(iss);
		REQUIRE(ok == false);
	}
}
TEST_CASE("Test Interpreter parser with extra input", "[interpreter]") {

	std::string program = "(begin (define r 10) (* pi (* r r))) )";
	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);

	REQUIRE(ok == false);
}

TEST_CASE("Test Interpreter parser with single non-keyword", "[interpreter]") {

	std::string program = "hello";
	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);

	REQUIRE(ok == false);
}

TEST_CASE("Test Interpreter parser with empty input", "[interpreter]") {

	std::string program;
	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);

	REQUIRE(ok == false);
}

TEST_CASE("Test Interpreter parser with empty expression", "[interpreter]") {

	std::string program = "( )";
	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);

	REQUIRE(ok == false);
}

TEST_CASE("Test Interpreter parser with bad number string", "[interpreter]") {

	std::string program = "(1abc)";
	std::istringstream iss(program);

	Interpreter interp;

	bool ok = interp.parse(iss);

	REQUIRE(ok == false);
}*/