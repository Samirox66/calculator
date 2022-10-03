#include "Calculator.h"

Calculator::Calculator()
{
	binaryFunctions["+"] = add;
	binaryFunctions["-"] = subtract;
	binaryFunctions["*"] = multiply;
	binaryFunctions["/"] = divide;
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

void Calculator::readDll()
{

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
		else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/' || expression[i] == '(')
		{
			while (!stack.empty() && (expression[i] == '+' || expression[i] == '-') && (stack.top() == "*" || stack.top() == "/"))
			{
				reversePolskNotation.push_back(stack.top());
				stack.pop();
			}

			stack.push(std::string(1, expression[i]));
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
		auto iter = binaryFunctions.find(piece);
		if (iter != binaryFunctions.end())
		{
			double tmp = stack.top();
			stack.pop();
			tmp = iter->second(stack.top(), tmp);
			stack.pop();
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
