#pragma once

#include "stack.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

class evaluator {
	/// <summary>
	/// ������ �켱����
	/// </summary>
	/// <param name="oper">�켱 ���� ã�� ������</param>
	/// <returns>������ �켱���� ���. �������� �켱�� ��</returns>
	int Priority(char oper) const;

	/// <summary>
	/// ����ڷκ��� ���� ����
	/// </summary>
	/// <param name="varName">�Է¹��� ���� �̸�</param>
	/// <returns>������ ����� ��</returns>
	double InputVariableValue(std::string varName) const;

	// ��Ģ����
	inline double plus(double a, double b);
	inline double minus(double a, double b);
	inline double multiple(double a, double b);
	inline double divide(double a, double b);

public:
	evaluator();

	/// <summary>
	/// infix ����� �޾ƿ���
	/// </summary>
	/// <returns>����� ���ڿ�</returns>
	std::string read_expression();

	/// <summary>
	/// infix ������� postfix ��������� ��ȯ
	/// </summary>
	/// <param name="s">��ȯ�� infix �����</param>
	/// <returns>��ȯ�� postfix �����</returns>
	std::string convert_to_postfix(std::string s);

	/// <summary>
	/// postfix ����� ���. ������ �� ������� ���� ���� ����
	/// </summary>
	/// <param name="s">postfix �����</param>
	/// <returns>�򰡵� ��</returns>
	double evaluate_postfix(std::string s);

};