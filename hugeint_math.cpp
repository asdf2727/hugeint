#include "hugeint.h"

#include <cstring>
#include <random>
#include <functional>

void hugeint::clearZeros () {
	while (!digits.empty() && digits.back() == (neg ? digit_max : 0)) {
		digits.pop_back();
	}
}
void hugeint::resize (size_t new_size) {
	digits.resize(new_size, neg ? digit_max : 0);
}
void hugeint::invert () {
	neg = !neg;
	for (digit_t &word : digits) {
		word = ~word;
	}
}

size_t hugeint::size () const {
	size_t size = digits.size() * digit_len;
	if (!digits.empty()) {
#ifdef DIGIT_32
		size -= __builtin_clz(neg ? ~digits.back() : digits.back());
#endif
#ifdef DIGIT_64
		size -= __builtin_clzll(neg ? ~digits.back() : digits.back());
#endif
	}
	return size;
}
void hugeint::negate () {
	invert();
	increment();
}

bool hugeint::getBit (size_t pos) const {
	size_t digit_id = pos >> digit_log_len;
	size_t digit_bit = pos & (digit_len - 1);
	return (digit_id < digits.size() ? (digits[digit_id] >> digit_bit) & 1 : neg);
}
void hugeint::flipBit (size_t pos) {
	size_t digit_id = pos >> digit_log_len;
	size_t digit_bit = pos & (digit_len - 1);
	if (digits.size() <= digit_id) {
		resize(digit_id + 1);
	}
	else {
		digits[digit_id] ^= (digit_t)1 << digit_bit;
	}
}
void hugeint::setBit (size_t pos, bool val) {
	if (val ^ getBit(pos)) {
		flipBit(pos);
	}
	clearZeros();
}

bool hugeint::compareSml (const hugeint &to_comp) const {
	if (neg != to_comp.neg) {
		return neg;
	}
	if (digits.size() != to_comp.digits.size()) {
		return neg ^ (digits.size() < to_comp.digits.size());
	}
	for (size_t index = digits.size() - 1; index < digits.size(); index--) {
		if (digits[index] != to_comp.digits[index]) {
			return neg ^ (digits[index] < to_comp.digits[index]);
		}
	}
	return false;
}

hugeint hugeint::calculateAnd (const hugeint &lhs, const hugeint &rhs) {
	hugeint ret;
	const hugeint *sml = &lhs;
	const hugeint *big = &rhs;
	if (sml->digits.size() > big->digits.size()) {
		std::swap(sml, big);
	}
	ret.digits.reserve(sml->neg ? sml->digits.size() : big->digits.size());
	for (size_t i = 0; i < sml->digits.size(); i++) {
		ret.digits.push_back(sml->digits[i] & big->digits[i]);
	}
	if (sml->neg) {
		for (size_t i = sml->digits.size(); i < big->digits.size(); i++) {
			ret.digits.push_back(big->digits[i]);
		}
	}
	ret.neg = sml->neg && big->neg;
	ret.clearZeros();
	return ret;
}
void hugeint::calculateAnd (const hugeint &rhs) {
	if (!rhs.neg && digits.size() > rhs.digits.size()) {
		resize(rhs.digits.size());
	}
	size_t stop = std::min(digits.size(), rhs.digits.size());
	for (size_t index = 0; index < stop; index++) {
		digits[index] &= rhs.digits[index];
	}
	if (neg && digits.size() < rhs.digits.size()) {
		for (size_t index = digits.size(); index < rhs.digits.size(); index++) {
			digits.push_back(rhs.digits[index]);
		}
	}
	neg = neg && rhs.neg;
	clearZeros();
}

