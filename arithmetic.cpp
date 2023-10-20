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
	__gnu_cxx::__normal_iterator <char *, std::basic_string <char>> parse;
	const char operators[6] = { '+', '-', '*', '/', '%', '^' };

	int error_id;
	std::string message;

	void skipSpaces () {
		while (*parse == ' ' && parse != equation.end()) {
			parse++;
		}
	}
	inline void nextChar () {
		parse++;
		skipSpaces();
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
		hugeint ans = 0;
		int errPos;
		bool neg = false;
		std::string::iterator start = parse;
		std::string str;

		/// Read string
		while (parse != equation.end() && (*parse == '+' || *parse == '-')) {
			neg ^= (*parse == '-');
			nextChar();
		}
		while (!isSpecial()) {
			str.push_back(*parse);
			parse++;
		}

		// Try interpreting as a number
		if (!str.empty()) {
			errPos = ans.fromString(str.begin(), str.end());
			skipSpaces();
			if (errPos == -1) {
				if (neg) {
					ans.negate();
				}
				return ans;
			}
		}

		/// If it isn't a number, try interpreting as a function or as brackets
		std::vector <hugeint> params;
		bool has_brackets = false;

		if (*parse == '(') {
			has_brackets = true;
			nextChar();
			if (*parse != ')') {
				params.push_back(calcMember(0));
				while (!error_id && parse != equation.end() && *parse == ',') {
					nextChar();
					params.push_back(calcMember(0));
				}
				if (error_id) {
					return 0;
				}
			}
			if (parse == equation.end()) {
				message = "Expected closing bracket for " + (str.empty() ? "operation" : "function \'" + str + "\'");
				error_id = 2;
				return 0;
			}
			if (*parse != ')') {
				error_id = 1;
				return 0;
			}
			nextChar();
		}

		if (str.empty()) {
			if (params.size() == 0) {
				message = "Void numbers are invalid. Try putting \'0\' instead";
				error_id = 5;
				return 0;
			}
			if (params.size() > 1) {
				message = "Invalid use of \',\' inside brackets";
				error_id = 4;
				return 0;
			}
			ans = params.empty() ? (hugeint)0 : params[0];
		}
		else if (str == "fact") {
			if (params.size() > 1) {
				message = "Function \'fact\' only accepts 1 parameter";
				error_id = 7;
				return 0;
			}
			hugeint ans = 1;
			for (int index = 2; index <= (int)params[0]; index++) {
				ans *= index;
			}
			return ans;
		}
		else if (str == "rand") {
			if (params.size() > 1) {
				message = "Function \'rand\' only accepts 0 or 1 parameters";
				error_id = 7;
				return 0;
			}
			ans.rand((params.empty() ? 32 : (size_t)params[0]), false);
		}
		else if (str == "gcd") {
			if (params.size() != 2) {
				message = "Function \'gcd\' only accepts 2 parameters";
				error_id = 7;
				return 0;
			}
			ans = huge::gcd(params[0], params[1]);
		}
		else if (str == "abs") {
			if (params.size() != 1) {
				message = "Function \'abs\' only accepts 1 parameter";
				error_id = 7;
				return 0;
			}
			ans = huge::abs(params[0]);
		}
		else if (str == "pow") {
			if (params.size() != 2) {
				message = "Function \'pow\' only accepts 2 parameters";
				error_id = 7;
				return 0;
			}
			ans = huge::pow(params[0], (int)params[1]);
		}
		else if (str == "sqrt") {
			if (params.size() != 1) {
				message = "Function \'sqrt\' only accepts 1 parameter";
				error_id = 7;
				return 0;
			}
			ans = huge::sqrt(params[0]);
		}
		else if (str == "cbrt") {
			if (params.size() != 1) {
				message = "Function \'cbrt\' only accepts 1 parameter";
				error_id = 7;
				return 0;
			}
			ans = huge::cbrt(params[0]);
		}
		else if (str == "nthroot") {
			if (params.size() != 2) {
				message = "Function \'nthroot\' only accepts 2 parameters";
				error_id = 7;
				return 0;
			}
			ans = huge::nthroot(params[0], params[1]);
		}
		else {
			if (has_brackets) {
				message = "Unrecognised function \'" + str + "\' found";
				error_id = 6;
			}
			else {
				message = "Unrecognised \'";
				message += *(start + errPos);
				message += "\' character found in word \'";
				message += str;
				message += "\'";
				error_id = 1;
			}
			return 0;
		}

		if (neg) {
			ans.negate();
		}

		return ans;
	}

	hugeint calcMember (int priority) {
		hugeint member = calcNumber();

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
		while (readOperator(3) && !error_id) {
			nextChar();
			ans.pow(calcMember(3));
		}
#else
		std::vector <hugeint> numbers;
		numbers.push_back(ans);
		while (readOperator(3) && !error_id) {
			nextChar();
			numbers.push_back(calcMember(3));
		}
		if (error_id) {
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
		while (readOperator(2) && !error_id) {
			if (*parse == '*') {
				nextChar();
				ans *= calcMember(2);
			}
			else if (*parse == '/') {
				nextChar();
				ans /= calcMember(2);
			}
			else if (*parse == '%') {
				nextChar();
				ans %= calcMember(2);
			}
			else {
				ans *= calcMember(2);
			}
		}
	}
	void calcAdition (hugeint &ans) {
		while (readOperator(1) && !error_id) {
			if (*parse == '+') {
				nextChar();
				ans += calcMember(1);
			}
			else {
				nextChar();
				ans -= calcMember(1);
			}
		}
	}
public:
	/*
	Error codes:
		0 - All clear
		1 - Unrecognised character
		2 - expected closing bracket
		3 - unmached closing bracket
		4 - invalid ','
		5 - void numbers invalid
		6 - invalid function name
		7 - function only accepts n params
	*/

	std::string equation;
	hugeint result;

	hugeint getResult (std::string &error) {
		error_id = 0;
		message = "";
		parse = equation.begin();
		skipSpaces();
		result = calcMember(0);
		if (!error_id && parse != equation.end()) {
			if (*parse == ')') {
				error_id = 3;
				message = "Unmatched closing bracket found";
			}
			else if (*parse == ',') {
				error_id = 4;
				message = "Invalid use of \',\' in operation";
			}
			else {
				error_id = 1;
				message = "Unrecognised \'";
				message += *parse;
				message += "\' character found";
			}
			result = 0;
		}
		if (error_id) {
			error = "#" + std::to_string(error_id) + ": " + message;
		}
		else {
			error = "";
		}
		return result;
	}
};

int main () {
	timer global;
	std::string read, get_error;
	calculator example;

	getline(std::cin, read);
	example.equation = read;

	global.reset();
	hugeint ans = example.getResult(get_error);

	std::cout << "Calculation time (s):" << global.reset() << std::endl;
	if (!get_error.empty()) {
		std::cout << get_error << '\n';
	}
	else {
		std::cout << "Answer:" << std::endl;
		std::cout << "\tDouble:      " << ans << " in " << global.reset() << " seconds." << std::endl;
		std::cout << "\tHexadecimal: " << ans.toHex() << " in " << global.reset() << " seconds." << std::endl;
		std::cout << "\tDecimal:     " << ans.toDec() << " in " << global.reset() << " seconds." << std::endl;
		std::cout << "\tOctal:       " << ans.toOct() << " in " << global.reset() << " seconds." << std::endl;
		std::cout << "\tBinary:      " << ans.toBin() << " in " << global.reset() << " seconds." << std::endl;
	}
}