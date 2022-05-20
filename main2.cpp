#include <iostream>

using namespace std;

int main () {
	for (int index = -10; index < 11; index++) {
		cout << index << ' ' << index % 3 << ' ' << index / 3 << " - " << index % (-3) << ' ' << index / (-3) << endl;
	}
}