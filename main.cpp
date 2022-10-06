#include <iostream>
#include <string>
#include "Calculator.h"

int main(int argc, char** argv)
{
	Calculator calc;
	calc.readDll();
	std::string expression;
	std::cout << "Enter expression(enter q to stop):" << std::endl;
	std::getline(std::cin, expression);
	while (expression != "q")
	{
		try
		{
			std::cout << expression << "=" << calc.solve(expression) << std::endl;
		}
		catch (std::exception error)
		{
			std::cout << error.what() << std::endl;
		}
		std::cout << "Enter expression(enter q to stop):" << std::endl;
		std::getline(std::cin, expression);
	}

	return 0;
}
