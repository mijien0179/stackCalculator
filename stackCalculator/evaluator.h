#pragma once

#include "stack.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

class evaluator {
	/// <summary>
	/// 연산자 우선순위
	/// </summary>
	/// <param name="oper">우선 순위 찾을 연산자</param>
	/// <returns>연산자 우선순위 등급. 높을수록 우선시 됨</returns>
	int Priority(char oper) const;

	/// <summary>
	/// 사용자로부터 값을 받음
	/// </summary>
	/// <param name="varName">입력받을 변수 이름</param>
	/// <returns>변수를 대신할 값</returns>
	double InputVariableValue(std::string varName) const;

	// 사칙연산
	inline double plus(double a, double b);
	inline double minus(double a, double b);
	inline double multiple(double a, double b);
	inline double divide(double a, double b);

public:
	evaluator();

	/// <summary>
	/// infix 산술식 받아오기
	/// </summary>
	/// <returns>산술식 문자열</returns>
	std::string read_expression();

	/// <summary>
	/// infix 산술식을 postfix 산술식으로 변환
	/// </summary>
	/// <param name="s">변환할 infix 산술식</param>
	/// <returns>변환된 postfix 산술식</returns>
	std::string convert_to_postfix(std::string s);

	/// <summary>
	/// postfix 산술식 계산. 변수가 들어간 산술식은 변수 값을 받음
	/// </summary>
	/// <param name="s">postfix 산술식</param>
	/// <returns>평가된 값</returns>
	double evaluate_postfix(std::string s);

};