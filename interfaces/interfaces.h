#pragma once
#include <string>
#include <stack>

typedef void (*oper_t)(std::stack<double>&);

class IOperation
{
public:
	virtual oper_t fun () = 0;
	virtual std::string name() = 0;
	virtual int priority() = 0;
	virtual bool isUnary() = 0;
	virtual bool isPrefixed() = 0;
	virtual ~IOperation() {};
};