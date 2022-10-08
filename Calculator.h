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
	void getReversePolskNotation(std::vector<std::string>& reversePolskNotation, std::string const& expression) const;
	double solveReversePolskNotation(std::vector<std::string> const& reversePolskNotation) const;
	static void add(std::stack<double>& polsk);
	static void subtract(std::stack<double>& polsk);
	static void multiply(std::stack<double>& polsk);
	static void divide(std::stack<double>& polsk);
private:
	std::set<Operation> operations;
	std::vector<HMODULE> dlls;
};
