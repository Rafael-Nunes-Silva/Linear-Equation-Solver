#include "StringMathSolver.hpp"

std::string StringMathSolver::FormatEquation(std::string equation) {
	for (int i = equation.length(); i >= 0; i--) {
		if (std::isdigit(equation[i]) && equation[i + 1] == '(') {
			std::string group = "";
			for (int j = 0; i+1 + j < equation.length(); j++) {
				if (equation[i+1 + j] == ')') {
					group = equation.substr(i+1, j);
					break;
				}
			}

			if (CanSolveAll(group)) {
				std::string replaceStr = "";
				replaceStr += equation[i];
				replaceStr += " * ";
				equation = equation.substr(0, i) + replaceStr + equation.substr(i + 1, equation.length() + 1);
			}
			else
				equation = equation.substr(0, i) + MulGroup(group, equation.substr(i, 1)) + equation.substr(i + group.length()+1, equation.length() + 1);
		}
		else if (IsOperator(equation[i]) || equation[i] == '=') {
			std::string replaceStr = "";
			if (equation[i - 1] != ' ')
				replaceStr += " ";
			replaceStr += equation[i];
			if (equation[i + 1] != ' ')
				replaceStr += " ";

			equation = equation.substr(0, i) + replaceStr + equation.substr(i + 1, equation.length() + 1);
		}
	}

	return equation;
}
std::string StringMathSolver::ReplaceInString(std::string base, std::string from, std::string to) {
	if (base.length() == 0 || from.length() == 0)
		std::cout << "Base and From can not be empty strings\n";

	for (int i = 0; i < base.length(); i++) {
		if (base[i] == from[0] && base.substr(i, from.length()) == from) {
			std::string result = base.substr(0, i);
			result += to;
			result += base.substr(i + from.length(), base.length());
			return result;
		}
	}
	std::cout << "Could not find to replace: " << base << " ('" << from << "' -> '" << to << "')\n";
	return base;
}

std::string StringMathSolver::GetNumberRight(std::string s, int offset) {
	if (offset >= s.length()) {
		std::cout << "Offset must be within the string's length\n" << s << " | offset: " << offset << "\n";
		return "?";
	}

	for (int i = offset + 1; i < s.length(); i++) {
		if (std::isdigit(s[i]) || std::isalpha(s[i]) || s[i] == '.' || s[i] == '-') {
			std::string result = std::string(1, s[i]);
			for (int j = i+1; j < s.length(); j++) {
				if (!std::isdigit(s[j]) && !std::isalpha(s[j]) && s[j] != '.')
					break;
				result += s[j];
			}
			return result;
		}
	}

	std::cout << "\nCould not get number right:\n" << s << "\n";
	std::string debug = "";
	for (int i = 0; i < offset; i++)
		debug += " ";
	debug += "^";
	std::cout << debug << "\n";

	return "?";
}
std::string StringMathSolver::GetNumberLeft(std::string s, int offset) {
	if (offset >= s.length()) {
		std::cout << "Offset must be within the string's length\n" << s << " | offset: " << offset << "\n";
		return "?";
	}

	if (s.length() == 1 && IsVariable(s))
		return "1" + s;

	for (int i = offset - 1; i>= 0; i--) {
		if (std::isdigit(s[i]) || std::isalpha(s[i])) {
			std::string result = "";
			for (int j = i; j>=0; j--) {
				if (!std::isdigit(s[j]) && !std::isalpha(s[j]) && s[j] != '.' && s[j] != '-')
					break;
				result = s[j] + result;
			}
			return result;
		}
	}

	std::cout << "\nCould not get number left:\n" << s << "\n";
	std::string debug = "";
	for (int i = 0; i < offset; i++)
		debug += " ";
	debug += "^";
	std::cout << debug << "\n";

	return "?";
}

bool StringMathSolver::CanSolveAll(std::string group) {
	if (!HasExpression(group))
		return false;

	for (int i = 0; i < group.length(); i++) {
		if (group[i] == '+' || group[i] == '-') {
			if (IsVariable(GetNumberLeft(group, i)) != IsVariable(GetNumberRight(group, i)))
				return false;
		}
	}

	return true;
}
bool StringMathSolver::HasGroup(std::string equation) {
	for (int i = equation.length() - 1; i >= 0; i--) {
		if (equation[i] == '(') {
			for (int j = i; j < equation.length(); j++) {
				if (equation[j] == ')')
					return true;
			}
		}
	}
	return false;
}
bool StringMathSolver::HasExpression(std::string equation) {
	for (int i = 1; i < equation.length()-1; i++) {
		if (!IsOperator(equation[i]))
			continue;

		if (equation[i-1] == ' ' && equation[i+1] == ' ') {
			if(equation[i]=='*' || equation[i]=='/')
				return true;

			if (equation[i] == '+' || equation[i] == '-') {
				if (IsVariable(GetNumberLeft(equation, i)) == IsVariable(GetNumberRight(equation, i)))
					return true;
			}
		}
	}
	return false;
}
bool StringMathSolver::IsVariable(std::string s) {
	return std::isalpha(s[s.length() - 1]);
}
bool StringMathSolver::IsOperator(char c) {
	return (c == '*' || c == '/' || c == '+' || c == '-');
}

