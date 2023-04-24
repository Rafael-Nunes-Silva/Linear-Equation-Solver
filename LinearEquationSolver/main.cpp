#include <iostream>
#include <string>
#include "StringMathSolver.hpp"

/* Inputs de teste                   | resultado esperado
* 6(2x + 3) + x - 7 = 3(5x + 7) + 2x | x = -2.5
* x = (1 + 3 * 5) / (1 + 3)          | x = 4
* 2x + 3(18 / 4) = 7x - 45           | x = 11.7
* 7x + 3 = 2x + 20                   | x = 3.4
*/

bool closeProgram = false;

char GetCharInput() {
	char input = getchar();
	getchar();
	return input;
}
std::string GetEquation() {
	std::cout << "\nType the equation: ";
	std::string equation;
	std::getline(std::cin, equation);
	return equation;
}

const std::pair<std::string, void(*)()> optionsList[] = {
	{ "Solve Equation", []() { std::cout << StringMathSolver::SolveEquation(GetEquation()) << "\n\n"; }},
	{ "Close Program" , []() { closeProgram = true; } }
};

void DisplayMenu() {
	for (int i = 0; i < sizeof(optionsList) / sizeof(std::pair<std::string, void*>); i++) {
		std::cout << i << " - " << optionsList[i].first << "\n";
	}
	std::cout << ": ";
	std::string input = "?";
	std::getline(std::cin, input);

	int choice = 0;
	try {
		choice = std::stoi(input);
	}
	catch(std::exception e){
		// std::cout << e.what << "\n";
		std::cout << "Invalid option\n";
		system("cls");
		DisplayMenu();
		return;
	}

	if (choice < 0) {
		std::cout << "Invalid option\n";
		system("cls");
		DisplayMenu();
		return;
	}

	optionsList[choice].second();
}

int main() {
	std::cout << "Linear Equation Solver\n";
	do {
		DisplayMenu();
	} while (!closeProgram);

	return 0;
}
