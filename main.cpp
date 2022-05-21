#include <iostream>
#include <fstream>
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

hugeint logPow (hugeint base, hugeint pow) {
	hugeint ans = 1;
	while (pow > 0) {
		if ((pow & 1) == 1) {
			ans *= base;
		}
		base *= base;
		pow >>= 1;
	}
	return ans;
}

class calculator{
private:
	std::string equation;
	std::string::iterator parse;

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
			while (getPriority() > 3) {
				if(getPriority() == 5){
					// TODO error handling
					break;
				}
				result = result * 10 + *parse - '0';
				parse++;
			}
			return result;
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
			// break if too high priority
			if(getPriority() < 3) {
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
			// break if too high priority
			if(getPriority() < 2){
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
				// TODO error handling
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
		parse = equation.begin();
		peakChar();
		return calcAdition();
	}
};

int main () {
	std::ofstream fout("file.txt");
	timer global;
	std::string read;
	calculator function;
	getline(std::cin, read);
	function.setEquation(read);
	global.reset();
	hugeint ans = function.getResult();
	std::cout << "Calculation time (s):" << global.reset() << std::endl;
	std::cout << "Answer: " << ans << std::endl;
	std::cout << "Output time (s):" << global.reset() << std::endl;
	std::cout.flush();
}
