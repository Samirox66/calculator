#pragma once
#include <string>
#include <stack>
#include <vector>
#include <optional>

class Calculator
{
public:
	void readDll();
	std::string solve(std::string& expression);
private:
	bool getReversePolskNotation(std::vector<std::string>& reversePolskNotation, std::string const& expression);
	double solveReversePolskNotation(std::vector<std::string> const& reversePolskNotation);
};

