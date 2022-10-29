#include "evaluator.h"

// 연산자 추가
// 일부 코드에 추가해 사용자 정의 연산자를 생성할 수 있습니다.
// 연산자 추가를 위한 작업은 주석에 [operator]가 포함됩니다.

int evaluator::Priority(char oper) const {
	// [operator]	
	switch (oper) {
		case '+':
		case '-':
			return 1;
		case '*':
		case '/':
			return 2;
		case '^':
			return 3;

	}

	return -1;
}

double evaluator::InputVariableValue(std::string varName) const
{
	double result;
	std::cout << "Enter ["<< std::setw(10) << varName << "] value: ";
	std::cin >> result;
	std::cin.ignore();

	return result;
}

inline double evaluator::plus(double a, double b)
{
	return a + b;
}

inline double evaluator::minus(double a, double b)
{
	return a - b;
}

inline double evaluator::multiple(double a, double b)
{
	return a * b;
}

inline double evaluator::divide(double a, double b)
{
	return a / b;
}

evaluator::evaluator()
{

}

std::string evaluator::read_expression()
{
	std::string str, result{};
	std::cout << "Enter infix expression  : ";
	const char spliter[] = {
		// [operator]
		'+', '-', '*', '/', '(', ')', ' ', '^'
	};

	std::getline(std::cin, str);


	size_t begin, pivot;
	for (begin = pivot = 0; pivot < str.length(); ++pivot) {
		for (size_t splitIdx = 0; splitIdx < (sizeof(spliter) / sizeof(spliter[0])); ++splitIdx) {
			if (spliter[splitIdx] == str[pivot]) {
				if (begin != pivot) result += str.substr(begin, pivot - begin) + " ";
				if (str[pivot] != ' ') result += std::string{ str[pivot] };

				begin = pivot + 1;
				break;
			}
		}
	}

	result += str.substr(begin, pivot - begin);

	return result;
}

std::string evaluator::convert_to_postfix(std::string s)
{
	using namespace std;

	size_t pivot;

	stack<string> numbers{};
	stack<char> operations{};

	string result{};

	// 한 글자씩 검색
	for (size_t cur = 0; cur < s.length(); ++cur) {
		switch (s[cur]) {
			case ' ': // 공백은 패스
				break;
			case '(': // 여는 괄호는 연산자 스택에 넣고 패스
				operations.push('(');
				break;
			// [operator]
			case '+': case '-':
			case '*': case '/':
			case '^':
				if (operations.empty()) {
					operations.push(s[cur]);
					break;
				} // 스택이 비어있으면 넣고 패스

				if (operations.top() == '(') {
					operations.push(s[cur]);
					break;
				} // 가장 최상위 oper가 괄호면 넣고 패스

				while (!operations.empty() && Priority(operations.top()) >= Priority(s[cur])) {
					// 스택이 비어있지 않고 스택의 연산자 우선순위가 같거나 높을때
					result.append(string{ operations.pop() } + " ");
					// 계속 뺌
				}

				operations.push(s[cur]);
				// 현재 연산자를 스택에 넣음

				break;
			case ')': // 닫는 괄호를 만난 경우
				while (/*!operations.empty() && */operations.top() != '(') {
					// 스택에서 여는 괄호를 만날 때까지
					result.append(string{ operations.pop() } + " ");
					// 스택의 연산자를 넣음
				}
				operations.pop();
				// 여는 괄호를 꺼냄
				break;
			default: // 변수, 상수 값
				pivot = cur + 1;

				// 닫는 괄호, 공백, 문자열의 끝 위치를 찾음
				while (s[pivot] != ')' && s[pivot] != ' ' && pivot < s.length()) ++pivot;

				// 해당 값을 postfix 산술식에 추가함
				result.append(s.substr(cur, pivot - cur) + " ");
				cur = pivot - 1;
		}
	}

	// 연산자 스택을 postfix 산술식에 모두 추가함
	while (!operations.empty()) {
		result.append(string{ operations.pop() } + " ");
	}

	return result;
}

double evaluator::evaluate_postfix(std::string s)
{
	double (evaluator:: * fp[4])(double, double) = {
	&evaluator::multiple, &evaluator::plus,
	&evaluator::minus, &evaluator::divide
	};
	size_t pivot;
	int fn;
	double operand1, operand2;
	stack<double> numbers{};
	std::string temp;
	std::vector<std::pair<std::string, double>> varList;
	std::vector<std::pair<std::string, double>>::iterator varListIter;

	// 산술식 탐색
	for (size_t cur = 0; cur < s.length(); ++cur) {
		switch (s[cur]) {
			case '+':
			case '-': // 연산자를 만나면
			case '*':
			case '/':
				fn = (s[cur] - 41) >> 1;
				/* + : 1    - : 2
				// * : 0    / : 3  */

				// 숫자 스택에서 두 개를 꺼내서
				operand2 = numbers.pop();
				operand1 = numbers.pop();

				// 그에 맞는 연산을 진행한 뒤 숫자 스택에 저장
				numbers.push((this->*fp[fn])(operand1, operand2));
			
				break;
			case '^':
				// 숫자 스택에서 두 개를 꺼내서
				operand2 = numbers.pop();
				operand1 = numbers.pop();

				numbers.push(pow(operand1, operand2));
				break;
			// [operator]



			case ' ': // 공백은 패스
				break;
			default: // 변수, 상수를 만난 경우
				pivot = cur + 1;
				while (s[pivot] != ' ' && pivot < s.length()) ++pivot;

				temp = s.substr(cur, pivot - cur);

				if (isdigit(s[cur]) || s[cur] == '.') numbers.push(std::stod(temp)); // 상수인 경우 parsing해서 바로 숫자 스택에 넣음
				else { // 변수인 경우 parsing해서 숫자 입력받도록 처리함
					for (varListIter = varList.begin(); varListIter != varList.end(); ++varListIter) {
						if (varListIter->first.compare(temp) == 0) {
							// 이미 입력한 변수라면 해당 값을 가져옴
							operand1 = varListIter->second;
							break;
						}
					}
					if (varListIter == varList.end()) {
						// 변수 리스트에 없는 변수명이면 새로 값을 받아서 저장
						operand1 = InputVariableValue(temp);
						varList.push_back(std::pair<std::string, double>{temp, operand1});
					}

					numbers.push(operand1);
					// (입력 받은)변수값 스택에 저장

				}
				cur = pivot;
		}
	}
	return numbers.pop();
}
