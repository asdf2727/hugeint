#include "hugeint.h"

// Internals

void hugeint::fromHex (const std::string::const_iterator &start, const std::string::const_iterator &stop, size_t &errPos) {
	for (std::string::const_iterator pos = start; pos != stop; pos++, errPos++) {
		if (*pos == '\'') {
			continue;
		}
		if (('0' > *pos || *pos > '9') && ('a' > *pos || *pos > 'f') && ('A' > *pos || *pos > 'F')) {
			return;
		}
	}
	errPos = -1;

	ullint form = 0;
	std::size_t index = 0;
	bits.clear();
	neg = false;
	std::string::const_iterator rstop = start - 1;
	for (std::string::const_iterator pos = stop - 1; pos != rstop; pos--) {
		if (*pos == '\'') {
			continue;
		}
		if ('0' <= *pos && *pos <= '9') {
			form |= ((llint)(*pos - '0')) << index;
		}
		else if ('a' <= *pos && *pos <= 'f') {
			form |= ((llint)(*pos - 'a' + 10)) << index;
		}
		else {
			form |= ((llint)(*pos - 'A' + 10)) << index;
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
}
void hugeint::fromDec (const std::string::const_iterator &start, const std::string::const_iterator &stop, size_t &errPos) {
	for (std::string::const_iterator pos = start; pos != stop; pos++, errPos++) {
		if (*pos == '\'') {
			continue;
		}
		if ('0' > *pos || *pos > '9') {
			return;
		}
	}
	errPos = -1;

	bits.clear();
	neg = false;
	for (std::string::const_iterator pos = start; pos != stop; pos++) {
		if (*pos == '\'') {
			continue;
		}
		calculateMult(10);
		calculateAdd(*pos - '0');
	}
}
void hugeint::fromOct (const std::string::const_iterator &start, const std::string::const_iterator &stop, size_t &errPos) {
	for (std::string::const_iterator pos = start; pos != stop; pos++, errPos++) {
		if (*pos == '\'') {
			continue;
		}
		if ('0' > *pos || *pos > '7') {
			return;
		}
	}
	errPos = -1;

	ullint form = 0;
	std::size_t index = 0;
	bits.clear();
	neg = false;
	std::string::const_iterator rstop = start - 1;
	for (std::string::const_iterator pos = stop - 1; pos != rstop; pos--) {
		if (*pos == '\'') {
			continue;
		}
		form |= ((llint)(*pos - '0')) << index;
		index += 3;
		if (index >= 32) {
			index -= 32;
			bits.push_back((uint)form);
			form >>= 32;
		}
	}
	bits.push_back((uint)form);
	clearZeros();
}
void hugeint::fromBin (const std::string::const_iterator &start, const std::string::const_iterator &stop, size_t &errPos) {
	for (std::string::const_iterator pos = start; pos != stop; pos++, errPos++) {
		if (*pos == '\'') {
			continue;
		}
		if (*pos != '0' && *pos != '1') {
			return;
		}
	}
	errPos = -1;

	ullint form = 0;
	std::size_t index = 0;
	bits.clear();
	neg = false;
	std::string::const_iterator rstop = start - 1;
	for (std::string::const_iterator pos = stop - 1; pos != rstop; pos--) {
		if (*pos == '\'') {
			continue;
		}
		form |= ((llint)(*pos - '0')) << index;
		index += 1;
		if (index >= 32) {
			index -= 32;
			bits.push_back((uint)form);
			form >>= 32;
		}
	}
	bits.push_back((uint)form);
	clearZeros();
}

// Publics

size_t hugeint::fromString (const std::string::const_iterator &begin, const std::string::const_iterator &end) {
	bool isNeg = false;
	size_t errPos = 0;
	std::string::const_iterator start = begin, stop = end;
	if (begin == end) {
		bits.clear();
		neg = false;
	}
	else {
		while (start != end && (*start == '+' || *start == '-')) {
			neg ^= (*start == '-');
			start++;
			errPos++;
		}
		if (start == end) {
			bits.clear();
			neg = false;
			errPos = -1;
		}
		else if (*start == '0') {
			start++;
			if (*start == 'x' || *start == 'X') {
				errPos += 2;
				fromHex(++start, stop, errPos);
			}
			else if (*start == 'o' || *start == 'O') {
				errPos += 2;
				fromOct(++start, stop, errPos);
			}
			else if (*start == 'b' || *start == 'B') {
				errPos += 2;
				fromBin(++start, stop, errPos);
			}
			else {
				errPos++;
				fromOct(start, stop, errPos);
			}
		}
		else if (*start == 'x' || *start == 'X') {
			errPos++;
			fromHex(++start, stop, errPos);
		}
		else if (*start == 'o' || *start == 'O') {
			errPos++;
			fromOct(++start, stop, errPos);
		}
		else if (*start == 'b' || *start == 'B') {
			errPos++;
			fromBin(++start, stop, errPos);
		}
		else {
			stop--;
			if (*stop == 'x' || *stop == 'X') {
				fromHex(start, stop, errPos);
			}
			else if (*stop == 'o' || *stop == 'O') {
				fromOct(start, stop, errPos);
			}
			else if (*stop == 'b' || *stop == 'B') {
				fromBin(start, stop, errPos);
			}
			else if ('0' <= *stop && *stop <= '9') {
				fromDec(start, ++stop, errPos);
			}
			else {
				for (; start != stop; start++, errPos++);
			}
		}
	}
	if (errPos == -1 && isNeg) {
		negate();
	}
	return errPos;
}

std::string hugeint::toHex () const {
	std::string ans;
	ans.reserve(bits.size() * 8);
	const hugeint *calc = this;
	if (neg) {
		calc = new hugeint(-*this);
		ans = "-0x";
	}
	else {
		ans = "0x";
	}
	bool first0 = true;
	uint to_push;
	for (std::deque <uint>::const_reverse_iterator pos = calc->bits.rbegin(); pos != calc->bits.rend(); pos++) {
		for (int bit = 28; bit >= 0; bit -= 4) {
			to_push = (*pos >> bit) & 15;
			if (first0 && to_push == 0) {
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
	if (neg) {
		delete calc;
		if (ans.size() == 3) {
			ans.push_back('1');
		}
	}
	else {
		if (ans.size() == 2) {
			ans.push_back('0');
		}
	}
	return ans;
}
std::string hugeint::toDec () const {
	bool is_neg = neg;
	const hugeint *calc = this;
	if (neg) {
		calc = new hugeint(-*this);
	}
	std::string ans;
	ans.reserve(bits.size() * 9633 / 1000 + 5); // container size multiplied by 32 * ln2 / ln10 to estimate final size
	char carry;
	bool done;
	ans.push_back(0);
	for (std::deque <uint>::const_reverse_iterator chunk = calc->bits.rbegin(); chunk != calc->bits.rend(); chunk++) {
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
			if (*chunk & pos) {
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
	std::string ans;
	ans.reserve(bits.size() * 32 / 3 + 1);
	const hugeint *calc = this;
	if (neg) {
		calc = new hugeint(-*this);
		ans = "-0";
	}
	else {
		ans = "0";
	}
	bool first0 = true;
	ullint form = 0;
	int count = calc->bits.size() % 3 - 3;
	uint to_push;
	std::deque <uint>::const_reverse_iterator pos = calc->bits.rbegin();
	while (true) {
		if (count < 0) {
			if (pos != calc->bits.rend()) {
				break;
			}
			form = (form << 32) | *pos++;
			count += 32;
		}
		to_push = (form >> count) & 7;
		count -= 3;
		if (first0 && to_push == 0) {
			continue;
		}
		first0 = false;
		ans.push_back(to_push + '0');
	}
	if (neg) {
		delete calc;
		if (ans.size() == 2) {
			ans.push_back('1');
		}
	}
	else {
		if (ans.size() == 1) {
			ans.push_back('0');
		}
	}
	return ans;
}
std::string hugeint::toBin () const {
	std::string ans;
	ans.reserve(bits.size() * 32 / 3 + 1);
	const hugeint *calc = this;
	if (neg) {
		calc = new hugeint(-*this);
		ans = "-";
	}
	bool first0 = true;
	uint to_push;
	for (std::deque <uint>::const_reverse_iterator pos = calc->bits.rbegin(); pos != calc->bits.rend(); pos++) {
		for (int bit = 31; bit >= 0; bit--) {
			to_push = (*pos >> bit) & 1;
			if (first0 && to_push == 0) {
				continue;
			}
			first0 = false;
			ans.push_back(to_push + '0');
		}
	}
	if (neg) {
		delete calc;
	}
	else if (ans.empty()) {
		ans.push_back('0');
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
hugeint::hugeint (bool to_copy) {
	neg = false;
	if (to_copy) {
		bits.push_back(1);
	}
}
hugeint::hugeint (sint to_copy) {
	neg = (to_copy < 0);
	if (to_copy != (neg ? -1 : 0)) {
		bits.push_back(*((usint *)&to_copy));
	}
}
hugeint::hugeint (int to_copy) {
	neg = (to_copy < 0);
	if (to_copy != (neg ? -1 : 0)) {
		bits.push_back(*((uint *)&to_copy));
	}
}
hugeint::hugeint (llint to_copy) {
	neg = (to_copy < 0);
	if (to_copy != (neg ? -1 : 0)) {
		ullint copy = *((ullint *)&to_copy);
		bits.push_back((uint)copy);
		copy >>= 32;
		if (copy != (neg ? 0xffffffff : 0)) {
			bits.push_back((uint)copy);
		}
	}
}
hugeint::hugeint (usint to_copy) {
	neg = false;
	if (to_copy) {
		bits.push_back(to_copy);
	}
}
hugeint::hugeint (uint to_copy) {
	neg = false;
	if (to_copy) {
		bits.push_back(to_copy);
	}
}
hugeint::hugeint (ullint to_copy) {
	neg = false;
	if (to_copy) {
		bits.push_back((uint)to_copy);
	}
	if (to_copy >> 32) {
		bits.push_back(to_copy >> 32);
	}
}
hugeint::hugeint (const std::string &to_copy) {
	neg = false;
	fromString(to_copy.begin(), to_copy.end());
}

hugeint &hugeint::operator= (hugeint &&to_copy) noexcept {
	bits = std::move(to_copy.bits);
	neg = to_copy.neg;
	return *this;
}
hugeint &hugeint::operator= (bool to_copy) {
	bits.clear();
	neg = false;
	if (to_copy) {
		bits.push_back(1);
	}
	return *this;
}
hugeint &hugeint::operator= (sint to_copy) {
	bits.clear();
	neg = (to_copy < 0);
	if (to_copy != (neg ? -1 : 0)) {
		bits.push_back(*((usint *)&to_copy));
	}
	return *this;
}
hugeint &hugeint::operator= (int to_copy) {
	bits.clear();
	neg = (to_copy < 0);
	if (to_copy != (neg ? -1 : 0)) {
		bits.push_back(*((uint *)&to_copy));
	}
	return *this;
}
hugeint &hugeint::operator= (llint to_copy) {
	bits.clear();
	neg = (to_copy < 0);
	if (to_copy != (neg ? -1 : 0)) {
		ullint copy = *((ullint *)&to_copy);
		bits.push_back((uint)copy);
		copy >>= 32;
		if (copy != (neg ? 0xffffffff : 0)) {
			bits.push_back((uint)copy);
		}
	}
	return *this;
}
hugeint &hugeint::operator= (usint to_copy) {
	bits.clear();
	neg = false;
	if (to_copy) {
		bits.push_back(to_copy);
	}
	return *this;
}
hugeint &hugeint::operator= (uint to_copy) {
	bits.clear();
	neg = false;
	if (to_copy) {
		bits.push_back(to_copy);
	}
	return *this;
}
hugeint &hugeint::operator= (ullint to_copy) {
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
	fromString(to_copy.begin(), to_copy.end());
	return *this;
}

hugeint::operator bool () const {
	return (!bits.empty() || neg);
}
hugeint::operator short int () const {
	return (bits.empty() ? (neg ? -1 : 0) : (sint)bits[0]);
}
hugeint::operator int () const {
	return (bits.empty() ? (neg ? -1 : 0) : (int)bits[0]);
}
hugeint::operator long long int () const {
	return (bits.empty() ? (neg ? -1 : 0) : bits[0] + (llint)(bits.size() < 2 ? (neg ? -0x7fffffff00000000 : 0) : (ullint)bits[1] << 32));
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
hugeint::operator float () const {
	if (!neg && bits.empty()) {
		return (float)0;
	}
	if (size() > 128) {
		return neg ? -__builtin_inff() : __builtin_inff();
	}
	bool sign = neg;
	const hugeint *num = this;
	if (neg) {
		hugeint *temp = new hugeint(-*this);
		num = temp;
	}
	uint mant, add;
	add = num->bits.back();
	int shift = __builtin_clz(add) - 8;
	mant = (shift < 0 ? add >> (-shift) : add << shift);
	shift -= 32;
	if (num->bits.size() > 1) {
		add = num->bits[num->bits.size() - 2];
		mant |= (shift < 0 ? add >> (-shift) : add << shift);
	}
	mant &= (1u << 23) - 1;
	if (sign) {
		delete (num);
	}
	int val = (neg ? 0x80000000 : 0x00000000) | ((size() + 126) << 23) | mant;
	return *((float *)&val);
}
hugeint::operator double () const {
	if (!neg && bits.empty()) {
		return (double)0;
	}
	if (size() > 1024) {
		return neg ? -__builtin_inf() : __builtin_inf();
	}
	bool sign = neg;
	const hugeint *num = this;
	if (neg) {
		hugeint *temp = new hugeint(-*this);
		num = temp;
	}
	ullint mant, add;
	add = num->bits.back();
	int shift = __builtin_clzll(add) - 11;
	mant = (shift < 0 ? add >> (-shift) : add << shift);
	shift -= 32;
	if (num->bits.size() > 1) {
		add = num->bits[num->bits.size() - 2];
		mant |= (shift < 0 ? add >> (-shift) : add << shift);
		shift -= 32;
	}
	if (num->bits.size() > 2) {
		add = num->bits[num->bits.size() - 3];
		mant |= (shift < 0 ? add >> (-shift) : add << shift);
	}
	mant &= (1ull << 52) - 1;
	if (sign) {
		delete (num);
	}
	ullint val = (neg ? 0x8000000000000000 : 0x0000000000000000) | ((size() + 1022) << 52) | mant;
	return *((double *)&val);
}
hugeint::operator std::string () const {
	return toDec();
}