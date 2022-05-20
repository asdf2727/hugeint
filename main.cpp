#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>

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

struct str {
	uint val;
	uint pow;
};
//Decomp fact is a very cool idea and briliant implementation (by me)...
//usless because its to slow doe
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
hugeint decompFact (uint fact) {
	std::vector <bool> iscomp;
	std::vector <str> vec;
	uint copy;
	hugeint ans = 1;
	iscomp.resize(fact + 1);
	for (uint index = 2; index * index <= fact; index++) {
		if (!iscomp[index]) {
			for (uint index2 = index * index; index2 * index2 <= fact; index2 += index) {
				iscomp[index2] = true;
			}
			vec.push_back({ index, 0 });
		}
	}
	for (uint index = 2; index <= fact; index++) {
		copy = index;
		for (str &prim : vec) {
			while (copy % prim.val == 0) {
				prim.pow++;
				copy /= prim.val;
			}
			if (copy == 1) {
				break;
			}
		}
		if (copy > 1) {
			ans *= copy;
		}
	}
	for (str now : vec) {
		ans *= logPow(now.val, now.pow);
	}
	return ans;
}

hugeint normFact (uint fact) {
	hugeint ans = 1;
	for (uint index = 2; index <= fact; index++) {
		ans *= index;
	}
	return ans;
}

timer global;

int main () {
	std::ofstream fout("file.txt");
	hugeint nr1, nr2;
	std::cin >> nr1 >> nr2;
	global.reset();
	hugeint ans = logPow(nr1, (uint)nr2);
	std::cout << global.reset() << '\n';
	std::cout << ans << '\n';
	std::cout << global.reset();
}
