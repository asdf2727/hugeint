#include "hugeint.h"

std::string hugeint::toHex () const {
	std::string ans = (neg ? "(neg)0x" : "0x");
	uint toadd;
	bool done;
	for (std::size_t index = bits.size() - 1; index < bits.size(); index--) {
		for (int shift = 28; shift >= 0; shift -= 4) {
			toadd = (bits[index] & (15u << shift)) >> shift;
			if (toadd != (neg ? 15 : 0) || ans.back() != 'x') {
				if (toadd > 9) {
					ans.push_back(toadd + 'a' - 10); // 'a' - 10
				}
				else {
					ans.push_back(toadd + '0');
				}
			}
		}
	}
	return ans;
}
std::string hugeint::toString () const {
	const hugeint *calc = nullptr;
	if (neg) {
		auto *negated = new hugeint(*this);
		negated->negate();
		calc = negated;
	}
	else {
		calc = this;
	}
	std::string ans;
	char carry;
	bool done;
	ans.push_back(0);
	for (std::size_t index = calc->bits.size() - 1; index < calc->bits.size(); index--) {
		for (uint pos = 0x80000000; pos > 0; pos >>= 1) {
			//Multiply by 2
			carry = 0;
			for (char &val : ans) {
				val = (val << 1) + carry;
				if (val > 9) {
					val -= 10;
					carry = 1;
				}
				else {
					carry = 0;
				}
			}
			if (carry) {
				ans.push_back(1);
			}
			// Add 1
			if (calc->bits[index] & pos) {
				done = false;
				for (char &chr : ans) {
					if (chr == 9) {
						chr = 0;
					}
					else {
						chr++;
						done = true;
						break;
					}
				}
				if (!done) {
					ans.push_back(1);
				}
			}
		}
	}
	for (char &chr : ans) {
		chr += '0';
	}
	if (neg) {
		delete calc;
		ans.push_back('-');
	}
	for (std::size_t index = 0; index < ans.size() / 2; index++) {
		std::swap(ans[index], ans[ans.size() - 1 - index]);
	}
	return ans;
}

std::size_t hugeint::size() const {
	return bits.size();
}

hugeint &hugeint::pow(ullint exponent) {
	hugeint result = 1;
	for(ullint bit = 1; bit <= exponent; bit <<= 1) {
		if (exponent & bit) {
			result *= *this;
		}
		*this *= *this;
	}
	return *this = result;
}
hugeint pow(hugeint base, unsigned long long int exponent) {
	hugeint result = 1;
	for(unsigned long long int bit = 1; bit <= exponent; bit <<= 1) {
		if (exponent & bit) {
			result *= base;
		}
		base *= base;
	}
	return result;
}