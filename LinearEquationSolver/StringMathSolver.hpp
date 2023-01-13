#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace StringMathSolver {
	std::string FormatEquation(std::string equation);
	std::string ReplaceInString(std::string base, std::string from, std::string to);

	std::string GetNumberRight(std::string s, int offset);
	std::string GetNumberLeft(std::string s, int offset);

	bool CanSolveAll(std::string group);
	bool HasGroup(std::string equation);
	bool HasExpression(std::string s);
	bool IsVariable(std::string s);
	bool IsOperator(char c);

	std::string OppositeOperator(char operation);
	std::string Encapsulate(std::string s);
	std::string IsolateVariables(std::string equation);

	std::string GetEquationRightSide(std::string equation);
	std::string GetEquationLeftSide(std::string equation);
	std::string GetHighierOrderGroup(std::string equation);
	std::string GetHighierOrderExpression(std::string group);

	std::string SolveEquation(std::string equation);
	std::string SolveEquationsGroups(std::string equation);
	std::string SolveEquationsExpressions(std::string equation);
	std::string SolveGroup(std::string group);
	std::string SolveExpression(std::string expression);

	std::string Mul(std::string a, std::string b);
	std::string Div(std::string a, std::string b);
	std::string Add(std::string a, std::string b);
	std::string Sub(std::string a, std::string b);

	std::string MulGroup(std::string group, std::string num);
};
