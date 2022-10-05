#pragma once
#include <functional>
#include <string>


struct Operation
{
	~Operation() {}
	Operation(double (*unary)(double), std::string name, int priority): unary(unary), name(name), isBinary(false), priority(priority) {}
	Operation(Operation const& other): isBinary(other.isBinary), name(other.name), priority(other.priority)
	{
		if (isBinary)
		{
			binary = other.binary;
		}
		else
		{
			unary = other.unary;
		}
	}

	bool operator < (Operation const& other) const
	{
		return name < other.name;
	}

	Operation(double (*binary)(double, double), std::string name, int priority) : binary(binary), name(name), isBinary(true), priority(priority) {}
	union {
		double (*unary)(double);
		double (*binary)(double, double);
	};
	bool isBinary;
	std::string name;
	int priority;
};
