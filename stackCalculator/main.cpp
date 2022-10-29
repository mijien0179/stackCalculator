// Microsoft Visual Studio Community 2019 버전 16.11.5
// Windows SDK 버전 10.0
// 플랫폼 도구 집합: Visual Studio 2019(v142)
// C++ 언어 표준 : ISO C++ 14 표준
// 

#include <iostream>
#include <fstream>

#include "evaluator.h"

int main() {

	evaluator eval{};

	std::string expression;
	while (1) {
		expression = eval.read_expression();
		if (expression[0] == ':') {
			if (expression.substr(1).compare("exit") == 0) {
				break;
			}
		}
		std::cout << "convert infix to postfix: " << (expression = eval.convert_to_postfix(expression)) << std::endl;
		std::cout << "evaluate postfix return : " << eval.evaluate_postfix(expression) << std::endl;
		std::cout << "========================================================================" << std::endl << std::endl;
	}

	return 0;
}