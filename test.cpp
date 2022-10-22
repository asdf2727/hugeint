#include <iostream>
#include <vector>
#include <chrono>
#include <stack>
#include <climits>

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

int main () {
	timer global;
	hugeint ans = 1;
	for (hugeint val = 2; val <= 500; val++) {
		ans *= val;
	}
	std::cout << ans.toHex() << '\n';
	std::cout << global.reset();
}