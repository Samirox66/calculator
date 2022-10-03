#include "Calculator.h"

std::string Calculator::solve(std::string& expression)
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

bool Calculator::getReversePolskNotation(std::vector<std::string>& reversePolskNotation, std::string const& expression)
{
	std::stack<std::string> stack;
	for (size_t i = 0; i < expression.size(); i++)
	{
		if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/' || expression[i] == '(')
		{
			while (!stack.empty() && (expression[i] == '+' || expression[i] == '-') && (stack.top() == "*" || stack.top() == "/"))
			{
				reversePolskNotation.push_back(stack.top());
				stack.pop();
			}
			stack.push(std::string(1, expression[i]));
		}
		else if (expression[i] >= '0' && expression[i] <= '9')
		{
			reversePolskNotation.push_back(std::string(1, expression[i]));

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

double Calculator::solveReversePolskNotation(std::vector<std::string> const& reversePolskNotation)
{
	std::stack<double> stack;
	std::optional<double> first;
	std::optional<double> second;
	for (auto& piece : reversePolskNotation)
	{
		if (piece == "+")
		{
			double tmp = stack.top();
			stack.pop();
			tmp += stack.top();
			stack.pop();
			stack.push(tmp);
		}
		else if(piece == "-")
		{
			double tmp = stack.top();
			stack.pop();
			tmp = stack.top() - tmp;
			stack.pop();
			stack.push(tmp);
		}
		else if (piece == "*")
		{
			double tmp = stack.top();
			stack.pop();
			tmp *= stack.top();
			stack.pop();
			stack.push(tmp);
		}
		else if (piece == "/")
		{
			double tmp = stack.top();
			stack.pop();
			tmp = stack.top() / tmp;
			stack.pop();
			stack.push(tmp);
		}
		else
		{
			stack.push(std::stoi(piece));
		}
	}

	return stack.top();
}