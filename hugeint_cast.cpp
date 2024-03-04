#include "hugeint.h"

using namespace huge;

size_t hugeint::fromHex (const std::string::const_iterator &begin, const std::string::const_iterator &end) {
	for (std::string::const_iterator pos = begin; pos != end; pos++) {
		if (*pos == '\'' || *pos == ' ') {
			continue;
		}
		if (!('0' < *pos && *pos < '9') && !('a' < *pos && *pos < 'f') && !('A' < *pos && *pos < 'F')) {
			return pos - begin;
		}
	}

	double_t form = 0;
	std::size_t index = 0;
	digits.clear();
	neg = false;
	std::string::const_iterator rstop = begin - 1;
	for (std::string::const_iterator pos = end - 1; pos != rstop; pos--) {
		if (*pos == '\'' || *pos == ' ') {
			continue;
		}
		if ('0' <= *pos && *pos <= '9') {
			form |= ((double_t)(*pos - '0')) << index;
		}
		else if ('a' <= *pos && *pos <= 'f') {
			form |= ((double_t)(*pos - 'a' + 10)) << index;
		}
		else {
			form |= ((double_t)(*pos - 'A' + 10)) << index;
		}
		index += 4;
		if (index >= digit_len) {
			index -= digit_len;
			digits.push_back(form);
			form >>= digit_len;
		}
	}
	digits.push_back(form);
	clearZeros();
	return -1;
}
size_t hugeint::fromDec (const std::string::const_iterator &begin, const std::string::const_iterator &end) {
	for (std::string::const_iterator pos = begin; pos != end; pos++) {
		if (*pos == '\'' || *pos == ' ') {
			continue;
		}
		if ('0' > *pos || *pos > '9') {
			return pos - begin;
		}
	}

	digits.clear();
	neg = false;
	for (std::string::const_iterator pos = begin; pos != end; pos++) {
		if (*pos == '\'' || *pos == ' ') {
			continue;
		}
		*this = calculateMult(*this, (hugeint)10);
		calculateAdd((hugeint)(*pos - '0'));
	}
	return -1;
}
size_t hugeint::fromOct (const std::string::const_iterator &begin, const std::string::const_iterator &end) {
	for (std::string::const_iterator pos = begin; pos != end; pos++) {
		if (*pos == '\'' || *pos == ' ') {
			continue;
		}
		if ('0' > *pos || *pos > '7') {
			return end - pos;
		}
	}

	double_t form = 0;
	std::size_t index = 0;
	digits.clear();
	neg = false;
	std::string::const_iterator rstop = begin - 1;
	for (std::string::const_iterator pos = end - 1; pos != rstop; pos--) {
		if (*pos == '\'' || *pos == ' ') {
			continue;
		}
		form |= ((double_t)(*pos - '0')) << index;
		index += 3;
		if (index >= digit_len) {
			index -= digit_len;
			digits.push_back(form);
			form >>= digit_len;
		}
	}
	digits.push_back(form);
	clearZeros();
	return -1;
}
size_t hugeint::fromBin (const std::string::const_iterator &begin, const std::string::const_iterator &end) {
	for (std::string::const_iterator pos = begin; pos != end; pos++) {
		if (*pos == '\'' || *pos == ' ') {
			continue;
		}
		if (*pos != '0' && *pos != '1') {
			return pos - begin;
		}
	}

	double_t form = 0;
	std::size_t index = 0;
	digits.clear();
	neg = false;
	std::string::const_iterator rstop = begin - 1;
	for (std::string::const_iterator pos = end - 1; pos != rstop; pos--) {
		if (*pos == '\'' || *pos == ' ') {
			continue;
		}
		form |= ((double_t)(*pos - '0')) << index;
		index += 1;
		if (index >= digit_len) {
			index -= digit_len;
			digits.push_back(form);
			form >>= digit_len;
		}
	}
	digits.push_back(form);
	clearZeros();
	return -1;
}

