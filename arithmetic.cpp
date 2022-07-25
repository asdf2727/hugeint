#include <iostream>
#include <vector>
#include <chrono>
#include <stack>

#include "hugeint.h"

#define L_TO_R_POWER

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
	std::string equation;
	std::string::iterator parse;

	bool error;
	std::string err_msg;

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
		skipSpaces();
		return ans;
	}

	int getPriority () {
		if (parse == equation.end()) {
			return 0;
		}
		else {
			char now = *parse;
			if (now == ')') {
				return 1;
			}
			else if (now == '+' || now == '-') {
				return 2;
			}
			else if (now == '*' || now == '/' || now == '%') {
				return 3;
			}
			else if (now == '^') {
				return 4;
			}
			else if ('0' <= now && now <= '9') {
				return 5;
			}
			return 6;
		}
	}

	hugeint calcWord () {
		if (peakChar() == '(') {
			getChar();
			return calcAdition();
		}
		else {
			bool neg = false;
			if (peakChar() == '-') {
				getChar();
				neg = true;
			}
			std::string::const_iterator start = parse;
			while (getPriority() > 4) {
				parse++;
			}
			hugeint result;
			if (!error && !result.fromString(start, parse)) {
				err_msg = (std::string)"Unrecognised symbol inside a number.";
				error = true;
			}
			return result;
		}
	}

	hugeint calcPower () {
		hugeint result = calcWord();
#ifdef L_TO_R_POWER
		while (true) {
			peakChar();
			// break if wrong priority or if a syntax error was found
			if (getPriority() != 4 || error) {
				break;
			}
			getChar();
			result.pow(calcWord());
		}
#else
		peakChar();
		// break if wrong priority or if a syntax error was found
		if (getPriority() != 4 || error) {
			return result;
		}
		getChar();
		hugeint exp = calcPower();
		if (exp != (int)exp) {
			error = true;
			err_msg = "Result is too big!";
		}
		else {
			result.pow(exp);
		}
#endif
		return result;
	}

	hugeint calcMultiplication () {
		short int type = 0; // 0 - multiply, 1 - divide, 2 - modulo
		hugeint result = 1;
		char newChar;
		while (true) {
			// recursive call
			if (type == 0) {
				result *= calcPower();
			}
			else if (type == 1) {
				result /= calcPower();
			}
			else if (type == 2) {
				result %= calcPower();
			}
			newChar = peakChar();
			// break if wrong priority or if a syntax error was found
			if (getPriority() != 3 || error) {
				break;
			}
			// get the next sign
			if (newChar == '*') {
				type = 0;
				getChar();
			}
			else if (newChar == '/') {
				type = 1;
				getChar();
			}
			else if (newChar == '%') {
				type = 2;
				getChar();
			}
			else {
				// Assume everything is multiplication if valid operator follows
				// This allows for some cool stuff like 2(1 + 1) = 4 or wierd stuff like (5 5) = 25
				type = 0;
			}
		}
		return result;
	}

	hugeint calcAdition () {
		short int type = 0; // 0 - add, 1 - subtract
		hugeint result = 0;
		char newChar;
		while (true) {
			// recursive call
			if (type == 0) {
				result += calcMultiplication();
			}
			else if (type == 1) {
				result -= calcMultiplication();
			}
			newChar = peakChar();
			// break if wrong priority or if a syntax error was found
			if (getPriority() != 2 || error) {
				if (getPriority() == 1) {
					parse++;
				}
				break;
			}
			// get the next sign
			if (newChar == '+') {
				type = 0;
				getChar();
			}
			else if (newChar == '-') {
				type = 1;
				getChar();
			}
			else {
				err_msg = (std::string)"Unrecognised symbol \'" + *parse + "\'.";
				error = true;
				break;
			}
		}
		return result;
	}

public:
	void setEquation (const std::string &get_equation) {
		equation = get_equation;
	}
	hugeint getResult () {
		error = false;
		err_msg.clear();
		parse = equation.begin();
		peakChar();
		hugeint result = calcAdition();
		if (error) {
			std::cout << err_msg << std::endl;
			return 0;
		}
		else {
			return result;
		}
	}
};

int main () {
	timer global;
	std::string read;
	calculator example;
	getline(std::cin, read);
	example.setEquation(read);
	global.reset();
	hugeint ans = example.getResult();
	std::cout << "Calculation time (s):" << global.reset() << std::endl;
	std::cout << "Answer: " << std::endl;
	std::cout << "Hexadecimal: " << ans.toHex() << std::endl;
	std::cout << "Decimal:     " << ans.toDec() << std::endl;
	std::cout << "Octal:       " << ans.toOct() << std::endl;
	std::cout << "Binary:      " << ans.toBin() << std::endl;
	std::cout << "Output time (s):" << global.reset() << std::endl;

}
