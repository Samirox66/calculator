#include <iostream>
#include <string>
#include "Calculator.h"

int main(int argc, char** argv)
{
	Calculator calc;
	if (!calc.readDll())
	{
		std::cout << "Error reading dll" << std::endl;
		return 4;
	}

	std::string expression;
	std::getline(std::cin, expression);
	while (expression != "q")
	{
		std::cout << expression << "=" << calc.solve(expression) << std::endl;
		std::getline(std::cin, expression);
	}

	return 0;
}