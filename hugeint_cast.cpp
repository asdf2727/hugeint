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

// Publics
hugeint::hugeint () {
	bits.clear();
	neg = false;
}
hugeint::hugeint (const hugeint &to_copy) {
	this->operator=(to_copy);
}
hugeint::hugeint (hugeint &&to_copy) noexcept {
	this->operator=(std::move(to_copy));
}
hugeint::hugeint (const bool &to_copy) {
	this->operator=(to_copy);
}
hugeint::hugeint (const sint &to_copy) {
	this->operator=(to_copy);
}
hugeint::hugeint (const int &to_copy) {
	this->operator=(to_copy);
}
hugeint::hugeint (const llint &to_copy) {
	this->operator=(to_copy);
}
hugeint::hugeint (const usint &to_copy) {
	this->operator=(to_copy);
}
hugeint::hugeint (const uint &to_copy) {
	this->operator=(to_copy);
}
hugeint::hugeint (const ullint &to_copy) {
	this->operator=(to_copy);
}
hugeint::hugeint (const char *to_copy) {
	fromString((std::string)to_copy);
}
hugeint::hugeint (const std::string &to_copy) {
	fromString(to_copy);
}

hugeint::operator bool () const {
	return (!bits.empty() || neg);
}
hugeint::operator short int () const {
	return (bits.empty() ? 0 : (sint)bits[0] * (1 - 2 * neg));
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