hugeint hugeint::calculateOr (const hugeint &lhs, const hugeint &rhs) {
	hugeint ret;
	const hugeint *sml = &lhs;
	const hugeint *big = &rhs;
	if (sml->digits.size() > big->digits.size()) {
		std::swap(sml, big);
	}
	ret.digits.reserve(sml->neg ? big->digits.size() : sml->digits.size());
	for (size_t i = 0; i < sml->digits.size(); i++) {
		ret.digits.push_back(sml->digits[i] | big->digits[i]);
	}
	if (!sml->neg) {
		for (size_t i = sml->digits.size(); i < big->digits.size(); i++) {
			ret.digits.push_back(big->digits[i]);
		}
	}
	ret.neg = sml->neg || big->neg;
	ret.clearZeros();
	return ret;
}
void hugeint::calculateOr (const hugeint &rhs) {
	if (rhs.neg && digits.size() > rhs.digits.size()) {
		resize(rhs.digits.size());
	}
	size_t stop = std::min(digits.size(), rhs.digits.size());
	for (size_t index = 0; index < stop; index++) {
		digits[index] |= rhs.digits[index];
	}
	if (!neg && digits.size() < rhs.digits.size()) {
		for (size_t index = digits.size(); index < rhs.digits.size(); index++) {
			digits.push_back(rhs.digits[index]);
		}
	}
	neg = neg || rhs.neg;
	clearZeros();
}

hugeint hugeint::calculateXor (const hugeint &lhs, const hugeint &rhs) {
	hugeint ret;
	const hugeint *sml = &lhs;
	const hugeint *big = &rhs;
	if (sml->digits.size() > big->digits.size()) {
		std::swap(sml, big);
	}
	ret.digits.reserve(big->digits.size());
	for (size_t i = 0; i < sml->digits.size(); i++) {
		ret.digits.push_back(sml->digits[i] ^ big->digits[i]);
	}
	if (sml->neg) {
		for (size_t i = sml->digits.size(); i < big->digits.size(); i++) {
			ret.digits.push_back(~big->digits[i]);
		}
	}
	else {
		for (size_t i = sml->digits.size(); i < big->digits.size(); i++) {
			ret.digits.push_back(big->digits[i]);
		}
	}
	ret.neg = sml->neg || big->neg;
	ret.clearZeros();
	return ret;
}
void hugeint::calculateXor (const hugeint &rhs) {
	if (digits.size() < rhs.digits.size()) {
		resize(rhs.digits.size());
	}
	size_t stop = std::min(digits.size(), rhs.digits.size());
	for (size_t index = 0; index < stop; index++) {
		digits[index] ^= rhs.digits[index];
	}
	if (rhs.neg && digits.size() > rhs.digits.size()) {
		for (size_t index = rhs.digits.size(); index < digits.size(); index++) {
			digits[index] = ~digits[index];
		}
	}
	neg = neg ^ rhs.neg;
	clearZeros();
}

void hugeint::shiftFwd (size_t val) {
	size_t digit_move = val >> digit_log_len;
	size_t bit_shift = val & (digit_len - 1);
	resize(digits.size() + digit_move + 1);
	if (bit_shift) {
		for (size_t index = digits.size() - 1; index > digit_move; index--) {
			digits[index] = (digits[index - digit_move] << bit_shift) | (digits[index - digit_move - 1] >> (digit_len - bit_shift));
		}
	}
	else {
		for (size_t index = digits.size() - 1; index > digit_move; index--) {
			digits[index] = digits[index - digit_move];
		}
	}
	digits[digit_move] = digits[0] << bit_shift;
	if (digit_move) {
		std::memset(&digits.front(), 0x00, sizeof(digit_t) * digit_move);
	}
	clearZeros();
}
void hugeint::shiftBack (size_t val) {
	size_t digit_move = val >> digit_log_len;
	size_t bit_shift = val & (digit_len - 1);
	resize(std::max((size_t)1, digits.size() - digit_move));
	if (bit_shift) {
		for (size_t index = 0; index < digits.size() - 1; index++) {
			digits[index] = (digits[index + digit_move] >> bit_shift) | (digits[index + digit_move + 1] << (digit_len - bit_shift));
		}
	}
	else {
		for (size_t index = 0; index < digits.size() - 1; index++) {
			digits[index] = digits[index + digit_move];
		}
	}
	digits.back() = digits[digits.size() - 1 + digit_move] >> bit_shift;
	clearZeros();
}

void hugeint::increment () {
	if (neg && digits.empty()) {
		neg = false;
		return;
	}
	for (digit_t &word : digits) {
		if (word == digit_max) {
			word = 0;
		}
		else {
			word++;
			clearZeros();
			return;
		}
	}
	digits.push_back(1);
}
void hugeint::decrement () {
	if (!neg && digits.empty()) {
		neg = true;
		return;
	}
	for (digit_t &word : digits) {
		if (word == 0) {
			word = digit_max;
		}
		else {
			word--;
			clearZeros();
			return;
		}
	}
	digits.push_back(digit_max - 1);
}

