#include <iostream>
#include <vector>
#include <chrono>
#include <stack>
#include <climits>
#include <bitset>

#include "hugeint.h"

class timer {
private:
	bool running = false;
	std::chrono::steady_clock::time_point lastStart;
	double time;

	double since_pause () {
		return (running ? std::chrono::duration_cast <std::chrono::duration <double>>(std::chrono::steady_clock::now() - lastStart).count() : 0);
	};

public:
	timer () {
		time = 0;
		running = false;
	}
	double reset () {
		double ans = time + since_pause();
		time = 0;
		running = false;
		return ans;
	}

	void start () {
		if (!running) {
			lastStart = std::chrono::steady_clock::now();
			running = true;
		}
	}
	double stop () {
		time += since_pause();
		running = false;
		return time;
	}

	double get () {
		return time + since_pause();
	}
};

hugeint factorial (int max_n) {
	hugeint ans = 1;
	int copy, exp;
	std::bitset <10005> iscomp;
	for (int val = 2; val <= max_n; val++) {
		if (!iscomp[val]) {
			copy = max_n;
			exp = 0;
			while (copy) {
				copy /= val;
				exp += copy;
			}
			ans *= pow((hugeint)val, exp);
			for (int mod = val * val; mod <= max_n; mod += val) {
				iscomp[mod] = true;
			}
		}
	}
	return ans;
}

hugeint multiple (int max_n) {
	hugeint ans = 1;
	int copy, exp;
	std::bitset <10005> iscomp;
	for (int val = 2; val <= max_n; val++) {
		if (!iscomp[val]) {
			copy = max_n / val;
			exp = 0;
			while (copy) {
				copy /= val;
				exp++;
			}
			ans *= pow((hugeint)val, exp);
			for (int mod = val * val; mod <= max_n; mod += val) {
				iscomp[mod] = true;
			}
		}
	}
	return ans;
}

int main () {
	//timer global;
	//hugeint ans;
	//double time;
	//global.start();
	//ans = ((hugeint)2).pow(1000000);
	//time = global.reset();
	//std::cout << ans.size() << '\n' << time << '\n';
	std::cout << sizeof(std::size_t) << '\n';
}