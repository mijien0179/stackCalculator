#include "evaluator.h"

// ������ �߰�
// �Ϻ� �ڵ忡 �߰��� ����� ���� �����ڸ� ������ �� �ֽ��ϴ�.
// ������ �߰��� ���� �۾��� �ּ��� [operator]�� ���Ե˴ϴ�.

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

	// �� ���ھ� �˻�
	for (size_t cur = 0; cur < s.length(); ++cur) {
		switch (s[cur]) {
			case ' ': // ������ �н�
				break;
			case '(': // ���� ��ȣ�� ������ ���ÿ� �ְ� �н�
				operations.push('(');
				break;
			// [operator]
			case '+': case '-':
			case '*': case '/':
			case '^':
				if (operations.empty()) {
					operations.push(s[cur]);
					break;
				} // ������ ��������� �ְ� �н�

				if (operations.top() == '(') {
					operations.push(s[cur]);
					break;
				} // ���� �ֻ��� oper�� ��ȣ�� �ְ� �н�

				while (!operations.empty() && Priority(operations.top()) >= Priority(s[cur])) {
					// ������ ������� �ʰ� ������ ������ �켱������ ���ų� ������
					result.append(string{ operations.pop() } + " ");
					// ��� ��
				}

				operations.push(s[cur]);
				// ���� �����ڸ� ���ÿ� ����

				break;
			case ')': // �ݴ� ��ȣ�� ���� ���
				while (/*!operations.empty() && */operations.top() != '(') {
					// ���ÿ��� ���� ��ȣ�� ���� ������
					result.append(string{ operations.pop() } + " ");
					// ������ �����ڸ� ����
				}
				operations.pop();
				// ���� ��ȣ�� ����
				break;
			default: // ����, ��� ��
				pivot = cur + 1;

				// �ݴ� ��ȣ, ����, ���ڿ��� �� ��ġ�� ã��
				while (s[pivot] != ')' && s[pivot] != ' ' && pivot < s.length()) ++pivot;

				// �ش� ���� postfix ����Ŀ� �߰���
				result.append(s.substr(cur, pivot - cur) + " ");
				cur = pivot - 1;
		}
	}

	// ������ ������ postfix ����Ŀ� ��� �߰���
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

	// ����� Ž��
	for (size_t cur = 0; cur < s.length(); ++cur) {
		switch (s[cur]) {
			case '+':
			case '-': // �����ڸ� ������
			case '*':
			case '/':
				fn = (s[cur] - 41) >> 1;
				/* + : 1    - : 2
				// * : 0    / : 3  */

				// ���� ���ÿ��� �� ���� ������
				operand2 = numbers.pop();
				operand1 = numbers.pop();

				// �׿� �´� ������ ������ �� ���� ���ÿ� ����
				numbers.push((this->*fp[fn])(operand1, operand2));
			
				break;
			case '^':
				// ���� ���ÿ��� �� ���� ������
				operand2 = numbers.pop();
				operand1 = numbers.pop();

				numbers.push(pow(operand1, operand2));
				break;
			// [operator]



			case ' ': // ������ �н�
				break;
			default: // ����, ����� ���� ���
				pivot = cur + 1;
				while (s[pivot] != ' ' && pivot < s.length()) ++pivot;

				temp = s.substr(cur, pivot - cur);

				if (isdigit(s[cur]) || s[cur] == '.') numbers.push(std::stod(temp)); // ����� ��� parsing�ؼ� �ٷ� ���� ���ÿ� ����
				else { // ������ ��� parsing�ؼ� ���� �Է¹޵��� ó����
					for (varListIter = varList.begin(); varListIter != varList.end(); ++varListIter) {
						if (varListIter->first.compare(temp) == 0) {
							// �̹� �Է��� ������� �ش� ���� ������
							operand1 = varListIter->second;
							break;
						}
					}
					if (varListIter == varList.end()) {
						// ���� ����Ʈ�� ���� �������̸� ���� ���� �޾Ƽ� ����
						operand1 = InputVariableValue(temp);
						varList.push_back(std::pair<std::string, double>{temp, operand1});
					}

					numbers.push(operand1);
					// (�Է� ����)������ ���ÿ� ����

				}
				cur = pivot;
		}
	}
	return numbers.pop();
}