hugeint hugeint::calculateAdd (const hugeint &lhs, const hugeint &rhs, const bool diff) {
	const hugeint *sml = &lhs, *big = &rhs;
	if (lhs.digits.size() > rhs.digits.size()) {
		std::swap(sml, big);
	}
	hugeint ret;
	ret.digits.reserve(big->digits.size());

	digit_t carry = diff ? 1 : 0;
	digit_t rhs_xor = diff ? digit_max : 0;
	for (size_t index = 0; index < sml->digits.size(); index++) {
		ret.digits.push_back(lhs.digits[index] + (rhs.digits[index] ^ rhs_xor) + carry);
		carry = digit_t(ret.digits.back() < lhs.digits[index]) | (carry & digit_t(ret.digits.back() == lhs.digits[index]));
	}
	digit_t sml_xor = sml->neg ^ (diff && lhs.digits.size() > rhs.digits.size()) ? digit_max : 0;
	digit_t big_xor = diff && lhs.digits.size() <= rhs.digits.size() ? digit_max : 0;
	for (size_t index = sml->digits.size(); index < big->digits.size(); index++) {
		if (sml_xor + carry == 0) {
			carry = sml_xor = 0;
			break;
		}
		ret.digits.push_back((big->digits[index] ^ big_xor) + sml_xor + carry);
		carry = digit_t(ret.digits.back() < sml_xor + carry);
	}
	while (ret.digits.size() < big->digits.size()) {
		ret.digits.push_back(big->digits[ret.digits.size()] ^ big_xor);
	}

	bool sml_sign = (bool)sml_xor;
	bool big_sign = (bool)big_xor ^ big->neg;
	if (carry) {
		if (!sml_sign && !big_sign) {
			ret.digits.push_back(1);
		}
		ret.neg = sml_sign && big_sign;
	}
	else {
		if (sml_sign && big_sign) {
			ret.digits.push_back(digit_max - 1);
		}
		ret.neg = sml_sign || big_sign;
	}
	ret.clearZeros();

	return ret;
}
void hugeint::calculateAdd (const hugeint &rhs, const bool diff) {
	if (digits.size() < rhs.digits.size()) {
		resize(rhs.digits.size());
	}

	digit_t carry = diff ? 1 : 0;
	digit_t rhs_xor = diff ? digit_max : 0;
	for (size_t index = 0; index < rhs.digits.size(); index++) {
		digits[index] += (rhs.digits[index] ^ rhs_xor) + carry;
		carry = digit_t(digits[index] < (rhs.digits[index] ^ rhs_xor)) | (carry & digit_t(digits[index] == (rhs.digits[index] ^ rhs_xor)));
	}
	rhs_xor ^= rhs.neg ? digit_max : 0;
	for (size_t index = rhs.digits.size(); index < digits.size(); index++) {
		if (rhs_xor + carry == 0) {
			carry = rhs_xor = 0;
			break;
		}
		digits[index] += rhs_xor + carry;
		carry = digit_t(digits[index] < rhs_xor + carry);
	}

	bool rhs_sign = (bool)rhs_xor;
	if (carry) {
		if (!(bool)rhs_sign && !neg) {
			digits.push_back(1);
		}
		neg &= (bool)rhs_sign;
	}
	else {
		if ((bool)rhs_sign && neg) {
			digits.push_back(digit_max - 1);
		}
		neg |= (bool)rhs_sign;
	}
	clearZeros();
}

