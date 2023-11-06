#include "hugeint.h"

#include <cmath>
#include <cstring>
#include <random>
#include <functional>

using namespace huge;

void hugeint::clearZeros () {
	while (!digits.empty() && digits.back() == (neg ? digit_max : 0)) {
		digits.pop_back();
	}
}
void hugeint::invert () {
	neg = !neg;
	for (digit_t &word : digits) {
		word = ~word;
	}
}
void hugeint::resize (size_t new_size) {
	digits.resize(new_size, neg ? digit_max : 0);
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

void hugeint::calculateAnd (const hugeint &to_and) {
	if (!to_and.neg && digits.size() > to_and.digits.size()) {
		digits.resize(to_and.digits.size());
	}
	for (size_t index = 0; index < std::min(digits.size(), to_and.digits.size()); index++) {
		digits[index] &= to_and.digits[index];
	}
	if (neg && digits.size() < to_and.digits.size()) {
		for (size_t index = digits.size(); index < to_and.digits.size(); index++) {
			digits.push_back(to_and.digits[index]);
		}
	}
	neg = neg && to_and.neg;
	clearZeros();
}
void hugeint::calculateOr (const hugeint &to_or) {
	if (to_or.neg && digits.size() > to_or.digits.size()) {
		digits.resize(to_or.digits.size());
	}
	for (size_t index = 0; index < std::min(digits.size(), to_or.digits.size()); index++) {
		digits[index] |= to_or.digits[index];
	}
	if (!neg && digits.size() < to_or.digits.size()) {
		for (size_t index = digits.size(); index < to_or.digits.size(); index++) {
			digits.push_back(to_or.digits[index]);
		}
	}
	neg = neg || to_or.neg;
	clearZeros();
}
void hugeint::calculateXor (const hugeint &to_xor) {
	if (digits.size() < to_xor.digits.size()) {
		digits.resize(to_xor.digits.size(), neg ? digit_max : 0);
	}
	for (size_t index = 0; index < std::min(digits.size(), to_xor.digits.size()); index++) {
		digits[index] ^= to_xor.digits[index];
	}
	if (to_xor.neg && digits.size() > to_xor.digits.size()) {
		for (size_t index = to_xor.digits.size(); index < digits.size(); index++) {
			digits[index] = ~digits[index];
		}
	}
	neg = neg ^ to_xor.neg;
	clearZeros();
}

void hugeint::shiftFwd (size_t val) {
	size_t digit_move = val >> digit_log_len;
	size_t bit_shift = val & (digit_len - 1);
	digits.resize(digits.size() + digit_move + 1);
	for (size_t index = digits.size() - 1; index > digit_move; index--) {
		digits[index] = (digits[index - digit_move] << bit_shift) | (digits[index - digit_move - 1] >> (digit_len - bit_shift));
	}
	digits[digit_move] = digits[0] << bit_shift;
	if (digit_move) {
		std::memset(&digits.front(), 0x00, sizeof(digit_t) * digit_move);
	}
	if (digits.back() == 0) {
		digits.pop_back();
	}
}
void hugeint::shiftBack (size_t val) {
	size_t digit_move = val >> digit_log_len;
	size_t bit_shift = val & (digit_len - 1);
	digits.resize(digits.size() - digit_move);
	for (size_t index = 0; index < digits.size() - 1; index++) {
		digits[index] = (digits[index + digit_move] >> bit_shift) | (digits[index + digit_move + 1] << (digit_len - bit_shift));
	}
	digits.back() = digits[digits.size() - 1 + digit_move] >> bit_shift;
	if (digits.back() == 0) {
		digits.pop_back();
	}
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

void hugeint::calculateAdd (const hugeint &to_add, bool negate) {
	// Add by default, subtract if negate is true
	if (digits.size() < to_add.digits.size()) {
		digits.resize(to_add.digits.size(), neg ? digit_max : 0);
	}
	double_t carry;
	if (negate) {
		carry = 1;
		for (size_t index = 0; index < to_add.digits.size(); index++) {
			carry += digits[index];
			carry += ~to_add.digits[index];
			digits[index] = (digit_t)carry;
			carry >>= digit_len;
		}
	}
	else {
		carry = 0;
		for (size_t index = 0; index < to_add.digits.size(); index++) {
			carry += digits[index];
			carry += to_add.digits[index];
			digits[index] = (digit_t)carry;
			carry >>= digit_len;
		}
	}
	if (to_add.neg ^ negate ^ carry) { // 2 cases condensed into 1: negative number with no carry and positive number with carry.
		digit_t add = carry ? 1 : digit_max;
		digit_t target = carry ? 0 : digit_max;
		digit_t push = carry ? 1 : digit_max - 1;
		for (size_t index = to_add.digits.size(); index < digits.size(); index++) {
			digits[index] += add;
			if (digits[index] != target) {
				return;
			}
		}
		if (neg ^ carry) {
			digits.push_back(push);
		}
		else {
			neg = !neg;
			clearZeros();
		}
	}
}

hugeint hugeint::simpleMult (hugeint &num1, hugeint &num2) {
	hugeint ans;
	double_t mult, over = 0, now = 0;
	for (size_t i = 0; i < num1.digits.size() + num2.digits.size() - 1; i++) {
		size_t start = i > num1.digits.size() ? i - num1.digits.size() : 0;
		size_t stop = std::min(i + 1, num2.digits.size());
		for (size_t j = start; j < stop; j++) {
			mult = (double_t)num1.digits[i - j] * num2.digits[j];
			over += mult >> digit_len;
			now += mult & digit_max;
		}
		ans.digits.push_back(now & digit_max);
		now >>= digit_len;
		now += over;
		over = now >> digit_len;
		now &= digit_max;
	}
	if (now) {
		ans.digits.push_back(now);
	}
	return ans;
}
hugeint hugeint::karatsuba (hugeint &num1, hugeint &num2, size_t tot_size) {
	// Explination for the karatsuba fast multiplication agorithm: https://en.wikipedia.org/wiki/Karatsuba_algorithm
	hugeint high, mid, low;
	hugeint &num1_l = num1;
	hugeint num1_h;
	hugeint &num2_l = num2;
	hugeint num2_h;
	tot_size >>= 1;
	if (num1.digits.size() > tot_size) {
		num1_h.resize(tot_size);
		std::copy(num1.digits.begin() + (size_t)tot_size, num1.digits.end(), num1_h.digits.begin());
		num1_l.resize(tot_size);
		num1_l.clearZeros();
	}
	if (num2.digits.size() > tot_size) {
		num2_h.resize(tot_size);
		std::copy(num2.digits.begin() + (size_t)tot_size, num2.digits.end(), num2_h.digits.begin());
		num2_l.resize(tot_size);
		num2_l.clearZeros();
	}
	hugeint add1 = num1_l + num1_h;
	hugeint add2 = num2_l + num2_h;
	high = doMultAlgorithm(num1_h, num2_h, tot_size);
	low = doMultAlgorithm(num1_l, num2_l, tot_size);

	mid = -high - low;
	if (add1.digits.size() > tot_size) {
		mid += add2 << (tot_size << 5);
		add1.digits.pop_back();
	}
	if (add2.digits.size() > tot_size) {
		mid += add1 << (tot_size << 5);
		add2.digits.pop_back();
	}
	mid += doMultAlgorithm(add1, add2, tot_size);
	high <<= tot_size << 5;
	high += mid;
	high <<= tot_size << 5;
	high += low;
	return high;
}
hugeint hugeint::doMultAlgorithm (hugeint &num1, hugeint &num2, size_t tot_size) {
	if (num1.digits.empty() || num2.digits.empty()) {
		return 0;
	}
	while (tot_size && tot_size >> 1 >= std::max(num1.digits.size(), num2.digits.size())) {
		tot_size >>= 1;
	}
	if (tot_size <= 1) {
		return (double_t)num1.digits[0] * num2.digits[0];
	}
	if (num1.digits.size() * num1.digits.size() < num2.digits.size()) {
		return simpleMult(num2, num1);
	}
	if (num2.digits.size() * num2.digits.size() < num1.digits.size()) {
		return simpleMult(num1, num2);
	}
	return karatsuba(num1, num2, tot_size);
}
void hugeint::calculateMult (const hugeint &to_mult) {
	bool is_neg = neg ^ to_mult.neg;
	hugeint &num1 = *this;
	num1.abs();
	hugeint num2 = huge::abs(to_mult);
	size_t max_size = 1ull << (64 - __builtin_clzll(num1.digits.size() | num2.digits.size() | 1));
	num1 = is_neg ? -doMultAlgorithm(num1, num2, max_size) : doMultAlgorithm(num1, num2, max_size);
}

digit_t hugeint::divBinSearch (hugeint &rest, const hugeint &to_div) {
	digit_t calc = 0;
	hugeint form, to_add = (to_div << (digit_len - 1));
	for (digit_t index = (digit_t)1 << (digit_len - 1); index > 0; index >>= 1) {
		if (form + to_add <= rest) {
			form += to_add;
			calc += index;
		}
		to_add >>= 1;
	}
	rest -= form;
	return calc;
}
void hugeint::calculateDiv (const hugeint &to_div) {
	if (!(bool)to_div) {
		throw (std::invalid_argument("Division by 0"));
	}
	// TODO later
}
void hugeint::calculateMod (const hugeint &to_div) {
	if (!(bool)to_div) {
		throw (std::invalid_argument("Division by 0"));
	}
	// TODO later
}

void hugeint::setRamdon (size_t size, bool rand_sign) {
	std::random_device seed;
	std::default_random_engine generator(seed());
	std::uniform_int_distribution <digit_t> distribution(0, digit_max);
	auto random_digit = std::bind(distribution, generator);
	neg = rand_sign && (random_digit() & 1);
	digits.clear();
	for (int index = 0; index < size >> digit_log_len; index++) {
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
	hugeint rest;
	while (other) {
		rest = (*this) % other;
		*this = other;
		other = rest;
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
		calculateMult(*this);
	}
	hugeint power = *this;
	exponent >>= 1;
	while (exponent) {
		power *= power;
		if (exponent & 1) {
			calculateMult(power);
		}
		exponent >>= 1;
	}
}
void hugeint::calculatePow (exp_t exponent, const hugeint &to_mod) {
	while (!(exponent & 1) && exponent) {
		exponent >>= 1;
		calculateMult(*this);
	}
	hugeint power = *this;
	exponent >>= 1;
	while (exponent) {
		power *= power;
		power %= to_mod;
		if (exponent & 1) {
			calculateMult(power);
			calculateMod(to_mod);
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

// Size also works as log2 (gives the number of binary digits, for example 64.size() = 7)
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