#include "Calculator.h"

typedef IOperation* (__stdcall* factory_t)();

Calculator::Calculator()
{
	operations.insert({ add, "+", 0});
	operations.insert({ subtract, "-", 0 });
	operations.insert({ multiply, "*", 1 });
	operations.insert({ divide, "/", 1 });
}

std::string Calculator::solve(std::string& expression) const
{
	std::vector<std::string> reversePolskNotation;
	if (!getReversePolskNotation(reversePolskNotation, expression))
	{
		return "error";
	}

	return std::to_string(solveReversePolskNotation(reversePolskNotation));
}

bool Calculator::readDll()
{
	const std::string pluginsDir = "plugins";
	const std::filesystem::path plugins{ pluginsDir };
	for (auto const& file : std::filesystem::directory_iterator{ plugins })
	{
		HMODULE Module = LoadLibraryA(file.path().generic_string().c_str());
		if (!Module)
		{
			return false;
		}
		std::string name = file.path().string().substr(pluginsDir.size() + 1, file.path().string().size() - pluginsDir.size() - 5);
		std::cout << name << std::endl;
		factory_t func = (factory_t)GetProcAddress(Module, "makeOperation");
		if (!func)
		{
			return false;
		}

		IOperation* hope = func();
		operations.insert({ hope->fun(), name, 2});
	}

	return true;
}

bool Calculator::getReversePolskNotation(std::vector<std::string>& reversePolskNotation, std::string const& expression) const
{
	std::stack<std::string> stack;
	for (size_t i = 0; i < expression.size(); ++i)
	{
		if (expression[i] >= '0' && expression[i] <= '9') {
			std::string number = "";
			bool hasPoint = false;
			while (expression[i] >= '0' && expression[i] <= '9' || expression[i] == '.')
			{
				if (expression[i] == '.')
				{
					if (hasPoint)
					{
						return false;
					}
					hasPoint = true;
				}
				
				number += std::string(1, expression[i]);
				++i;
			}

			--i;
			reversePolskNotation.push_back(number);
		}
		else if (expression[i] == ')')
		{
			while (!stack.empty() && stack.top() != "(")
			{
				reversePolskNotation.push_back(stack.top());
				stack.pop();
			}

			if (stack.empty())
			{
				return false;
			}

			stack.pop();
		}
		else if (expression[i] == '(')
		{
			stack.push(std::string(1, expression[i]));
		}
		else 
		{
			std::string oper = "";
			if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/')
			{
				oper = std::string(1, expression[i]);
			}
			else
			{
				auto checkIfPartOfOperation = [](char symbol) {
					return (symbol < '0' || symbol > '9')
						&& symbol != '.'
						&& symbol != '('
						&& symbol != ')'; };

				while (checkIfPartOfOperation(expression[i]) && i < expression.size())
				{
					oper += std::string(1, expression[i]);
					++i;
				}

				--i;
			}

			auto operIter = std::find_if(operations.begin(), operations.end(), [oper](auto operation) {
				return operation.name == oper;
				});
			if (operIter == operations.end())
			{
				return false;
			}

			if (operIter->isBinary && !stack.empty())
			{
				std::string top = stack.top();
				auto stackIter = std::find_if(operations.begin(), operations.end(), [top](auto operation) {
					return operation.name == top;
				});
				while (!stack.empty() && stackIter != operations.end()
					&& (operIter->priority < stackIter->priority || !stackIter->isBinary))
				{
					reversePolskNotation.push_back(top);
					stack.pop();
					if (!stack.empty())
					{
						top = stack.top();
						stackIter = std::find_if(operations.begin(), operations.end(), [top](auto operation) {
							return operation.name == top;
						});
					}
				}
			}

			stack.push(oper);
		}
	}

	while (!stack.empty())
	{
		reversePolskNotation.push_back(stack.top());
		stack.pop();
	}

	return true;
}

double Calculator::solveReversePolskNotation(std::vector<std::string> const& reversePolskNotation) const
{
	std::stack<double> stack;
	for (auto& piece : reversePolskNotation)
	{
		auto iter = std::find_if(operations.cbegin(), operations.cend(), [piece](auto oper) {
			return oper.name == piece;
			});
		if (iter != operations.end())
		{
			double tmp = stack.top();
			stack.pop();
			if (iter->isBinary)
			{
				tmp = iter->binary(stack.top(), tmp);
				stack.pop();
			}
			else
			{
				tmp = iter->unary(tmp);
			}
			stack.push(tmp);
		}
		else
		{
			stack.push(std::stod(piece));
		}
	}

	return stack.top();
}

double Calculator::add(double a, double b)
{
	return a + b;
}

double Calculator::subtract(double a, double b)
{
	return a - b;
}

double Calculator::multiply(double a, double b)
{
	return a * b;
}

double Calculator::divide(double a, double b)
{
	return a / b;
}