hugeint hugeint::simpleMult (std::vector <digit_t>::iterator begin1, std::vector <digit_t>::iterator end1,
                             std::vector <digit_t>::iterator begin2, std::vector <digit_t>::iterator end2) {
	size_t size1 = end1 - begin1;
	size_t size2 = end2 - begin2;
	hugeint ans;
	double_t mult, over = 0, now = 0;
	for (size_t i = 0; i < size1 + size2 - 1; i++) {
		digit_t start = std::max(i + 1, size1) - size1;
		std::vector <digit_t>::iterator it1 = begin1 + (i - start);
		std::vector <digit_t>::iterator it2 = begin2 + start;
		std::vector <digit_t>::iterator stop = begin2 + std::min(i + 1, size2);
		while (it2 != stop) {
			mult = (double_t)*it1 * *it2;
			over += mult >> digit_len;
			now += mult & digit_max;
			it1--;
			it2++;
		}
		ans.digits.push_back(now & digit_max);
		over += now >> digit_len;
		now = over & digit_max;
		over >>= digit_len;
	}
	if (now) {
		ans.digits.push_back(now);
	}
	return ans;
}
hugeint hugeint::addKaratsuba (std::vector <digit_t>::iterator begin1, std::vector <digit_t>::iterator end1,
                               std::vector <digit_t>::iterator begin2, std::vector <digit_t>::iterator end2) {
	hugeint ret;
	if (end1 - begin1 < end2 - begin2) {
		std::swap(begin1, begin2);
		std::swap(end1, end2);
	}
	ret.digits.reserve(end1 - begin1);
	digit_t carry = 0;
	while (begin2 != end2) {
		ret.digits.push_back(*begin1 + *begin2 + carry);
		carry = digit_t(ret.digits.back() < *begin1) | (carry & digit_t(ret.digits.back() == *begin1));
		begin1++;
		begin2++;
	}
	while (begin1 != end1) {
		ret.digits.push_back(*begin1 + carry);
		carry = digit_t(*begin1 == digit_max);
		begin1++;
	}
	if (carry) {
		ret.digits.push_back(carry);
	}
	return ret;
}
hugeint hugeint::karatsuba (std::vector <digit_t>::iterator begin1, std::vector <digit_t>::iterator end1,
                            std::vector <digit_t>::iterator begin2, std::vector <digit_t>::iterator end2, size_t block_size) {
	// Explanation for the karatsuba fast multiplication algorithm: https://en.wikipedia.org/wiki/Karatsuba_algorithm
	block_size >>= 1;
	std::vector <digit_t>::iterator cut1 = std::min(begin1 + block_size, end1);
	std::vector <digit_t>::iterator cut2 = std::min(begin2 + block_size, end2);
	std::vector <digit_t>::iterator last1 = cut1;
	std::vector <digit_t>::iterator last2 = cut2;
	while (begin1 != last1 && *(last1 - 1) == 0) {
		last1--;
	}
	while (begin2 != last2 && *(last2 - 1) == 0) {
		last2--;
	}
	hugeint high, mid, low;
	high = multAlgorithm(cut1, end1, cut2, end2, block_size);
	low = multAlgorithm(begin1, last1, begin2, last2, block_size);
	hugeint add1 = addKaratsuba(begin1, last1, cut1, end1);
	hugeint add2 = addKaratsuba(begin2, last2, cut2, end2);
	if (add1.digits.size() > block_size) {
		mid += add2 << (block_size << digit_log_len);
		add1.digits.pop_back();
	}
	if (add2.digits.size() > block_size) {
		mid += add1 << (block_size << digit_log_len);
		add2.digits.pop_back();
	}
	mid += multAlgorithm(add1.digits.begin(), add1.digits.end(),
	                     add2.digits.begin(), add2.digits.end(), block_size);

	mid -= high;
	mid -= low;
	high <<= block_size << digit_log_len;
	high += mid;
	high <<= block_size << digit_log_len;
	high += low;
	return high;
}
hugeint hugeint::multAlgorithm (std::vector <digit_t>::iterator begin1, std::vector <digit_t>::iterator end1,
                                std::vector <digit_t>::iterator begin2, std::vector <digit_t>::iterator end2, size_t block_size) {
	size_t size1 = end1 - begin1;
	size_t size2 = end2 - begin2;
	if (size1 <= 0 || size2 <= 0) {
		return 0;
	}
	if (size1 == 1 && size2 == 1) {
		return (hugeint)((double_t)*begin1 * *begin2);
	}
	if (size1 * size1 < size2 || size2 * size2 < size1) {
		return simpleMult(begin1, end1, begin2, end2);
	}
	while (block_size >= std::max(size1, size2) << 1) {
		block_size >>= 1;
	}
	return karatsuba(begin1, end1, begin2, end2, block_size);
}
hugeint hugeint::calculateMult (const hugeint &lhs, const hugeint &rhs) {
	hugeint lhs_copy = lhs;
	hugeint rhs_copy = rhs;
	lhs_copy.abs();
	rhs_copy.abs();
	hugeint ret = multAlgorithm(lhs_copy.digits.begin(), lhs_copy.digits.end(),
	                            rhs_copy.digits.begin(), rhs_copy.digits.end(), (digit_t)1 << (digit_len - 1));
	if (lhs.neg ^ rhs.neg) {
		ret.negate();
	}
	return ret;
}

