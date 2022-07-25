#include "hugeint.h"

// Internals
bool hugeint::fromHex (const std::string::const_iterator &start, const std::string::const_iterator &stop) {
	for (std::string::const_iterator pos = start; pos != stop; pos++) {
		if (('0' > *pos || *pos > '9') && ('a' > *pos || *pos > 'z') && ('A' > *pos || *pos > 'Z')) {
			return false;
		}
	}
	ullint form = 0;
	std::size_t index = 0;
	bits.clear();
	neg = false;
	std::string::const_iterator rstop = start - 1;
	for (std::string::const_iterator pos = stop - 1; pos != rstop; pos--) {
		if ('0' <= *pos && *pos <= '9') {
			form |= (*pos - '0') << index;
		}
		else if ('a' <= *pos && *pos <= 'f') {
			form |= (*pos - 'a' + 10) << index;
		}
		else {
			form |= (*pos - 'A' + 10) << index;
		}
		index += 4;
		if (index >= 32) {
			index -= 32;
			bits.push_back((uint)form);
			form >>= 32;
		}
	}
	bits.push_back((uint)form);
	clearZeros();
	return true;
}
bool hugeint::fromDec (const std::string::const_iterator &start, const std::string::const_iterator &stop) {
	for (std::string::const_iterator pos = start; pos != stop; pos++) {
		if ('0' > *pos || *pos > '9') {
			return false;
		}
	}
	bits.clear();
	neg = false;
	for (std::string::const_iterator pos = start; pos != stop; pos++) {
		calculateMult(10);
		calculateAdd(*pos - '0');
	}
	return true;
}
bool hugeint::fromOct (const std::string::const_iterator &start, const std::string::const_iterator &stop) {
	for (std::string::const_iterator pos = start; pos != stop; pos++) {
		if ('0' > *pos || *pos > '7') {
			return false;
		}
	}
	ullint form = 0;
	std::size_t index = 0;
	bits.clear();
	neg = false;
	std::string::const_iterator rstop = start - 1;
	for (std::string::const_iterator pos = stop - 1; pos != rstop; pos--) {
		form |= (*pos - '0') << index;
		index += 3;
		if (index >= 32) {
			index -= 32;
			bits.push_back((uint)form);
			form >>= 32;
		}
	}
	bits.push_back((uint)form);
	clearZeros();
	return true;
}
bool hugeint::fromBin (const std::string::const_iterator &start, const std::string::const_iterator &stop) {
	for (std::string::const_iterator pos = start; pos != stop; pos++) {
		if (*pos != '0' && *pos != '1') {
			return false;
		}
	}
	ullint form = 0;
	std::size_t index = 0;
	bits.clear();
	neg = false;
	std::string::const_iterator rstop = start - 1;
	for (std::string::const_iterator pos = stop - 1; pos != rstop; pos--) {
		form |= (*pos - '0') << index;
		index += 1;
		if (index >= 32) {
			index -= 32;
			bits.push_back((uint)form);
			form >>= 32;
		}
	}
	bits.push_back((uint)form);
	clearZeros();
	return true;
}

// Publics
bool hugeint::fromString (const std::string::const_iterator &begin, const std::string::const_iterator &end) {
	bool isNeg = false, ans = true;
	std::string::const_iterator start = begin, stop = end;
	if (begin == end) {
		bits.clear();
		neg = false;
	}
	else {
		if (*start == '-') {
			start++;
			isNeg = true;
		}
		if (*start == '0') {
			start++;
			if (*start == 'x' || *start == 'X') {
				ans = fromHex(++start, stop);
			}
			else if (*start == 'o' || *start == 'O') {
				ans = fromOct(++start, stop);
			}
			else if (*start == 'b' || *start == 'B') {
				ans = fromBin(++start, stop);
			}
			else {
				ans = fromOct(start, stop);
			}
		}
		else {
			stop--;
			if ('0' <= *stop && *stop <= '9') {
				stop++;
				ans = fromDec(start, stop);
			}
			else {
				if (*stop == 'x' || *stop == 'X') {
					ans = fromHex(start, stop);
				}
				else if (*stop == 'o' || *stop == 'O') {
					ans = fromOct(start, stop);
				}
				else if (*stop == 'b' || *stop == 'B') {
					ans = fromBin(start, stop);
				}
				else {
					return false;
				}
			}
		}
	}
	if (ans && isNeg) {
		negate();
	}
	return ans;
}

std::string hugeint::toHex () const {
	bool first0 = true;
	std::string ans = (neg ? "-0x" : "0x");
	uint to_push;
	for (std::size_t index = bits.size() - 1; index < bits.size(); index--) {
		for (int bit = 28; bit >= 0; bit -= 4) {
			to_push = (bits[index] >> bit) & 15;
			if (first0 && to_push == (neg ? 15 : 0)) {
				continue;
			}
			first0 = false;
			if (to_push > 9) {
				ans.push_back(to_push + 'a' - 10);
			}
			else {
				ans.push_back(to_push + '0');
			}
		}
	}
	return ans;
}
std::string hugeint::toDec () const {
	const hugeint *calc = nullptr;
	bool is_neg = neg;
	if (neg) {
		hugeint *negated = new hugeint(*this);
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
std::string hugeint::toOct () const {
	bool first0 = true;
	std::string ans = (neg ? "-0" : "0");
	ullint form = 0;
	int count = -(((bits.size() - 1) % 3) + 1);
	uint to_push;
	std::size_t index = bits.size() - 1;
	while (true) {
		if (count < 0) {
			if (index > bits.size()) {
				break;
			}
			form = (form << 32) | bits[index--];
			count += 32;
		}
		to_push = (form >> count) & 7;
		count -= 3;
		if (first0 && to_push == (neg ? 7 : 0)) {
			continue;
		}
		first0 = false;
		ans.push_back(to_push + '0');
	}
	return ans;
}
std::string hugeint::toBin () const {
	bool first0 = true;
	std::string ans = (neg ? "-" : "");
	uint to_push;
	for (std::size_t index = bits.size() - 1; index < bits.size(); index--) {
		for (int bit = 31; bit >= 0; bit--) {
			to_push = (bits[index] >> bit) & 1;
			if (first0 && to_push == (neg ? 1 : 0)) {
				continue;
			}
			first0 = false;
			ans.push_back(to_push + '0');
		}
	}
	ans.push_back('b');
	return ans;
}

std::ostream &operator<< (std::ostream &out, const hugeint &to_show) {
	out << (std::string)to_show;
	return out;
}
std::istream &operator>> (std::istream &in, hugeint &to_set) {
	std::string input_string;
	in >> input_string;
	to_set.fromString(input_string.begin(), input_string.end());
	return in;
}

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
	std::string *strForm = new std::string(to_copy);
	fromString(strForm->begin(), strForm->end());
	delete (strForm);
}
hugeint::hugeint (const std::string &to_copy) : bits() {
	neg = false;
	fromString(to_copy.begin(), to_copy.end());
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
#ifdef HUGEINT_DECIMAL_OUTPUT
	return toDec().c_str();
#else
	return toHex().c_str();
#endif
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
hugeint &hugeint::operator= (const char *to_copy) {
	std::string *strForm = new std::string(to_copy);
	fromString(strForm->begin(), strForm->end());
	delete (strForm);
	return *this;
}
hugeint &hugeint::operator= (const std::string &to_copy) {
	fromString(to_copy.begin(), to_copy.end());
	return *this;
}