std::string StringMathSolver::OppositeOperator(char operation) {
	switch (operation)
	{
	case '*':
		return "/";
	case '/':
		return "*";
	case '+':
		return "-";
	case '-':
		return "+";
	}
	std::cout << "Could not find opposite operator: " << operation << "\n";
	return "?";
}
std::string StringMathSolver::Encapsulate(std::string s) {
	return "(" + s + ")";
}
std::string StringMathSolver::IsolateVariables(std::string equation) {
	std::string leftSide = GetEquationLeftSide(equation), rightSide = GetEquationRightSide(equation);

	int i = 0;
	while (i < leftSide.length()) {
		if (!IsOperator(leftSide[i])) {
			i++;
			continue;
		}

		if (i == 0 || (leftSide[i - 1] != ' ' && leftSide[i + 1] != ' ')) {
			i++;
			continue;
		}

		std::string numLeft = GetNumberLeft(leftSide, i), numRight = GetNumberRight(leftSide, i);
		if (IsVariable(numLeft) && !IsVariable(numRight)) {
			std::string tempRightSide = Encapsulate(rightSide), tempLeftSide = leftSide;
			tempRightSide += " " + OppositeOperator(leftSide[i]) + " " + numRight;

			tempLeftSide = ReplaceInString(tempLeftSide, " " + std::string(1, leftSide[i]) + " " + numRight, "");

			equation = ReplaceInString(ReplaceInString(equation, leftSide, tempLeftSide), rightSide, tempRightSide);
			i = 0;
		}
		else if (!IsVariable(numLeft) && IsVariable(numRight)) {
			std::string tempRightSide = Encapsulate(rightSide), tempLeftSide = leftSide;
			tempRightSide += (numLeft[0] == '-' ? (" + " + numLeft) : (" - " + numLeft));

			tempLeftSide = ReplaceInString(tempLeftSide, numLeft + " " + std::string(1, leftSide[i]) + " ", "");

			equation = ReplaceInString(ReplaceInString(equation, leftSide, tempLeftSide), rightSide, tempRightSide);
			i = 0;
		}

		leftSide = GetEquationLeftSide(equation);
		rightSide = GetEquationRightSide(equation);
		i++;
	}

	leftSide = GetEquationLeftSide(equation), rightSide = GetEquationRightSide(equation);

	i = 0;
	while (i < rightSide.length()) {
		if (!IsOperator(rightSide[i])) {
			i++;
			continue;
		}

		if (i == 0 || (rightSide[i - 1] != ' ' && rightSide[i + 1] != ' ')) {
			i++;
			continue;
		}

		std::string numLeft = GetNumberLeft(rightSide, i), numRight = GetNumberRight(rightSide, i);
		if (IsVariable(numLeft) && !IsVariable(numRight)) {
			std::string tempLeftSide = Encapsulate(leftSide), tempRightSide = rightSide;
			tempLeftSide += (numLeft[0] == '-' ? " + " + numLeft : " - " + numLeft);

			tempRightSide = ReplaceInString(tempRightSide, numLeft + " " + std::string(1, rightSide[i]) + " ", (rightSide[i] == '-' ? "-" : ""));

			equation = ReplaceInString(ReplaceInString(equation, leftSide, tempLeftSide), rightSide, tempRightSide);
			i = 0;
		}
		else if (!IsVariable(numLeft) && IsVariable(numRight)) {
			std::string tempLeftSide = Encapsulate(leftSide), tempRightSide = rightSide;
			tempLeftSide += " " + OppositeOperator(rightSide[i]) + " " + numRight;

			tempRightSide = ReplaceInString(tempRightSide, " " + std::string(1, rightSide[i]) + " " + numRight, "");

			equation = ReplaceInString(ReplaceInString(equation, leftSide, tempLeftSide), rightSide, tempRightSide);
			i = 0;
		}

		leftSide = GetEquationLeftSide(equation);
		rightSide = GetEquationRightSide(equation);
		i++;
	}

	return equation;
}

