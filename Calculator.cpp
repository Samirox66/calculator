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
	std::vector<std::string> reversePolskNotation = getReversePolskNotation(expression);
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

std::vector<std::string> Calculator::getReversePolskNotation(std::string& expression) const
{
	trim(expression);
	std::vector<std::string> reversePolskNotation;
	std::stack<std::string> stack;
	for (size_t i = 0; i < expression.size(); ++i)
	{
		if (expression[i] >= '0' && expression[i] <= '9' || expression[i] == '-' && (i == 0 || expression[i - 1] < '0' || expression[i - 1] > '9')) {
			std::string number = "";
			size_t k = i;
			if (expression[i] == '-')
			{
				number = "-";
				++k;
			}
			
			number += getNumberFromStr(expression, k);
			i += number.size() - 1;
			reversePolskNotation.push_back(number);
		}
		else if (expression[i] == ')')
		{
			handleClosingBracket(reversePolskNotation, stack);
		}
		else if (expression[i] == '(')
		{
			stack.push(std::string(1, expression[i]));
		}
		else if (expression[i] ==',')
		{
			continue;
		}
		else
		{
			handleOperation(reversePolskNotation, stack, expression, &i);
		}
	}

	while (!stack.empty())
	{
		if (stack.top() == "(")
		{
			throw std::exception("bracket has been lost");
		}
		reversePolskNotation.push_back(stack.top());
		stack.pop();
	}

	return reversePolskNotation;
}

double Calculator::solveReversePolskNotation(std::vector<std::string> const& reversePolskNotation) const
{
	std::stack<double> polsk;
	for (auto& piece : reversePolskNotation)
	{
		if ('0' <= piece[0] && piece[0] <= '9' || piece[0] == '-' && piece.size() > 1) {
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

void Calculator::handleClosingBracket(std::vector<std::string>& reversePolskNotation, std::stack<std::string>& stack) const
{
	while (!stack.empty() && stack.top() != "(")
	{
		reversePolskNotation.push_back(stack.top());
		stack.pop();
	}

	if (stack.empty())
	{
		throw std::exception("expression has more closing brackets, than opening ");
	}

	stack.pop();
	if (!stack.empty())
	{
		auto topOfStackIter = std::find_if(operations.begin(), operations.end(), [&stack](auto operation) {
			return operation.name == stack.top();
			});
		if (topOfStackIter != operations.end() && topOfStackIter->isPrefixed)
		{
			reversePolskNotation.push_back(stack.top());
			stack.pop();
		}
	}
}

void Calculator::handleOperation(std::vector<std::string>& reversePolskNotation, std::stack<std::string>& stack, std::string expression, size_t* i) const
{
	auto operation = operations.begin();
	for (; operation != operations.end(); ++operation)
	{
		if (operation->name == expression.substr(*i, operation->name.size()))
		{
			break;
		}
	}
	if (operation == operations.end())
	{
		throw std::exception("impossible operation");
	}

	*i += operation->name.size() - 1;
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
			auto topOfStackIter = std::find_if(operations.begin(), operations.end(), [top](auto operation) {
				return operation.name == top;
				});
			while (!stack.empty() && topOfStackIter != operations.end()
				&& (operation->priority < topOfStackIter->priority || topOfStackIter->isUnary))
			{
				reversePolskNotation.push_back(top);
				stack.pop();
				if (!stack.empty())
				{
					top = stack.top();
					topOfStackIter = std::find_if(operations.begin(), operations.end(), [top](auto operation) {
						return operation.name == top;
						});
				}
			}
		}

		stack.push(operation->name);
	}
}

std::string Calculator::getNumberFromStr(std::string const& str, size_t index)
{
	std::string number = "";
	bool hasPoint = false;
	while (str[index] >= '0' && str[index] <= '9' || str[index] == '.')
	{
		if (str[index] == '.')
		{
			if (hasPoint)
			{
				throw std::exception("too many points in number");
			}
			hasPoint = true;
		}

		number += std::string(1, str[index]);
		++index;
	}

	return number;
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

void Calculator::trim(std::string& str)
{
	size_t k = 0;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (str[i] != ' ')
		{
			str[k] = str[i];
			++k;
		}
	}

	str.resize(k);
}
