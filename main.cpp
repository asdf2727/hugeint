#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "hugeint.h"

typedef unsigned int uint;

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

hugeint logPow (hugeint base, uint pow) {
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

int main () {
	std::ofstream fout("file.txt");
	timer global;
	hugeint nr1;
	uint nr2;
	std::cin >> nr1 >> nr2;
	global.reset();
	hugeint ans = logPow(nr1, nr2);
	std::cout << global.reset() << '\n';
	fout << ans << '\n';
	std::cout << global.reset();
}
