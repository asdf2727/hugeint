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

	void skipSpaces () {
		while (*parse == ' ' && parse != equation.end()) {
			parse++;
		}
	}
	char peakChar () {
		skipSpaces();
		return *parse;
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

	bool isValid () {
		return parse != equation.end() && !isOperator() && *parse != '(' && *parse != ')' && *parse != ' ';
	}

	inline hugeint readNumber (bool &error) {
		hugeint ans;
		skipSpaces();
		if (*parse == '(') {
			parse++;
			ans = readNumber(error);
			calcAdition(ans, error);
			if (parse == equation.end()) {
				error = true;
			}
			else {
				parse++;
			}
			return ans;
		}
		else {
			std::string::const_iterator start = parse;
			while (parse != equation.end() && isValid()) {
				parse++;
			}
			int errPos = ans.fromString(start, parse);
			if (errPos != -1) {
				error = true;
				parse = start + errPos;
			}
			return ans;
		}
	}

	void calcPower (hugeint &result, bool &error) {
		hugeint temp;
		bool willContinue = readOperator(result, 3, error);
#ifdef L_TO_R_POWER
		while (willContinue) {
			parse++;
			temp = readNumber(error);
			if(error) {
				break;
			}
			willContinue = readOperator(temp, 3, error);
			if(error) {
				break;
			}
			result.pow(temp);
		}
#else
		std::vector <hugeint> numbers;
		while (willContinue) {
			parse++;
			temp = readNumber(error);
			if (error) {
				break;
			}
			willContinue = readOperator(temp, 3, error);
			if (error) {
				break;
			}
			numbers.push_back(temp);
		}
		if (error) {
			return;
		}
		while (numbers.size() > 1) {
			numbers[numbers.size() - 2].pow(numbers.back());
			numbers.pop_back();
		}
		result.pow(numbers[0]);
#endif
	}

	void calcMultiplication (hugeint &result, bool &error) {
		int type = 0; // 0 - multiplcation, 1 - division, 2 - modulo
		hugeint temp;
		bool willContinue = readOperator(result, 2, error);
		while (willContinue) {
			if (*parse == '*') {
				type = 0;
			}
			else if (*parse == '/') {
				type = 1;
			}
			else if (*parse == '%') {
				type = 2;
			}
			else {
				type = 0;
				parse--;
			}
			parse++;
			temp = readNumber(error);
			if (error) {
				break;
			}
			willContinue = readOperator(temp, 2, error);
			if (error) {
				break;
			}
			if (type == 0) {
				result *= temp;
			}
			else if (type == 1) {
				result /= temp;
			}
			else {
				result %= temp;
			}
		}
	}

	void calcAdition (hugeint &result, bool &error) {
		int type = 0; // 0 - adition, 1 - subtraction
		hugeint temp;
		bool willContinue = readOperator(result, 1, error);
		while (willContinue) {
			if (*parse == '+') {
				type = 0;
			}
			else {
				type = 1;
			}
			parse++;
			temp = readNumber(error);
			if (error) {
				break;
			}
			willContinue = readOperator(temp, 1, error);
			if (error) {
				break;
			}
			if (type == 0) {
				result += temp;
			}
			else {
				result -= temp;
			}
		}
	}

	inline bool readOperator (hugeint &number, int order, bool &error) {
		bool modified;
		skipSpaces();

		// Execute while any smaller priority functions can be called
		do {
			modified = false;
			if (parse == equation.end() || *parse == ')') {
				break;
			}
			else if ((*parse == '+' || *parse == '-') && order < 1) {
				calcAdition(number, error);
				modified = true;
			}
			else if (*parse == '^' && order < 3) {
				calcPower(number, error);
				modified = true;
			}
			else if ((!isOperator() || *parse == '*' || *parse == '/' || *parse == '%') && order < 2) {
				calcMultiplication(number, error);
				modified = true;
			}
		} while (modified && !error);
		if (error) {
			return false;
		}
		// Then see if you should continue
		if (parse == equation.end() || *parse == ')') {
			return false;
		}
		if (*parse == '+' || *parse == '-') {
			return order == 1;
		}
		if (*parse == '^') {
			return order == 3;
		}
		// If no other operator was found, try aplying multiplication, maybe without the symbol
		return order == 2;
	}
public:
	std::string equation;

	hugeint getResult () {
		bool error = false;
		parse = equation.begin();
		hugeint result = readNumber(error);
		if (!error) {
			calcAdition(result, error);
		}
		if (parse != equation.end()) {
			std::cout << "Unrecognised character \'" << *parse << "\'!\n";
		}
		else if (error) {
			std::cout << "Missing closing paranthesis!\n";
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
	std::cout << "Hexadecimal: " << ans.toHex() << " in " << global.reset() << " seconds." << std::endl;
	std::cout << "Decimal:     " << ans.toDec() << " in " << global.reset() << " seconds." << std::endl;
	std::cout << "Octal:       " << ans.toOct() << " in " << global.reset() << " seconds." << std::endl;
	std::cout << "Binary:      " << ans.toBin() << " in " << global.reset() << " seconds." << std::endl;
	std::cout << "Square root: " << ans.sqrt() << " in " << global.reset() << " seconds." << std::endl;
	std::cout << "Cubic root:  " << ans.cbrt() << " in " << global.reset() << " seconds." << std::endl;
}
