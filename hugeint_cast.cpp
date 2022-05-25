#include "hugeint.h"

// Internals
void hugeint::fromString (const std::string &to_copy) {
	bits.clear();
	neg = false;
	std::string::const_iterator it = to_copy.begin();
	if (*it == '-') {
		it++;
	}
	for (; it != to_copy.end(); it++) {
		*this *= 10;
		*this += *it - '0';
	}
	if (to_copy[0] == '-') {
		negate();
		neg = true;
	}
}

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
	bool is_neg = neg;
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
	if (is_neg) {
		delete calc;
		ans.push_back('-');
	}
	for (std::size_t index = 0; index < ans.size() / 2; index++) {
		std::swap(ans[index], ans[ans.size() - 1 - index]);
	}
	return ans;
}

// Publics
hugeint::hugeint () {
	bits.clear();
	neg = false;
}
hugeint::hugeint (const hugeint &to_copy) {
	bits = to_copy.bits;
	neg = to_copy.neg;
}
hugeint::hugeint (hugeint &&to_copy) noexcept {
	bits = std::move(to_copy.bits);
	neg = to_copy.neg;
}
hugeint::hugeint (const bool &to_copy) : bits() {
	neg = false;
	if (to_copy) {
		bits.push_back(1);
	}
}
hugeint::hugeint (const sint &to_copy) : bits() {
	neg = to_copy < 0;
	if (to_copy) {
		bits.push_back(to_copy);
	}
}
hugeint::hugeint (const int &to_copy) : bits() {
	neg = to_copy < 0;
	if (to_copy) {
		bits.push_back(to_copy);
	}
}
hugeint::hugeint (const llint &to_copy) : bits() {
	neg = to_copy < 0;
	ullint copy = *((uint *)&to_copy);
	if (to_copy) {
		bits.push_back((uint)to_copy);
	}
	if (to_copy >> 32) {
		bits.push_back(to_copy >> 32);
	}
}
hugeint::hugeint (const usint &to_copy) : bits() {
	neg = false;
	if (to_copy) {
		bits.push_back(to_copy);
	}
}
hugeint::hugeint (const uint &to_copy) : bits() {
	neg = false;
	if (to_copy) {
		bits.push_back(to_copy);
	}
}
hugeint::hugeint (const ullint &to_copy) : bits() {
	neg = false;
	if (to_copy) {
		bits.push_back((uint)to_copy);
	}
	if (to_copy >> 32) {
		bits.push_back(to_copy >> 32);
	}
}
hugeint::hugeint (const char *to_copy) : bits() {
	neg = false;
	fromString((std::string)to_copy);
}
hugeint::hugeint (const std::string &to_copy) : bits() {
	neg = false;
	fromString(to_copy);
}

hugeint::operator bool () const {
	return (!bits.empty() || neg);
}
hugeint::operator short int () const {
	return (bits.empty() ? 0 : (sint)(neg ? -bits[0] : bits[0]));
}
hugeint::operator int () const {
	return (bits.empty() ? 0 : (int)bits[0] * (1 - 2 * neg));
}
hugeint::operator long long int () const {
	return (bits.empty() ? 0 : (bits[0] + (bits.size() < 2 ? (neg ? 0xffffffff00000000 : 0) : (llint)bits[1] << 32)) * (1 - 2 * neg));
}
hugeint::operator unsigned short int () const {
	return (bits.empty() ? 0 : (short int)(neg ? -bits[0] : bits[0]));
}
hugeint::operator unsigned int () const {
	return (bits.empty() ? 0 : (neg ? -bits[0] : bits[0]));
}
hugeint::operator unsigned long long int () const {
	return (bits.empty() ? 0 : (neg ? -bits[0] : bits[0]) + (bits.size() < 2 ? 0 : (neg ? -(llint)bits[1] : (llint)bits[1]) << 32));
}
hugeint::operator const char * () const {
	return toString().c_str();
}
hugeint::operator std::string () const {
	return toString();
}

hugeint &hugeint::operator= (hugeint &&to_copy) noexcept {
	bits = std::move(to_copy.bits);
	neg = to_copy.neg;
	return *this;
}
hugeint &hugeint::operator= (const bool &to_copy) {
	bits.clear();
	neg = false;
	if (to_copy) {
		bits.push_back(1);
	}
	return *this;
}
hugeint &hugeint::operator= (const sint &to_copy) {
	bits.clear();
	neg = (to_copy < 0);
	if (to_copy) {
		bits.push_back((int)to_copy);
	}
	return *this;
}
hugeint &hugeint::operator= (const int &to_copy) {
	bits.clear();
	neg = (to_copy < 0);
	if (to_copy) {
		bits.push_back(to_copy);
	}
	return *this;
}
hugeint &hugeint::operator= (const llint &to_copy) {
	bits.clear();
	neg = (to_copy < 0);
	if (to_copy) {
		bits.push_back((uint)to_copy);
	}
	if (to_copy >> 32) {
		bits.push_back(to_copy >> 32);
	}
	return *this;
}
hugeint &hugeint::operator= (const usint &to_copy) {
	bits.clear();
	neg = false;
	if (to_copy) {
		bits.push_back(to_copy);
	}
	return *this;
}
hugeint &hugeint::operator= (const uint &to_copy) {
	bits.clear();
	neg = false;
	if (to_copy) {
		bits.push_back(to_copy);
	}
	return *this;
}
hugeint &hugeint::operator= (const ullint &to_copy) {
	bits.clear();
	neg = false;
	if (to_copy) {
		bits.push_back((uint)to_copy);
	}
	if (to_copy >> 32) {
		bits.push_back(to_copy >> 32);
	}
	return *this;
}
hugeint &hugeint::operator= (const std::string &to_copy) {
	fromString(to_copy);
	return *this;
}

std::ostream &operator<< (std::ostream &out, const hugeint &to_show) {
#ifdef HUGEINT_DECIMAL_OUTPUT
	out << to_show.toString();
#else
	out << to_show.toHex();
#endif
	return out;
}
std::istream &operator>> (std::istream &in, hugeint &to_set) {
	std::string input_string;
	in >> input_string;
	to_set = input_string;
	return in;
}