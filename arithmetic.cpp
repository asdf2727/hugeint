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

	inline hugeint readNumber () {
		hugeint ans;
		skipSpaces();
		if (*parse == '(') {
			parse++;
			ans = readNumber();
			calcAdition(ans);
			parse++;
			return ans;
		}
		else {
			std::string::const_iterator start = parse;
			while (parse != equation.end() && isValid()) {
				parse++;
			}
			int errPos = ans.fromString(start, parse);
			if (errPos != -1) {
				// Error handling
			}
			return ans;
		}
	}

	void calcPower (hugeint &result) {
		hugeint temp;
		bool willContinue = readOperator(result, 3);
#ifdef L_TO_R_POWER

		while (willContinue) {
			parse++;
			temp = readNumber();
			willContinue = readOperator(temp, 3);
			result.pow(temp);
		}
#else
		std::vector <hugeint> numbers;
		while (willContinue) {
			parse++;
			temp = readNumber();
			willContinue = readOperator(temp, 3);
			numbers.push_back(temp);
		}
		while (numbers.size() > 1) {
			numbers[numbers.size() - 2].pow(numbers.back());
			numbers.pop_back();
		}
		result.pow(numbers[0]);
#endif
	}

	void calcMultiplication (hugeint &result) {
		int type = 0; // 0 - multiplcation, 1 - division, 2 - modulo
		hugeint temp;
		bool willContinue = readOperator(result, 2);
		while (willContinue) {
			skipSpaces();
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
			temp = readNumber();
			willContinue = readOperator(temp, 2);
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

	void calcAdition (hugeint &result) {
		int type = 0; // 0 - adition, 1 - subtraction
		hugeint temp;
		bool willContinue = readOperator(result, 1);
		while (willContinue) {
			skipSpaces();
			if (*parse == '+') {
				type = 0;
			}
			else {
				type = 1;
			}
			parse++;
			temp = readNumber();
			willContinue = readOperator(temp, 1);
			if (type == 0) {
				result += temp;
			}
			else {
				result -= temp;
			}
		}
	}

	inline bool readOperator (hugeint &number, int order) {
		bool modified;
		skipSpaces();

		// Execute while any smaller priority functions can be called
		do {
			modified = false;
			if (parse == equation.end() || *parse == ')') {
				break;
			}
			else if ((*parse == '+' || *parse == '-') && order < 1) {
				calcAdition(number);
				modified = true;
			}
			else if (*parse == '^' && order < 3) {
				calcPower(number);
				modified = true;
			}
			else if ((!isOperator() || *parse == '*' || *parse == '/' || *parse == '%') && order < 2) {
				calcMultiplication(number);
				modified = true;
			}
		} while (modified);

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
		parse = equation.begin();
		peakChar();
		hugeint result = readNumber();
		calcAdition(result);
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
	std::cout << "Hexadecimal: " << ans.toHex() << " done in " << global.reset() << " seconds" << std::endl;
	std::cout << "Decimal:     " << ans.toDec() << " done in " << global.reset() << " seconds" << std::endl;
	std::cout << "Octal:       " << ans.toOct() << " done in " << global.reset() << " seconds" << std::endl;
	std::cout << "Binary:      " << ans.toBin() << " done in " << global.reset() << " seconds" << std::endl;
}
