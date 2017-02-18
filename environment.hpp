#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "string.h"
#include "expression.hpp"
#include <map>

class Environment {
public:
	typedef Expression(Environment::*FunctionPointer(Expression));
	typedef std::map<std::string, FunctionPointer > MAP;
	Environment();
	void EnviroBuild();
	Expression Operations(Expression * Top);
	Expression ProType(Expression * Top);
	Expression EnviroAccess(Expression *Top, size_t childIndex);
	Expression EnvDef(Expression*Top);
	Expression EnvIf(Expression*Top);
	Expression NonSpec(Expression*Top);

private:
	MAP Express;
};




#endif // !ENVIRONMENT_H