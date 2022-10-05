#include "Calculator.h"

typedef IOperation* (__stdcall* factory_t)();

Calculator::Calculator()
{
	operations.insert({ add, "+", 0});
	operations.insert({ subtract, "-", 0 });
	operations.insert({ multiply, "*", 1 });
	operations.insert({ divide, "/", 1 });
}

Calculator::~Calculator()
{
	for (auto& dll : dlls)
	{
		FreeLibrary(dll);
	}
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
	std::cout << "Loaded operations:" << std::endl;
	for (auto const& file : std::filesystem::directory_iterator{ plugins })
	{
		HMODULE Module = LoadLibraryA(file.path().generic_string().c_str());
		if (!Module)
		{
			return false;
		}
		factory_t func = (factory_t)GetProcAddress(Module, "makeOperation");
		if (!func)
		{
			return false;
		}

		dlls.push_back(Module);
		IOperation* hope = func();
		std::string name = hope->name();
		std::cout << name << std::endl;
		operations.insert({ hope->fun(), name, hope->priority()});
		delete hope;
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
			auto operation = operations.begin();
			for (; operation != operations.end(); ++operation)
			{
				if (operation->name == expression.substr(i, operation->name.size()))
				{
					break;
				}
			}
			if (operation == operations.end())
			{
				return false;
			}
			i += operation->name.size() - 1;
			
			if (operation->isBinary && !stack.empty())
			{
				std::string top = stack.top();
				auto stackIter = std::find_if(operations.begin(), operations.end(), [top](auto operation) {
					return operation.name == top;
				});
				while (!stack.empty() && stackIter != operations.end()
					&& (operation->priority < stackIter->priority || !stackIter->isBinary))
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

			stack.push(operation->name);
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
