#include <iostream>
#include <vector>
#include <chrono>

#include "hugeint.h"

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

class calculator{
private:
	std::string equation;
	std::string::iterator parse;

	bool error;
	std::string err_msg;

	char peakChar(){
		while(parse != equation.end() && *parse == ' ') {
			parse++;
		}
		return *parse;
	}
	char getChar(){
		while(parse != equation.end() && *parse == ' ') {
			parse++;
		}
		if(parse == equation.end()){
			return *parse;
		}
		else{
			return *parse++;
		}
	}

	int getPriority(){
		if (parse == equation.end()) {
			return 0;
		}
		else {
			char now = peakChar();
			if(now == ')') {
				return 1;
			}
			else if(now == '+' || now == '-') {
				return 2;
			}
			else if(now == '*' || now == '/' || now == '%') {
				return 3;
			}
			else if('0' <= now && now <= '9'){
				return 4;
			}
			return 5;
		}
	}

	hugeint calcWord(){
		if(peakChar() == '('){
			parse++;
			return calcAdition();
		}
		else{
			hugeint result = 0;
			bool neg = false;
			if(peakChar() == '-'){
				neg = true;
				parse++;
			}
			while (getPriority() > 3) {
				if(getPriority() == 5){
					err_msg = (std::string)"Unrecognised symbol \'" + peakChar() + "\'.";
					break;
				}
				result = result * 10 + *parse - '0';
				parse++;
			}
			return (neg ? -result : result);
		}
	}

	hugeint calcMultiplication(){
		short int type = 0; // 0 - multiply, 1 - divide, 2 - modulo
		hugeint result = 1;
		char newChar;
		while(true) {
			// recursive call
			if(type == 0) {
				result *= calcWord();
			}
			else if(type == 1) {
				result /= calcWord();
			}
			else if(type == 2) {
				result %= calcWord();
			}
			// break if too high priority or if a syntax error was found
			if(getPriority() < 3 || error) {
				break;
			}
			// get the next sign
			newChar = peakChar();
			if(newChar == '*') {
				type = 0;
				parse++;
			}
			else if(newChar == '/') {
				type = 1;
				parse++;
			}
			else if(newChar == '%') {
				type = 2;
				parse++;
			}
			else {
				// Assume everything is multiplication if there is no symbol inbetween terms
				type = 0;
			}
		}
		return result;
	}

	hugeint calcAdition(){
		short int type = 0; // 0 - add, 1 - subtract
		hugeint result = 0;
		char newChar;
		if(*parse == '-'){
			type = 1;
			parse++;
		}
		while(true) {
			// recursive call
			if(type == 0) {
				result += calcMultiplication();
			}
			else if(type == 1) {
				result -= calcMultiplication();
			}
			// break if too high priority or if a syntax error was found
			if(getPriority() < 2 || error){
				if(getPriority() == 1){
					parse++;
				}
				break;
			}
			// get the next sign
			newChar = peakChar();
			if(newChar == '+') {
				type = 0;
				parse++;
			}
			else if(newChar == '-') {
				type = 1;
				parse++;
			}
			else {
				error = true;
				err_msg = (std::string)"Unrecognised symbol \'" + peakChar() + "\'.";
				result = 0;
				break;
			}
		}
		return result;
	}

public:
	void setEquation(const std::string &get_equation) {
		equation = get_equation;
	}
	hugeint getResult(){
		error = false;
		err_msg.clear();
		parse = equation.begin();
		peakChar();
		hugeint result = calcAdition();
		if (error) {
			std::cout << err_msg << std::endl;
			return 0;
		}
		else{
			return result;
		}
	}
};

int main () {
	timer global;
	std::string read;
	calculator function;
	getline(std::cin, read);
	function.setEquation(read);
	global.reset();
	hugeint ans = function.getResult();
	std::cout << "Answer: " << ans << std::endl;
	std::cout << "Calculation time (s):" << global.reset() << std::endl;
}