std::string StringMathSolver::GetEquationRightSide(std::string equation) {
	for (int i = equation.length() - 1; i >= 0; i--) {
		if (equation[i - 1] == '=')
			return equation.substr(i+1, equation.length() - i);
	}
	return equation;
}
std::string StringMathSolver::GetEquationLeftSide(std::string equation) {
	for (int i = 0; i < equation.length(); i++) {
		if (equation[i + 1] == '=')
			return equation.substr(0, i);
	}
	return equation;
}
std::string StringMathSolver::GetHighierOrderGroup(std::string equation) {
	if (!HasGroup(equation)) {
		std::cout << equation << " | Has no group\n";
		return "";
	}

	for (int i = equation.length() - 1; i >= 0; i--) {
		if (equation[i] == '(') {
			for (int j = i + 1; j < equation.length(); j++) {
				if (equation[j] == ')')
					return equation.substr(i, j - i+1);
			}
		}
	}

	std::cout << "Something went wrong at GetHighierOrderGroup\n";
	return "?";
}
std::string StringMathSolver::GetHighierOrderExpression(std::string group) {
	if (!HasExpression(group)) {
		std::cout << group << " | Has no expression\n";
		return "";
	}

	char highiestOperator = '-';
	std::string result = "";
	for (int i = group.length() - 1; i > 0; i--) {
		if (!IsOperator(group[i]))
			continue;

		if (group[i - 1] != ' ' || group[i + 1] != ' ')
			continue;

		switch (group[i]) {
		case '*':
				highiestOperator = group[i];
				result = GetNumberLeft(group, i);
				result += " * ";
				result += GetNumberRight(group, i);
			break;
		case '/':
				highiestOperator = group[i];
				result = GetNumberLeft(group, i);
				result += " / ";
				result += GetNumberRight(group, i);
			break;
		case '+':
			if (highiestOperator == '+' || highiestOperator=='-') {
				highiestOperator = group[i];
				result = GetNumberLeft(group, i);
				result += " + ";
				result += GetNumberRight(group, i);
			}
			break;
		case '-':
			if (highiestOperator == '+' || highiestOperator == '-') {
				highiestOperator = group[i];
				result = GetNumberLeft(group, i);
				result += " - ";
				result += GetNumberRight(group, i);
			}
			break;
		}
	}

	return result;
}

std::string StringMathSolver::SolveEquation(std::string equation) {
	std::string tempEquation = equation;
	std::cout << "\n";

	equation = FormatEquation(equation);
	if (equation != tempEquation) {
		tempEquation = equation;
		std::cout << "\n" << tempEquation << "\n";
	}

	do {
		equation = SolveEquationsGroups(equation);
		if (equation != tempEquation) {
			tempEquation = equation;
			std::cout << "\n" << tempEquation << "\n";
		}
		equation = SolveEquationsExpressions(equation);
		if (equation != tempEquation) {
			tempEquation = equation;
			std::cout << "\n" <<tempEquation << "\n";
		}

		equation = IsolateVariables(equation);
		if (equation != tempEquation) {
			tempEquation = equation;
			std::cout << "\n" << tempEquation << "\n";
		}

		equation = SolveEquationsGroups(equation);
		if (equation != tempEquation) {
			tempEquation = equation;
			std::cout << "\n" << tempEquation << "\n";
		}
		equation = SolveEquationsExpressions(equation);
		if (equation != tempEquation) {
			tempEquation = equation;
			std::cout << "\n" << tempEquation << "\n";
		}

		for (int i = 1; i < equation.length(); i++) {
			if (std::isalpha(equation[i])) {
				std::string numLeft = GetNumberLeft(equation, i);

				std::string tempRightSide = GetEquationRightSide(equation) + " / " + numLeft;

				equation = ReplaceInString(equation, GetEquationRightSide(equation), tempRightSide);
				equation = ReplaceInString(equation, numLeft, "");
				break;
			}
		}
		if (equation != tempEquation) {
			tempEquation = equation;
			std::cout << "\n" << tempEquation << "\n";
		}

		tempEquation = SolveEquationsExpressions(tempEquation);
	} while (HasGroup(tempEquation) || HasExpression(tempEquation));

	return tempEquation; // SolveEquationsExpressions(equation);
}
std::string StringMathSolver::SolveEquationsGroups(std::string equation) {
	while (HasGroup(equation)) {
		std::string highiestGroup = GetHighierOrderGroup(equation);
		equation = ReplaceInString(equation, highiestGroup, SolveGroup(highiestGroup));
	}
	return equation;
}
std::string StringMathSolver::SolveEquationsExpressions(std::string equation) {
	while (HasExpression(equation)) {
		std::string highiestExpression = GetHighierOrderExpression(equation);
		equation = ReplaceInString(equation, highiestExpression, SolveExpression(highiestExpression));
	}
	return equation;
}
std::string StringMathSolver::SolveGroup(std::string group) {
	if (!HasExpression(group))
		group.substr(1, group.length() - 2);

	// std::cout << group << ":\n";
	while (HasExpression(group)) {
		std::string highiestExpression = GetHighierOrderExpression(group);
		group = ReplaceInString(group, highiestExpression, SolveExpression(highiestExpression));
	}
	return group.substr(1, group.length() - 2);
}
std::string StringMathSolver::SolveExpression(std::string expression) {
	int operatorIndex = 0;
	char operation = '-';
	for (int i = 1; i < expression.length()-1; i++) {
		if (IsOperator(expression[i]) && expression[i-1]==' ' && expression[i+1]==' ') {
			operatorIndex = i;
			operation = expression[i];
			break;
		}
	}

	std::cout << "\n" << expression << " --> ";

	std::string numLeft = "?", numRight = "?", result="?";
	switch (operation){
	case '*':
		numLeft = GetNumberLeft(expression, operatorIndex);
		numRight = GetNumberRight(expression, operatorIndex);
		result = Mul(numLeft, numRight);
		std::cout << result << "\n";
		return result;
	case '/':
		numLeft = GetNumberLeft(expression, operatorIndex);
		numRight = GetNumberRight(expression, operatorIndex);
		result = Div(numLeft, numRight);
		std::cout << result << "\n";
		return result;
	case '+':
		numLeft = GetNumberLeft(expression, operatorIndex);
		numRight = GetNumberRight(expression, operatorIndex);
		result = Add(numLeft, numRight);
		std::cout << result << "\n";
		return result;
	case '-':
		numLeft = GetNumberLeft(expression, operatorIndex);
		numRight = GetNumberRight(expression, operatorIndex);
		result = Sub(numLeft, numRight);
		std::cout << result << "\n";
		return result;
	}

	std::cout << "Could not solve expression: " << expression << "\n";
	return "?";
}

