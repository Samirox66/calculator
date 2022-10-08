#include "Calculator.h"

typedef IOperation* (__stdcall* factory_t)();

Calculator::Calculator()
{
	operations.insert({ add, "+", 0, false, false});
	operations.insert({ subtract, "-", 0, false, false });
	operations.insert({ multiply, "*", 1, false, false });
	operations.insert({ divide, "/", 1, false, false });
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
	getReversePolskNotation(reversePolskNotation, expression);
	return std::to_string(solveReversePolskNotation(reversePolskNotation));
}

void Calculator::readDll()
{
	const std::string pluginsDir = "plugins";
	const std::filesystem::path plugins{ pluginsDir };
	std::cout << "Loaded operations:" << std::endl;
	for (auto const& file : std::filesystem::directory_iterator{ plugins })
	{
		HMODULE Module = LoadLibraryA(file.path().generic_string().c_str());
		if (Module)
		{
			factory_t func = (factory_t)GetProcAddress(Module, "makeOperation");
			dlls.push_back(Module);
			if (func)
			{
				IOperation* hope = func();
				std::string name = hope->name();
				std::cout << name << std::endl;
				operations.insert({ hope->fun(), name, hope->priority(), hope->isUnary(), hope->isPrefixed()});
				delete hope;
			}
		}
	}

}

void Calculator::getReversePolskNotation(std::vector<std::string>& reversePolskNotation, std::string const& expression) const
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
						throw std::exception("too many points in number");
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
				throw std::exception("expression has more closed brackets, than opened ");
			}

			stack.pop();
		}
		else if (expression[i] == '(')
		{
			stack.push(std::string(1, expression[i]));
		}
		else if (expression[i] == ' ' || expression[i] ==',')
		{
			continue;
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
				throw std::exception("impossible operation");
			}

			i += operation->name.size() - 1;
			if (operation->isUnary)
			{
				if (operation->isPrefixed)
				{
					stack.push(operation->name);
				}
				else
				{
					reversePolskNotation.push_back(operation->name);
				}
			}
			else
			{
				if (!stack.empty())
				{
					std::string top = stack.top();
					auto stackIter = std::find_if(operations.begin(), operations.end(), [top](auto operation) {
						return operation.name == top;
						});
					while (!stack.empty() && stackIter != operations.end()
						&& (operation->priority < stackIter->priority || stackIter->isUnary))
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
	}

	while (!stack.empty())
	{
		reversePolskNotation.push_back(stack.top());
		stack.pop();
	}
}

double Calculator::solveReversePolskNotation(std::vector<std::string> const& reversePolskNotation) const
{
	std::stack<double> polsk;
	for (auto& piece : reversePolskNotation)
	{
		if ('0' <= piece[0] && piece[0] <= '9') {
			polsk.push(std::stod(piece));
		}
		else
		{
			auto iter = std::find_if(operations.cbegin(), operations.cend(), [piece](auto oper) {
				return oper.name == piece;
				});
			
			if (polsk.empty())
			{
				throw std::exception("something wrong with operations");
			}

			iter->oper(polsk);
		}
	}

	if (polsk.size() != 1)
	{
		throw std::exception("wrong expression");
	}

	return polsk.top();
}

void Calculator::add(std::stack<double>& polsk)
{
	if (polsk.size() < 2)
	{
		throw std::exception("too litle numbers for operation");
	}
	double b = polsk.top();
	polsk.pop();
	b = polsk.top() + b;
	polsk.pop();
	polsk.push(b);
}

void Calculator::subtract(std::stack<double>& polsk)
{
	if (polsk.size() < 2)
	{
		throw std::exception("too litle numbers for operation");
	}
	double b = polsk.top();
	polsk.pop();
	b = polsk.top() - b;
	polsk.pop();
	polsk.push(b);
}

void Calculator::multiply(std::stack<double>& polsk)
{
	if (polsk.size() < 2)
	{
		throw std::exception("too litle numbers for operation");
	}
	double b = polsk.top();
	polsk.pop();
	b = polsk.top() * b;
	polsk.pop();
	polsk.push(b);
}

void Calculator::divide(std::stack<double>& polsk)
{
	if (polsk.size() < 2)
	{
		throw std::exception("too litle numbers for operation");
	}
	double b = polsk.top();
	polsk.pop();
	b = polsk.top() / b;
	polsk.pop();
	polsk.push(b);
}
