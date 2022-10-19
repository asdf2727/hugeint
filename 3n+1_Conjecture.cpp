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

inline void next1 (unsigned long long int &val) {
	val += (val & 1 ? (val << 1) + 1 : -(val >> 1));
}
inline void next2 (hugeint &val) {
	val += (val.getBit(0) ? (val << 1) + 1 : -(val >> 1));
}

// Checked: 563800001

int main () {
	timer global;
	hugeint copy2;
	unsigned long long int num = 857000000003, copy1;

	global.reset();

	for (int cnt = 1; true; cnt++, num += 2) {
		copy1 = num + (num + 1) / 2;
		while (num < copy1 && copy1 <= ULONG_LONG_MAX / 3 - 1) {
			next1(copy1);
		}
		if (copy1 > num) {
			copy2 = copy1;
			//std::cout << num << " turned huge: " << copy1 << "\n";
			while (num < copy2) {
				next2(copy2);
			}
			copy1 = (unsigned long long int)copy2;
		}
		if (copy1 == num) {
			std::cout << num << " CYCLE!\n";
			break;
		}

		if (cnt == 50000000) {
			std::cout << num << " OK\n";
			cnt = 0;
		}
	}

	std::cout << global.reset();
}