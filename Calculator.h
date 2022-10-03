#pragma once
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <functional>

class Calculator
{
public:
	Calculator();
	void readDll();
	std::string solve(std::string& expression) const;
private:
	bool getReversePolskNotation(std::vector<std::string>& reversePolskNotation, std::string const& expression) const;
	double solveReversePolskNotation(std::vector<std::string> const& reversePolskNotation) const;
	static double add(double a, double b);
	static double subtract(double a, double b);
	static double multiply(double a, double b);
	static double divide(double a, double b);
private:
	std::map <std::string, std::function<double(double, double)>> binaryFunctions;
};

