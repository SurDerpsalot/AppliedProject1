#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "string.h"
#include "interpreter.hpp"
#include "expression.hpp"
#include <map>
typedef std::map<Expression, Expression> MAP;
class Environment {
public:
	Environment();

	void EnviroBuild(Node * Top);

	Expression EnviroAccess(Node*Top);

private:
	MAP Express;
};




#endif // !ENVIRONMENT_H