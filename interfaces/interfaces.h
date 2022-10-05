#pragma once
#include <string>

typedef double (*oper_t)(double);

class IOperation
{
public:
	virtual oper_t fun () = 0;
	virtual std::string name() = 0;
	virtual int priority() = 0;
};