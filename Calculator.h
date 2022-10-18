#pragma once
#include <string>
#include <stack>
#include <vector>
#include <set>
#include <filesystem>
#include <iostream>
#include <exception>
#include <Windows.h>
#include "Operation.h"

class Calculator
{
public:
	Calculator();
	Calculator(Calculator const&) = delete;
	Calculator& operator =(Calculator const&) = delete;
	Calculator(Calculator&&) = default;
	Calculator& operator =(Calculator&&) = default;
	~Calculator();
	void readDll();
	std::string solve(std::string const& expression);
private:
	std::vector<std::string> getReversePolskNotation();
	double solveReversePolskNotation(std::vector<std::string> const& reversePolskNotation) const;
	void handleClosingBracket(std::vector<std::string>& reversePolskNotation, std::stack<std::string>& stack) const;
	void handleOperation(std::vector<std::string>& reversePolskNotation, std::stack<std::string>& stack, size_t* i) const;
	std::string getNumberFromStr(size_t index);
	static void add(std::stack<double>& polsk);
	static void subtract(std::stack<double>& polsk);
	static void multiply(std::stack<double>& polsk);
	static void divide(std::stack<double>& polsk);
	void trim();
private:
	std::string expression;
	std::set<Operation*> operations;
	std::vector<HMODULE> dlls;
};
