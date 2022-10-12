#include <iostream>
#include <vector>
#include <chrono>
#include <stack>

#include "hugeint.h"

//#define L_TO_R_POWER

class timer {
	std::chrono::steady_clock::time_point lastReset;
	std::chrono::duration <double> time;
public:
	timer () {
		lastReset = std::chrono::steady_clock::now();
	}
	double reset () {
		std::chrono::steady_clock::time_point reset_time = std::chrono::steady_clock::now();
		time = std::chrono::duration_cast <std::chrono::duration <double>>(reset_time - lastReset);
		lastReset = reset_time;
		return time.count();
	}
};

class calculator {
private:
	std::string::const_iterator parse;
	const char operators[6] = { '+', '-', '*', '/', '%', '^' };

	int errorID;
	std::string message;

	void skipSpaces () {
		while (*parse == ' ' && parse != equation.end()) {
			parse++;
		}
	}
	char getChar () {
		skipSpaces();
		char ans = *parse++;
		return ans;
	}

	bool isOperator () {
		for (char comp : operators) {
			if (comp == *parse) {
				return true;
			}
		}
		return false;
	}
	bool isSpecial () {
		return parse == equation.end() || isOperator() || *parse == '(' || *parse == ')' || *parse == ' ' || *parse == ',';
	}

	hugeint calcNumber () {
		hugeint ans;
		bool neg = false;
		skipSpaces();
		while (parse != equation.end() && (*parse == '+' || *parse == '-')) {
			neg ^= (*parse == '-');
			parse++;
			skipSpaces();
		}

		if ('0' <= *parse && *parse <= '9') {
			std::string::const_iterator start = parse;
			while (!isSpecial()) {
				parse++;
			}
			int errPos = ans.fromString(start, parse);
			if (errPos != -1) {
				errorID = 1;
				message = "Unrecognised character \'";
				message += *(start + errPos);
				message += "\' found in number";
				return 0;
			}
		}
		else {
			std::string func;
			std::vector <hugeint> params;
			while ('a' <= *parse && *parse <= 'z') {
				func += *parse;
				parse++;
			}

			if (*parse != '(') {
				errorID = 2;
				message = "Expected opening paranthesis for function \'" + func + "\'";
				return 0;
			}
			parse++;
			params.push_back(calcMember(0));
			while (*parse == ',') {
				parse++;
				params.push_back(calcMember(0));
			}
			if (parse == equation.end()) {
				errorID = 3;
				message = "Expected closing paranthesis for function \'" + func + "\'";
				return 0;
			}
			parse++;

			if (func == "abs") {
				if (params.size() != 1) {
					errorID = 4;
					message = "Function \'abs\' only accepts one parameter";
					return 0;
				}
				ans = abs(params[0]);
			}
			else if (func == "pow") {
				if (params.size() != 2) {
					errorID = 4;
					message = "Function \'pow\' only accepts two parameters";
					return 0;
				}
				ans = pow(params[0], params[1]);
			}
			else if (func == "sqrt") {
				if (params.size() != 1) {
					errorID = 4;
					message = "Function \'sqrt\' only accepts one parameter";
					return 0;
				}
				ans = sqrt(params[0]);
			}
			else if (func == "cbrt") {
				if (params.size() != 1) {
					errorID = 4;
					message = "Function \'cbrt\' only accepts one parameter";
					return 0;
				}
				ans = cbrt(params[0]);
			}
			else if (func == "nthroot") {
				if (params.size() != 2) {
					errorID = 4;
					message = "Function \'nthroot\' only accepts 2 parameters";
					return 0;
				}
				ans = nthroot(params[0], params[1]);
			}
			else if (func == "") {
				if (params.size() != 1) {
					errorID = 5;
					message = "Invalid use of \',\' in paranthesis";
					return 0;
				}
				ans = params[0];
			}
			else {
				errorID = 6;
				message = "Unrecognised function \'" + func + "\'";
				return 0;
			}

		}

		if (!errorID && neg) {
			ans.negate();
		}

		return ans;
	}

	hugeint calcMember (int priority) {
		hugeint member = calcNumber();
		skipSpaces();

		if (parse != equation.end() && *parse == '^' && priority < 3) {
			calcPower(member);
		}
		if (parse != equation.end() && (!isOperator() || *parse == '*' || *parse == '/' || *parse == '%') && priority < 2) {
			calcMultiplication(member);
		}
		if (parse != equation.end() && (*parse == '+' || *parse == '-') && priority < 1) {
			calcAdition(member);
		}

		return member;
	}
	inline bool readOperator (int priority) {
		skipSpaces();
		if (parse == equation.end() || *parse == ')' || *parse == ',') {
			return false;
		}
		if (*parse == '+' || *parse == '-') {
			return priority == 1;
		}
		if (*parse == '^') {
			return priority == 3;
		}
		// If no other operator was found, try aplying multiplication, maybe without the symbol
		return priority == 2;
	}

	void calcPower (hugeint &ans) {
#ifdef L_TO_R_POWER
		while (readOperator(3) && !errorID) {
			getChar();
			ans.pow(calcMember(3));
		}
#else
		std::vector <hugeint> numbers;
		numbers.push_back(ans);
		while (readOperator(3) && !errorID) {
			getChar();
			numbers.push_back(calcMember(3));
		}
		if (errorID) {
			return;
		}
		while (numbers.size() > 1) {
			numbers[numbers.size() - 2].pow(numbers.back());
			numbers.pop_back();
		}
		ans = numbers[0];
#endif
	}

	void calcMultiplication (hugeint &ans) {
		while (readOperator(2) && !errorID) {
			skipSpaces();
			if (*parse == '*') {
				parse++;
				ans *= calcMember(3);
			}
			else if (*parse == '/') {
				parse++;
				ans /= calcMember(3);
			}
			else if (*parse == '%') {
				parse++;
				ans %= calcMember(3);
			}
			else {
				ans *= calcMember(3);
			}
		}
	}
	void calcAdition (hugeint &ans) {
		while (readOperator(1) && !errorID) {
			skipSpaces();
			if (*parse == '+') {
				parse++;
				ans += calcMember(1);
			}
			else {
				parse++;
				ans -= calcMember(1);
			}
		}
	}
public:
	std::string equation;
	hugeint result;

	hugeint getResult () {
		errorID = 0;
		message = "";
		parse = equation.begin();
		result = calcMember(0);
		if (errorID) {
			std::cout << '#' << errorID << ": " << message << '\n';
		}
		return result;
	}
};

int main () {
	timer global;
	std::string read;
	calculator example;
	getline(std::cin, read);
	example.equation = read;
	global.reset();
	hugeint ans = example.getResult();
	std::cout << "Calculation time (s):" << global.reset() << std::endl;
	std::cout << "Answer:" << std::endl;
	std::cout << "\tHexadecimal: " << ans.toHex() << " in " << global.reset() << " seconds." << std::endl;
	std::cout << "\tDecimal:     " << ans.toDec() << " in " << global.reset() << " seconds." << std::endl;
	std::cout << "\tOctal:       " << ans.toOct() << " in " << global.reset() << " seconds." << std::endl;
	std::cout << "\tBinary:      " << ans.toBin() << " in " << global.reset() << " seconds." << std::endl;
}