size_t hugeint::fromString (std::string::const_iterator begin, std::string::const_iterator end) {
	bool is_neg = false;
	size_t err_pos = -1;
	while (begin != end && (*begin == '+' || *begin == '-')) {
		is_neg ^= (*begin == '-');
		begin++;
		err_pos++;
	}
	if (begin == end) {
		digits.clear();
		neg = false;
		return -1;
	}
	if (*begin == '0') {
		begin++;
		if (*begin == 'x' || *begin == 'X') {
			err_pos = fromHex(++begin, end) + 2;
		}
		else if (*begin == 'd' || *begin == 'D') {
			err_pos = fromDec(++begin, end + 2);
		}
		else if (*begin == 'o' || *begin == 'O') {
			err_pos = fromOct(++begin, end) + 2;
		}
		else if (*begin == 'b' || *begin == 'B') {
			err_pos = fromBin(++begin, end) + 2;
		}
		else {
			err_pos = fromOct(begin, end) + 1;
		}
	}
	else if (*begin == 'x' || *begin == 'X') {
		err_pos = fromHex(++begin, end) + 1;
	}
	else if (*begin == 'd' || *begin == 'D') {
		err_pos = fromDec(++begin, end) + 1;
	}
	else if (*begin == 'o' || *begin == 'O') {
		err_pos = fromOct(++begin, end) + 1;
	}
	else if (*begin == 'b' || *begin == 'B') {
		err_pos = fromBin(++begin, end) + 1;
	}
	else {
		end--;
		if (*end == 'x' || *end == 'X') {
			err_pos = fromHex(begin, end);
		}
		else if (*end == 'd' || *end == 'D') {
			err_pos = fromDec(begin, end);
		}
		else if (*end == 'o' || *end == 'O') {
			err_pos = fromOct(begin, end);
		}
		else if (*end == 'b' || *end == 'B') {
			err_pos = fromBin(begin, end);
		}
		else if ('0' <= *end && *end <= '9') {
			err_pos = fromDec(begin, ++end);
		}
		else {
			err_pos = end - begin;
		}
	}
	if (err_pos == -1 && is_neg) {
		negate();
	}
	return err_pos;
}