std::string StringMathSolver::Mul(std::string a, std::string b) {
	if (IsVariable(a) && IsVariable(b))
		return std::to_string(std::stof(GetNumberLeft(a, a.length() - 1))* std::stof(GetNumberLeft(b, b.length() - 1)));

	if (IsVariable(a))
		return std::to_string(std::stof(GetNumberLeft(a, a.length() - 1))*std::stof(b)) + a[a.length()-1];
	else if (IsVariable(b))
		return std::to_string(std::stof(a)* std::stof(GetNumberLeft(b, b.length() - 1)))+b[b.length()-1];

	return std::to_string(std::stof(a) * std::stof(b));
}
std::string StringMathSolver::Div(std::string a, std::string b) {
	if (IsVariable(a) && IsVariable(b))
		return std::to_string(std::stof(GetNumberLeft(a, a.length() - 1)) / std::stof(GetNumberLeft(b, b.length() - 1)));

	if (IsVariable(a))
		return std::to_string(std::stof(GetNumberLeft(a, a.length() - 1)) / std::stof(b))+a[a.length()-1];
	else if (IsVariable(b))
		return std::to_string(std::stof(a) / std::stof(GetNumberLeft(b, b.length() - 1)))+b[b.length()-1];

	return std::to_string(std::stof(a) / std::stof(b));
}
std::string StringMathSolver::Add(std::string a, std::string b) {
	if (IsVariable(a) != IsVariable(b)) {
		std::cout << "Add must have both numbers or variables to work\n";
		return "?";
	}

	if (IsVariable(a) && IsVariable(b))
		return std::to_string(std::stof(GetNumberLeft(a, a.length() - 1)) + std::stof(GetNumberLeft(b, b.length() - 1))) + a[a.length() - 1];

	return std::to_string(std::stof(a) + std::stof(b));
}
std::string StringMathSolver::Sub(std::string a, std::string b) {
	if (IsVariable(a) != IsVariable(b)) {
		std::cout << "Sub must have both numbers or variables to work\n";
		return "?";
	}

	if (IsVariable(a) && IsVariable(b))
		return std::to_string(std::stof(GetNumberLeft(a, a.length() - 1)) - std::stof(GetNumberLeft(b, b.length() - 1))) + a[a.length() - 1];

	return std::to_string((a.length()==1?std::stoi(a):std::stof(a)) -(b.length()==1?std::stoi(b):std::stof(b)));
}

std::string StringMathSolver::MulGroup(std::string group, std::string num) {
	std::vector<std::string >groupNumbers = std::vector<std::string>();

	for (int i = 1; i < group.length(); ) {
		if (!std::isdigit(group[i])) {
			i++;
			continue;
		}

		std::string numRight = GetNumberRight(group, i - 1);
		groupNumbers.push_back(group.substr(i, numRight.length()));
		i += numRight.length();
	}

	while (groupNumbers.size() > 0) {
		std::string replace = groupNumbers[groupNumbers.size()-1];
		groupNumbers.pop_back();

		group = ReplaceInString(group, replace, replace + " * " + num);
	}

	return group;
}
