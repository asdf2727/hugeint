#include <iostream>
#include <bitset>
#include <chrono>

#include "hugeint.h"

#define N_MAX 10000000

typedef unsigned long long int ullint;

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

int main(){
	timer time;
	hugeint cnt = 0;
	std::bitset <N_MAX + 1> is_comp;
	is_comp.reset();
	time.reset();
	for (ullint index = 2; index <= N_MAX; index++) {
		if (!is_comp[(ullint)index]) {
			cnt++;
			for(ullint index2 = index * index; index2 <= N_MAX; index2 += index) {
				is_comp[(ullint)index2] = true;
			}
		}
	}
	std::cout << cnt << ' ' << time.reset() << std::endl;
}