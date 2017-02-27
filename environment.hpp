#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "expression.hpp"
#include <map>
#include <math.h>
#include "interpreter_semantic_error.hpp"
class Environment {
public:
	typedef Expression(Environment::*FunctionPointer)(Expression);
	typedef std::map<std::string, FunctionPointer > MAP;
	Environment();
	void EnviroBuild();
	bool checkProcedure(Expression Target);
	Expression Operations(Expression  Top);
	Expression ProType(Expression  Top);
	Expression EnvDef(Expression Top);
	Expression EnvIf(Expression Top);
	Expression NonSpec(Expression Top);
	Expression EnvNot(Expression Top);
	Expression EnvMinus(Expression Top);
	Expression EnvMult(Expression Top);
	Expression EnvAnd(Expression Top);
	Expression EnvAdd(Expression Top);
	Expression EnvOr(Expression Top);
	Expression EnvLes(Expression Top);
	Expression EnvLeq(Expression Top);
	Expression EnvGrt(Expression Top);
	Expression EnvGeq(Expression Top);
	Expression EnvEq(Expression Top);
	Expression EnvDiv(Expression Top);
private:
	MAP Express;
	Expression Error;
	std::map<std::string, Expression> Dictionary;
	std::map<std::string, Expression>::iterator found;
	MAP::iterator pro;
};




#endif // !ENVIRONMENT_H