#pragma once

typedef double (*oper_t)(double);

class IOperation
{
public:
	virtual oper_t fun () = 0;
};