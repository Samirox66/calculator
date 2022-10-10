#pragma once
#include <string>
#include <stack>
#include <vector>
#include <set>
#include <filesystem>
#include <iostream>
#include <exception>
#include <Windows.h>
#include "interfaces/interfaces.h"
#include "Operation.h"

class Calculator
{
public:
	Calculator();
	~Calculator();
	void readDll();
	std::string solve(std::string& expression) const;
private:
	std::vector<std::string> getReversePolskNotation(std::string& expression) const;
	double solveReversePolskNotation(std::vector<std::string> const& reversePolskNotation) const;
	void handleClosingBracket(std::vector<std::string>& reversePolskNotation, std::stack<std::string>& stack) const;
	void handleOperation(std::vector<std::string>& reversePolskNotation, std::stack<std::string>& stack, std::string expression, size_t* i) const;
	static std::string getNumberFromStr(std::string const& str, size_t index);
	static void add(std::stack<double>& polsk);
	static void subtract(std::stack<double>& polsk);
	static void multiply(std::stack<double>& polsk);
	static void divide(std::stack<double>& polsk);
	static void trim(std::string& str);
private:
	std::set<Operation> operations;
	std::vector<HMODULE> dlls;
};