std::string hugeint::toHex () const {
	std::string ans = "0x";
	ans.reserve(digits.size() * digit_len >> 2);
	const hugeint *calc = this;
	if (neg) {
		calc = new hugeint(-*this);
		ans = "-0x";
	}
	bool first0 = true;
	digit_t to_push;
	for (std::vector <digit_t>::const_reverse_iterator pos = calc->digits.rbegin(); pos != calc->digits.rend(); pos++) {
		for (int bit = digit_len - 4; bit >= 0; bit -= 4) {
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
	std::string ans;
	ans.reserve(digits.size() * digit_len * 301 / 1000); // digit count multiplied by log10(2) to estimate final size
	ans.push_back(0);
	for (std::vector <digit_t>::const_reverse_iterator chunk = calc->digits.rbegin(); chunk != calc->digits.rend(); chunk++) {
		for (digit_t pos = (digit_t)1 << (digit_len - 1); pos > 0; pos >>= 1) {
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
	ans.reserve(digits.size() * digit_len / 3);
	const hugeint *calc = this;
	if (neg) {
		calc = new hugeint(-*this);
		ans = "-o";
	}
	bool first0 = true;
	double_t form = 0;
	int count = calc->digits.size() % 3 - 3;
	digit_t to_push;
	std::vector <digit_t>::const_reverse_iterator chunk = calc->digits.rbegin();
	while (true) {
		if (count < 0) {
			if (chunk == calc->digits.rend()) {
				break;
			}
			form = (form << digit_len) | *chunk++;
			count += digit_len;
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
	ans.reserve(digits.size() * digit_len);
	const hugeint *calc = this;
	if (neg) {
		calc = new hugeint(-*this);
		ans = "-";
	}
	bool first0 = true;
	digit_t to_push;
	for (std::vector <digit_t>::const_reverse_iterator pos = calc->digits.rbegin(); pos != calc->digits.rend(); pos++) {
		for (int bit = digit_len - 1; bit >= 0; bit--) {
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

hugeint::hugeint () {
	digits.clear();
	neg = false;
}
hugeint::hugeint (hugeint &&to_copy) noexcept {
	digits = std::move(to_copy.digits);
	neg = to_copy.neg;
}
hugeint::hugeint (bool to_copy) {
	neg = false;
	if (to_copy) {
		digits.push_back(1);
	}
}
hugeint::hugeint (int16_t to_copy) {
	neg = false;
	if (to_copy < 0) {
		digits.push_back(-to_copy);
		negate();
	}
	else {
		digits.push_back(to_copy);
	}
	clearZeros();
}
hugeint::hugeint (uint16_t to_copy) {
	neg = false;
	digits.push_back(to_copy);
	clearZeros();
}
hugeint::hugeint (int32_t to_copy) {
	neg = false;
	if (to_copy < 0) {
		digits.push_back(-to_copy);
		negate();
	}
	else {
		digits.push_back(to_copy);
	}
	clearZeros();
}
hugeint::hugeint (uint32_t to_copy) {
	neg = false;
	digits.push_back(to_copy);
	clearZeros();
}
#ifdef DIGIT_32
hugeint::hugeint (int64_t to_copy) {
	neg = false;
	if (to_copy < 0) {
		digits.push_back(-to_copy);
		digits.push_back((-to_copy) >> digit_len);
		negate();
	}
	else {
		digits.push_back(to_copy);
		digits.push_back(to_copy >> digit_len);
	}
	clearZeros();
}
hugeint::hugeint (uint64_t to_copy) {
	neg = false;
	digits.push_back(to_copy);
	digits.push_back(to_copy >> digit_len);
	clearZeros();
}
#endif
#ifdef DIGIT_64
hugeint::hugeint (int64_t to_copy) {
	neg = false;
	if (to_copy < 0) {
		digits.push_back(-to_copy);
		negate();
	}
	else {
		digits.push_back(to_copy);
	}
	clearZeros();
}
hugeint::hugeint (uint64_t to_copy) {
	neg = false;
	digits.push_back(to_copy);
	clearZeros();
}
hugeint::hugeint (__int128 to_copy) {
	neg = false;
	if (to_copy < 0) {
		digits.push_back(-to_copy);
		digits.push_back((-to_copy) >> digit_len);
		negate();
	}
	else {
		digits.push_back(to_copy);
		digits.push_back(to_copy >> digit_len);
	}
	clearZeros();
}
hugeint::hugeint (unsigned __int128 to_copy) {
	neg = false;
	digits.push_back(to_copy);
	digits.push_back(to_copy >> digit_len);
	clearZeros();
}
#endif
hugeint::hugeint (float to_copy) {
	uint32_t val = *(uint32_t *)(&to_copy);
	uint32_t shift = ((val & 0x7f800000) >> 23) - 150;
	*this = (val & 0x007fffff) | 0x00800000;
	*this <<= shift;
	clearZeros();
	if (val & 0x80000000) {
		negate();
	}
}
hugeint::hugeint (double to_copy) {
	uint64_t val = *(uint64_t *)(&to_copy);
	uint64_t shift = ((val & 0x7ff0000000000000) >> 52) - 1075;
	*this = (val & 0x000fffffffffffff) | 0x0010000000000000;
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
	digits = std::move(to_copy.digits);
	neg = to_copy.neg;
	return *this;
}
hugeint &hugeint::operator= (bool to_copy) {
	digits.clear();
	neg = false;
	if (to_copy) {
		digits.push_back(1);
	}
	return *this;
}
hugeint &hugeint::operator= (int16_t to_copy) {
	digits.clear();
	if (to_copy < 0) {
		digits.push_back(-to_copy);
		negate();
	}
	else {
		digits.push_back(to_copy);
	}
	clearZeros();
	return *this;
}
hugeint &hugeint::operator= (uint16_t to_copy) {
	digits.clear();
	neg = false;
	digits.push_back(to_copy);
	clearZeros();
	return *this;
}
hugeint &hugeint::operator= (int32_t to_copy) {
	digits.clear();
	if (to_copy < 0) {
		digits.push_back(-to_copy);
		negate();
	}
	else {
		digits.push_back(to_copy);
	}
	clearZeros();
	return *this;
}
hugeint &hugeint::operator= (uint32_t to_copy) {
	digits.clear();
	neg = false;
	digits.push_back(to_copy);
	clearZeros();
	return *this;
}
#ifdef DIGIT_32
hugeint &hugeint::operator= (int64_t to_copy) {
	digits.clear();
	if (to_copy < 0) {
		digits.push_back(-to_copy);
		digits.push_back((-to_copy) >> digit_len);
		negate();
	}
	else {
		digits.push_back(to_copy);
		digits.push_back(to_copy >> digit_len);
	}
	clearZeros();
	return *this;
}
hugeint &hugeint::operator= (uint64_t to_copy) {
	digits.clear();
	neg = false;
	digits.push_back(to_copy);
	digits.push_back(to_copy >> digit_len);
	clearZeros();
	return *this;
}
#endif
#ifdef DIGIT_64
hugeint &hugeint::operator= (int64_t to_copy) {
	digits.clear();
	if (to_copy < 0) {
		digits.push_back(-to_copy);
		negate();
	}
	else {
		digits.push_back(to_copy);
	}
	clearZeros();
	return *this;
}
hugeint &hugeint::operator= (uint64_t to_copy) {
	digits.clear();
	neg = false;
	digits.push_back(to_copy);
	clearZeros();
	return *this;
}
hugeint &hugeint::operator= (__int128 to_copy) {
	digits.clear();
	if (to_copy < 0) {
		digits.push_back(-to_copy);
		digits.push_back((-to_copy) >> digit_len);
		negate();
	}
	else {
		digits.push_back(to_copy);
		digits.push_back(to_copy >> digit_len);
	}
	clearZeros();
	return *this;
}
hugeint &hugeint::operator= (unsigned __int128 to_copy) {
	digits.clear();
	neg = false;
	digits.push_back(to_copy);
	digits.push_back(to_copy >> digit_len);
	clearZeros();
	return *this;
}
#endif
hugeint &hugeint::operator= (float to_copy) {
	digits.clear();
	uint32_t val = *(uint32_t *)(&to_copy);
	uint32_t shift = ((val & 0x7f800000) >> 23) - 150;
	*this = (val & 0x007fffff) | 0x00800000;
	*this <<= shift;
	clearZeros();
	if (val & 0x80000000) {
		negate();
	}
	return *this;
}
hugeint &hugeint::operator= (double to_copy) {
	digits.clear();
	uint64_t val = *(uint64_t *)(&to_copy);
	uint64_t shift = ((val & 0x7ff0000000000000) >> 52) - 1075;
	*this = (val & 0x000fffffffffffff) | 0x0010000000000000;
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
	return (!digits.empty() || neg);
}
hugeint::operator int16_t () const {
	uint16_t num = digits.empty() ? (neg ? 0xffff : 0) : digits[0];
	return *(int16_t *)(&num);
}
hugeint::operator uint16_t () const {
	return digits.empty() ? (neg ? 0xffff : 0) : digits[0];
}
hugeint::operator int32_t () const {
	uint32_t num = digits.empty() ? (neg ? 0xffffffff : 0) : digits[0];
	return *(int32_t *)(&num);
}
hugeint::operator uint32_t () const {
	return digits.empty() ? (neg ? 0xffffffff : 0) : digits[0];
}
#ifdef DIGIT_32
hugeint::operator int64_t () const {
	uint64_t num1 = digits.empty() ? (neg ? 0xffffffff : 0) : digits[0];
	uint64_t num2 = digits.size() < 2 ? (neg ? 0xffffffff : 0) : digits[1];
	uint64_t num = (num2 << digit_len) | num1;
	return *(int64_t *)(&num);
}
hugeint::operator uint64_t () const {
	uint64_t num1 = digits.empty() ? (neg ? 0xffffffff : 0) : digits[0];
	uint64_t num2 = digits.size() < 2 ? (neg ? 0xffffffff : 0) : digits[1];
	return (num2 << digit_len) | num1;
}
#endif
#ifdef DIGIT_64
hugeint::operator int64_t () const {
	uint64_t num = digits.empty() ? (neg ? 0xffffffffffffffff : 0) : digits[0];
	return *(int64_t *)(&num);
}
hugeint::operator uint64_t () const {
	return digits.empty() ? (neg ? 0xffffffffffffffff : 0) : digits[0];
}
hugeint::operator __int128 () const {
	unsigned __int128 num1 = digits.empty() ? (neg ? 0xffffffffffffffff : 0) : digits[0];
	unsigned __int128 num2 = digits.size() < 2 ? (neg ? 0xffffffffffffffff : 0) : digits[1];
	unsigned __int128 num = (num2 << digit_len) | num1;
	return *(__int128 *)(&num);
}
hugeint::operator unsigned __int128 () const {
	unsigned __int128 num1 = digits.empty() ? (neg ? 0xffffffffffffffff : 0) : digits[0];
	unsigned __int128 num2 = digits.size() < 2 ? (neg ? 0xffffffffffffffff : 0) : digits[1];
	return (num2 << digit_len) | num1;
}
#endif
hugeint::operator float () const {
	if (!neg && digits.empty()) {
		return (float)0;
	}
	if (size() > 128) {
		return neg ? -__builtin_inff() : __builtin_inff();
	}

	uint32_t mant = (uint32_t)(*this >> (size() - 24));
	if (neg) {
		mant = ~mant;
	}
	uint32_t val = (neg ? 0x80000000 : 0x00000000) | ((size() + 127) << 23) | (mant & 0x007fffff);
	return *((float *)&val);
}
hugeint::operator double () const {
	if (!neg && digits.empty()) {
		return (float)0;
	}
	if (size() > 128) {
		return neg ? -__builtin_inf() : __builtin_inf();
	}

	uint64_t mant = (uint64_t)(*this >> (size() - 53));
	if (neg) {
		mant = ~mant;
	}
	uint64_t val = (neg ? 0x8000000000000000 : 0x0000000000000000) | ((size() + 1023) << 52) | (mant & 0x000fffffffffffff);
	return *((float *)&val);
}
hugeint::operator std::string () const {
	return toDec();
}