hugeint hugeint::simpleDiv (hugeint lhs, hugeint rhs, bool rem) {
	size_t init_shift = digit_len - (rhs.size() & (digit_len - 1));
	lhs <<= init_shift;
	rhs <<= init_shift;

	size_t len_dif = lhs.digits.size() - rhs.digits.size();
	hugeint temp1, temp2, ans;
	ans.digits.resize(len_dif);

	digit_t quot;
	temp1 = rhs << (len_dif * digit_len);
	if (lhs > temp1) {
		lhs -= temp1;
		ans.digits.push_back(1);
	}
	for (size_t i = len_dif - 1; i < lhs.digits.size(); i--) {
		temp1 >>= int(digit_len); // Don't remove: https://www.reddit.com/r/cpp/comments/dl0c9y/passing_a_static_const_member_by_reftoconst/

		quot = std::min(
				(((double_t)lhs.digits[i + rhs.digits.size()] << digit_len) |
				 lhs.digits[i + rhs.digits.size() - 1]) / rhs.digits.back(),
				(double_t)digit_max);
		temp2 = temp1 * quot;

		lhs -= temp2;
		while (lhs < 0) {
			lhs += temp1;
			quot--;
		}
		ans.digits[i] = quot;
	}
	if (rem) {
		lhs >>= init_shift;
		return lhs;
	}
	else {
		ans.clearZeros();
		return ans;
	}
}
hugeint hugeint::calculateDiv (const hugeint &lhs, const hugeint &rhs) {
	if (!(bool)rhs) {
		throw (std::invalid_argument("Division by 0"));
	}
	return simpleDiv(lhs, rhs);
}
hugeint hugeint::calculateMod (const hugeint &lhs, const hugeint &rhs) {
	if (!(bool)rhs) {
		throw (std::invalid_argument("Division by 0"));
	}
	return simpleDiv(lhs, rhs, true);
}

void hugeint::setRamdon (size_t size, bool rand_sign) {
	std::random_device seed;
	std::default_random_engine generator(seed());
	std::uniform_int_distribution <digit_t> distribution(0, digit_max);
	auto random_digit = std::bind(distribution, generator);
	neg = rand_sign && (random_digit() & 1);
	digits.clear();
	for (size_t index = 0; index < (size >> digit_log_len); index++) {
		digits.push_back(random_digit());
	}
	if (size & (digit_len - 1)) {
		digits.push_back(random_digit() & (((digit_t)1 << (size & (digit_len - 1))) - 1));
	}
	clearZeros();
	if (neg) {
		negate();
	}
}

void hugeint::calculateGcd (hugeint other) {
	while (other) {
		*this = (*this) % other;
		if (*this) {
			other = other % (*this);
		}
		else {
			*this = other;
			break;
		}
	}
}

void hugeint::calculatePow (exp_t exponent) {
	if (exponent == 0) {
		neg = false;
		digits.clear();
		digits.push_back(1);
		return;
	}
	while (!(exponent & 1) && exponent) {
		exponent >>= 1;
		*this = calculateMult(*this, *this);
	}
	hugeint power = *this;
	exponent >>= 1;
	while (exponent) {
		power *= power;
		if (exponent & 1) {
			*this = calculateMult(*this, power);
		}
		exponent >>= 1;
	}
}
void hugeint::calculatePow (exp_t exponent, const hugeint &to_mod) {
	while (!(exponent & 1) && exponent) {
		exponent >>= 1;
		*this = calculateMult(*this, *this);
	}
	hugeint power = *this;
	exponent >>= 1;
	while (exponent) {
		power *= power;
		power %= to_mod;
		if (exponent & 1) {
			*this = calculateMult(*this, power);
			*this = calculateMod(*this, to_mod);
		}
		exponent >>= 1;
	}
}

void hugeint::calculateNthRoot (root_t degree) {
	hugeint ans;
	for (size_t pos = size() / degree; pos <= size() / degree; pos--) {
		ans.flipBit(pos);
		if (pow(ans, degree) > *this) {
			ans.flipBit(pos);
		}
	}
	*this = ans;
}
