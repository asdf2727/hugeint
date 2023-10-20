#include "hugeint.h"

// Internals

void hugeint::fromHex (const std::string::const_iterator &begin, const std::string::const_iterator &end, size_t &errPos) {
	for (std::string::const_iterator pos = begin; pos != end; pos++, errPos++) {
		if (*pos == '\'' || *pos == ' ') {
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
	std::string::const_iterator rstop = begin - 1;
	for (std::string::const_iterator pos = end - 1; pos != rstop; pos--) {
		if (*pos == '\'' || *pos == ' ') {
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
void hugeint::fromDec (const std::string::const_iterator &begin, const std::string::const_iterator &end, size_t &errPos) {
	for (std::string::const_iterator pos = begin; pos != end; pos++, errPos++) {
		if (*pos == '\'' || *pos == ' ') {
			continue;
		}
		if ('0' > *pos || *pos > '9') {
			return;
		}
	}
	errPos = -1;

	bits.clear();
	neg = false;
	for (std::string::const_iterator pos = begin; pos != end; pos++) {
		if (*pos == '\'' || *pos == ' ') {
			continue;
		}
		calculateMult(10);
		calculateAdd(*pos - '0');
	}
}
void hugeint::fromOct (const std::string::const_iterator &begin, const std::string::const_iterator &end, size_t &errPos) {
	for (std::string::const_iterator pos = begin; pos != end; pos++, errPos++) {
		if (*pos == '\'' || *pos == ' ') {
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
	std::string::const_iterator rstop = begin - 1;
	for (std::string::const_iterator pos = end - 1; pos != rstop; pos--) {
		if (*pos == '\'' || *pos == ' ') {
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
void hugeint::fromBin (const std::string::const_iterator &begin, const std::string::const_iterator &end, size_t &errPos) {
	for (std::string::const_iterator pos = begin; pos != end; pos++, errPos++) {
		if (*pos == '\'' || *pos == ' ') {
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
	std::string::const_iterator rstop = begin - 1;
	for (std::string::const_iterator pos = end - 1; pos != rstop; pos--) {
		if (*pos == '\'' || *pos == ' ') {
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

size_t hugeint::fromString (std::string::const_iterator begin, std::string::const_iterator end) {
	bool isNeg = false;
	size_t errPos = -1;
	while (begin != end && (*begin == '+' || *begin == '-')) {
		isNeg ^= (*begin == '-');
		begin++;
		errPos++;
	}
	if (begin == end) {
		bits.clear();
		neg = false;
		return -1;
	}
	else if (*begin == '0') {
		begin++;
		if (*begin == 'x' || *begin == 'X') {
			errPos += 2;
			fromHex(++begin, end, errPos);
		}
		else if (*begin == 'd' || *begin == 'D') {
			errPos += 2;
			fromDec(++begin, end, errPos);
		}
		else if (*begin == 'o' || *begin == 'O') {
			errPos += 2;
			fromOct(++begin, end, errPos);
		}
		else if (*begin == 'b' || *begin == 'B') {
			errPos += 2;
			fromBin(++begin, end, errPos);
		}
		else {
			errPos++;
			fromOct(begin, end, errPos);
		}
	}
	else if (*begin == 'x' || *begin == 'X') {
		errPos++;
		fromHex(++begin, end, errPos);
	}
	else if (*begin == 'd' || *begin == 'D') {
		errPos++;
		fromDec(++begin, end, errPos);
	}
	else if (*begin == 'o' || *begin == 'O') {
		errPos++;
		fromOct(++begin, end, errPos);
	}
	else if (*begin == 'b' || *begin == 'B') {
		errPos++;
		fromBin(++begin, end, errPos);
	}
	else {
		end--;
		if (*end == 'x' || *end == 'X') {
			fromHex(begin, end, errPos);
		}
		else if (*end == 'd' || *end == 'D') {
			fromDec(begin, end, errPos);
		}
		else if (*end == 'o' || *end == 'O') {
			fromOct(begin, end, errPos);
		}
		else if (*end == 'b' || *end == 'B') {
			fromBin(begin, end, errPos);
		}
		else if ('0' <= *end && *end <= '9') {
			fromDec(begin, ++end, errPos);
		}
		else {
			errPos = end - begin;
		}
	}
	if (errPos == -1 && isNeg) {
		negate();
	}
	return errPos;
}

std::string hugeint::toHex () const {
	std::string ans = "0x";
	ans.reserve(bits.size() * 8);
	const hugeint *calc = this;
	if (neg) {
		calc = new hugeint(-*this);
		ans = "-0x";
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
				ans.push_back(to_push + 'A' - 10);
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
	std::string ans = "";
	ans.reserve(bits.size() * 9633 / 1000 + 5); // container size multiplied by 32 * ln2 / ln10 to estimate final size
	ans.push_back(0);
	for (std::deque <uint>::const_reverse_iterator chunk = calc->bits.rbegin(); chunk != calc->bits.rend(); chunk++) {
		for (uint pos = 0x80000000; pos > 0; pos >>= 1) {
			//Multiply by 2
			for (char &val : ans) {
				val <<= 1;
			}
			// Add 1
			if (*chunk & pos) {
				ans[0]++;
			}
			// Carry
			for (std::string::iterator it = ans.begin(); it + 1 != ans.end(); it++) {
				if (*it > 9) {
					*it -= 10;
					*(it + 1) += 1;
				}
			}
			if (ans.back() > 9) {
				ans.back() -= 10;
				ans.push_back(1);
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
	std::string ans = "o";
	ans.reserve(bits.size() * 32 / 3 + 1);
	const hugeint *calc = this;
	if (neg) {
		calc = new hugeint(-*this);
		ans = "-o";
	}
	bool first0 = true;
	ullint form = 0;
	int count = calc->bits.size() % 3 - 3;
	uint to_push;
	std::deque <uint>::const_reverse_iterator chunk = calc->bits.rbegin();
	while (true) {
		if (count < 0) {
			if (chunk == calc->bits.rend()) {
				break;
			}
			form = (form << 32) | *chunk++;
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
	std::string ans = "";
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
		if (ans.size() == 1) {
			ans.push_back('1');
		}
	}
	else {
		if (ans.empty()) {
			ans.push_back('0');
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
	bool copy_neg = false;
	if (to_copy < 0) {
		to_copy = -to_copy;
		copy_neg = true;
	}
	if (to_copy) {
		bits.push_back(to_copy);
	}
	if (copy_neg) {
		negate();
	}
}
hugeint::hugeint (int to_copy) {
	bool copy_neg = false;
	if (to_copy < 0) {
		to_copy = -to_copy;
		copy_neg = true;
	}
	if (to_copy) {
		bits.push_back(to_copy);
	}
	if (copy_neg) {
		negate();
	}
}
hugeint::hugeint (llint to_copy) {
	bool copy_neg = false;
	if (to_copy < 0) {
		to_copy = -to_copy;
		copy_neg = true;
	}
	if (to_copy) {
		bits.push_back((uint)(to_copy & 0x00000000ffffffff));
		to_copy <<= 32;
		if (to_copy) {
			bits.push_back(to_copy);
		}
	}
	if (copy_neg) {
		negate();
	}
}
hugeint::hugeint (usint to_copy) {
	if (to_copy) {
		bits.push_back(to_copy);
	}
}
hugeint::hugeint (uint to_copy) {
	if (to_copy) {
		bits.push_back(to_copy);
	}
}
hugeint::hugeint (ullint to_copy) {
	if (to_copy) {
		bits.push_back((uint)to_copy);
		to_copy >>= 32;
		if (to_copy) {
			bits.push_back(to_copy);
		}
	}
}
hugeint::hugeint (float to_copy) {
	uint val = *(uint *)(&to_copy);
	uint shift = ((val & 0x7f800000) >> 23) - 150;
	bits.push_back((val & 0x007fffff) | 0x00800000);
	*this <<= shift;
	clearZeros();
	if (val & 0x80000000) {
		negate();
	}
}
hugeint::hugeint (double to_copy) {
	ullint val = *(ullint *)(&to_copy);
	uint shift = ((val & 0x7ff0000000000000) >> 52) - 1075;
	bits.push_back(val & 0x00000000ffffffff);
	bits.push_back(((val & 0x000fffff00000000) >> 32) | 0x00100000);
	*this <<= shift;
	clearZeros();
	if (val & 0x8000000000000000) {
		negate();
	}
}
hugeint::hugeint (const std::string &to_copy) {
	fromString(to_copy.begin(), to_copy.end());
}
hugeint::hugeint (const char *to_copy) {
	std::string str = to_copy;
	fromString(str.begin(), str.end());
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
	bool copy_neg = false;
	if (to_copy < 0) {
		to_copy = -to_copy;
		copy_neg = true;
	}
	bits.clear();
	neg = false;
	if (to_copy) {
		bits.push_back(to_copy);
	}
	if (copy_neg) {
		negate();
	}
	return *this;
}
hugeint &hugeint::operator= (int to_copy) {
	bool copy_neg = false;
	if (to_copy < 0) {
		to_copy = -to_copy;
		copy_neg = true;
	}
	bits.clear();
	neg = false;
	if (to_copy) {
		bits.push_back(to_copy);
	}
	if (copy_neg) {
		negate();
	}
	return *this;
}
hugeint &hugeint::operator= (llint to_copy) {
	bool copy_neg = false;
	if (to_copy < 0) {
		to_copy = -to_copy;
		copy_neg = true;
	}
	bits.clear();
	neg = false;
	if (to_copy) {
		bits.push_back((uint)(to_copy & 0x00000000ffffffff));
		to_copy <<= 32;
		if (to_copy) {
			bits.push_back(to_copy);
		}
	}
	if (copy_neg) {
		negate();
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
		to_copy >>= 32;
		if (to_copy) {
			bits.push_back(to_copy);
		}
	}
	return *this;
}
hugeint &hugeint::operator= (float to_copy) {
	uint val = *(uint *)(&to_copy);
	int shift = ((val & 0x7f800000) >> 23) - 150;
	bits.clear();
	neg = false;
	bits.push_back((val & 0x007fffff) | 0x00800000);
	*this <<= shift;
	clearZeros();
	if (val & 0x80000000) {
		negate();
	}
	return *this;
}
hugeint &hugeint::operator= (double to_copy) {
	ullint val = *(ullint *)(&to_copy);
	int shift = ((val & 0x7ff0000000000000) >> 52) - 1075;
	bits.clear();
	neg = false;
	bits.push_back(val & 0x00000000ffffffff);
	bits.push_back(((val & 0x000fffff00000000) >> 32) | 0x00100000);
	*this <<= shift;
	clearZeros();
	if (val & 0x8000000000000000) {
		negate();
	}
	return *this;
}
hugeint &hugeint::operator= (const std::string &to_copy) {
	fromString(to_copy.begin(), to_copy.end());
	return *this;
}
hugeint &hugeint::operator= (const char *to_copy) {
	std::string str = to_copy;
	fromString(str.begin(), str.end());
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
hugeint::operator long int () const {
	return (lint)this->operator long long int();
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
hugeint::operator unsigned long int () const {
	return (ulint)this->operator unsigned long long int();
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
	if (neg) {
		delete (num);
	}
	uint val = (neg ? 0x80000000 : 0x00000000) | ((size() + 126) << 23) | mant;
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
hugeint::operator const char * () const {
	return toDec().c_str();
}