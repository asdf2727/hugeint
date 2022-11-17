#include "hugeint.h"

// Intermideats

// Size also works as log2 (gives the number of binary digits after the most significant bit of 1, for example 64.size() = 6, etc.)
unsigned long long int hugeint::size () const {
	ullint size = (bits.size() << 5) - 1;
	if (!bits.empty()) {
		for (uint index = 0x80000000; (index & (neg ? ~bits.back() : bits.back())) == 0; index >>= 1, size--);
	}
	return size;
}
void hugeint::clearZeros () {
	while (!bits.empty() && bits.back() == (neg ? 0xffffffff : 0)) {
		bits.pop_back();
	}
}
void hugeint::resize (std::size_t new_size) {
	std::size_t old_size = bits.size();
	bits.resize(new_size, (neg ? 0xffffffff : 0));
}
void hugeint::invert () {
	for (uint &word : bits) {
		word = ~word;
	}
}
void hugeint::negate () {
	neg = !neg;
	invert();
	increment();
}

bool hugeint::getBit (size_t pos) const {
	return (pos >> 5 < bits.size() ? (bits[pos >> 5] >> (pos & 31)) & 1 : neg);
}
void hugeint::flipBit (size_t pos) {
	while (bits.size() <= pos >> 5) {
		bits.push_back(neg ? 0xffffffff : 0);
	}
	bits[pos >> 5] ^= 1 << (pos & 31);
}
void hugeint::setBit (size_t pos, bool val) {
	if (val ^ getBit(pos)) {
		flipBit(pos);
	}
}

hugeint hugeint::simpleMult (const hugeint &num1, const hugeint &num2) {
	hugeint ans = 0;
	ullint chunk, over;
	for (size_t index = num1.bits.size() - 1; index < num1.bits.size(); index--) {
		chunk = num1.bits[index];
		if (ans) {
			ans.bits.push_front(0);
		}
		ans.resize(std::max(ans.bits.size(), num2.bits.size() + 1));
		over = 0;
		for (size_t index2 = 0; index2 < num2.bits.size(); index2++) {
			over += chunk * num2.bits[index2] + ans.bits[index2];
			ans.bits[index2] = (uint)over;
			over >>= 32;
		}
		for (size_t index2 = num2.bits.size(); index2 < ans.bits.size() && over != 0; index2++) {
			over += ans.bits[index2];
			ans.bits[index2] = (uint)over;
			over >>= 32;
		}
		while (over) {
			ans.bits.push_back((uint)over);
			over >>= 32;
		}
		ans.clearZeros();
	}
	return ans;
}
// Explination for the karatsuba fast multiplication agorithm: https://en.wikipedia.org/wiki/Karatsuba_algorithm
hugeint hugeint::karatsuba (const hugeint &num1, const hugeint &num2, std::size_t tot_size) {
	hugeint high, mid, low;
	hugeint num1L = num1, num1H;
	hugeint num2L = num2, num2H;
	tot_size >>= 1;
	if (num1.bits.size() > tot_size) {
		for (std::size_t index = tot_size; index < num1.bits.size(); index++) {
			num1H.bits.push_back(num1.bits[index]);
		}
		num1L.resize(tot_size);
		num1L.clearZeros();
	}
	if (num2.bits.size() > tot_size) {
		for (std::size_t index = tot_size; index < num2.bits.size(); index++) {
			num2H.bits.push_back(num2.bits[index]);
		}
		num2L.resize(tot_size);
		num2L.clearZeros();
	}
	high = doMultAlgorithm(num1H, num2H, tot_size);
	low = doMultAlgorithm(num1L, num2L, tot_size);

	mid = -high - low;
	num1L += num1H;
	num2L += num2H;
	if (num1L.bits.size() > tot_size) {
		mid += num2L << (tot_size << 5);
		num1L.bits.pop_back();
	}
	if (num2L.bits.size() > tot_size) {
		mid += num1L << (tot_size << 5);
		num2L.bits.pop_back();
	}
	mid += doMultAlgorithm(num1L, num2L, tot_size);
	high <<= tot_size << 5;
	high += mid;
	high <<= tot_size << 5;
	high += low;
	return high;
}
inline hugeint hugeint::doMultAlgorithm (const hugeint &num1, const hugeint &num2, std::size_t tot_size) {
	if (num1.bits.empty() || num2.bits.empty()) {
		return 0;
	}

	while (tot_size && tot_size >> 1 >= std::max(num1.bits.size(), num2.bits.size())) {
		tot_size >>= 1;
	}
	if (tot_size <= 1) {
		return (ullint)num1.bits[0] * num2.bits[0];
	}

	size_t mins = std::min(num1.bits.size(), num2.bits.size()), maxs = std::max(num1.bits.size(), num2.bits.size());
	return mins * mins < maxs ? simpleMult(num1, num2) : karatsuba(num1, num2, tot_size);
}

unsigned int hugeint::divBinSearch (hugeint &rest, const hugeint &to_div) {
	uint calc = 0;
	hugeint form, to_add = (to_div << 31);
	for (uint index = 0x80000000; index > 0; index >>= 1) {
		if (form + to_add <= rest) {
			form += to_add;
			calc += index;
		}
		to_add >>= 1;
	}
	rest -= form;
	return calc;
}

// Used by publics
bool hugeint::compareSml (const hugeint &to_comp) const {
	if (neg != to_comp.neg) {
		return neg;
	}
	else if (bits.size() != to_comp.bits.size()) {
		return neg ^ (bits.size() < to_comp.bits.size());
	}
	else {
		for (std::size_t index = bits.size() - 1; index < bits.size(); index--) {
			if (bits[index] != to_comp.bits[index]) {
				return neg ^ (bits[index] < to_comp.bits[index]);
			}
		}
	}
	return false;
}

void hugeint::calculateAnd (const hugeint &to_and) {
	if (to_and.bits.size() < bits.size()) {
		resize(to_and.bits.size());
	}
	for (std::size_t index = 0; index < bits.size(); index++) {
		bits[index] &= to_and.bits[index];
	}
	neg = neg && to_and.neg;
	clearZeros();
}
void hugeint::calculateOr (const hugeint &to_or) {
	if (bits.size() < to_or.bits.size()) {
		resize(to_or.bits.size());
	}
	for (std::size_t index = 0; index < to_or.bits.size(); index++) {
		bits[index] |= to_or.bits[index];
	}
	if (to_or.neg && to_or.bits.size() < bits.size()) {
		bits.resize(to_or.bits.size());
	}
	neg = neg || to_or.neg;
	clearZeros();
}
void hugeint::calculateXor (const hugeint &to_xor) {
	if (bits.size() < to_xor.bits.size()) {
		resize(to_xor.bits.size());
	}
	for (std::size_t index = 0; index < to_xor.bits.size(); index++) {
		bits[index] ^= to_xor.bits[index];
	}
	if (to_xor.neg && to_xor.bits.size() < bits.size()) {
		for (std::size_t index = to_xor.bits.size(); index < bits.size(); index++) {
			bits[index] = ~bits[index];
		}
	}
	neg = neg || to_xor.neg;
	clearZeros();
}

void hugeint::shiftFwd (ullint val) {
	uint digadd = val >> 5;
	uint bitshift = val & 0x1f;
	if (bitshift) {
		bits.push_back(0);
		for (std::size_t index = bits.size() - 1; index > 0; index--) {
			bits[index] = (bits[index] << bitshift) + (bits[index - 1] >> (32 - bitshift));
		}
		bits[0] = bits[0] << bitshift;
		if (bits.back() == 0) {
			bits.pop_back();
		}
	}
	if (!bits.empty()) {
		for (uint index = 0; index < digadd; bits.push_front(0), index++);
	}
}
void hugeint::shiftBack (ullint val) {
	uint digdel = val >> 5;
	uint bitshift = val & 0x1f;
	for (uint index = 0; !bits.empty() && index < digdel; bits.pop_front(), index++);
	if (!bits.empty() && bitshift) {
		for (std::size_t index = 0; index < bits.size() - 1; index++) {
			bits[index] = (bits[index] >> bitshift) | (bits[index + 1] << (32 - bitshift));
		}
		bits.back() = bits.back() >> bitshift;
		if (bits.back() == 0) {
			bits.pop_back();
		}
	}
}

void hugeint::increment () {
	if (neg && bits.empty()) {
		bits.clear();
		neg = false;
	}
	else {
		if (!neg) {
			bits.push_back(0);
		}
		for (uint &word : bits) {
			if (word == 0xffffffff) {
				word = 0;
			}
			else {
				word++;
				break;
			}
		}
		clearZeros();
	}
}
void hugeint::decrement () {
	if (!neg && bits.empty()) {
		bits.clear();
		neg = true;
	}
	else {
		if (neg) {
			bits.push_back(0xffffffff);
		}
		for (uint &word : bits) {
			if (word == 0) {
				word = 0xffffffff;
			}
			else {
				word--;
				break;
			}
		}
		clearZeros();
	}
}

void hugeint::calculateAdd (const hugeint &to_add) {
	ullint sav = 0;
	resize(std::max(bits.size(), to_add.bits.size()) + 1);
	for (std::size_t index = 0; index < bits.size(); index++) {
		if (index >= to_add.bits.size() && !to_add.neg && sav == 0) {
			break;
		}
		sav += (ullint)bits[index] + (index < to_add.bits.size() ? to_add.bits[index] : (to_add.neg ? 0xffffffff : 0));
		bits[index] = (uint)sav;
		sav >>= 32;
	}
	neg = bits.back() & 0x80000000;
	clearZeros();
}
void hugeint::calculateDec (const hugeint &to_dec) {
	ullint sav = 0;
	resize(std::max(bits.size(), to_dec.bits.size()) + 1);
	for (std::size_t index = 0; index < bits.size(); index++) {
		if (index >= to_dec.bits.size() && !to_dec.neg && sav == 0) {
			break;
		}
		sav += (ullint)bits[index] - (index < to_dec.bits.size() ? to_dec.bits[index] : (to_dec.neg ? 0xffffffff : 0));
		bits[index] = (uint)sav;
		sav = (sav & 0xffffffff00000000) + (sav >> 32); // keep the negative for carry
	}
	neg = bits.back() & 0x80000000;
	clearZeros();
}

void hugeint::calculateMult (const hugeint &to_mult) {
	bool is_neg = neg ^ to_mult.neg;
	if (neg) {
		negate();
	}
	const hugeint *calc;
	int max_size = 0;
	if (to_mult.neg) {
		auto *negated = new hugeint(to_mult);
		negated->negate();
		calc = negated;
	}
	else {
		calc = &to_mult;
	}
	for (max_size = 1; max_size < std::max(calc->bits.size(), this->bits.size()); max_size <<= 1);
	*this = doMultAlgorithm(*this, *calc, max_size);
	if (to_mult.neg) {
		delete calc;
	}
	if (is_neg) {
		negate();
	}
}

void hugeint::calculateDiv (const hugeint &to_div) {
	hugeint ans, rest;
	bool is_neg = neg ^ to_div.neg;
	if (neg) {
		negate();
	}
	const hugeint *calc = nullptr;
	if (to_div.neg) {
		auto *negated = new hugeint(to_div);
		negated->negate();
		calc = negated;
	}
	else {
		calc = &to_div;
	}
	for (std::size_t index = bits.size() - 1; index < bits.size(); index--) {
		rest.bits.push_front(bits[index]);
		ans.bits.push_front(*calc <= rest ? divBinSearch(rest, *calc) : 0);
	}
	if (to_div.neg) {
		delete calc;
	}
	if (is_neg) {
		ans.negate();
	}
	*this = ans;
}
void hugeint::calculateMod (const hugeint &to_div) {
	hugeint rest;
	bool is_neg = neg ^ to_div.neg;
	if (neg) {
		negate();
	}
	const hugeint *calc = nullptr;
	if (to_div.neg) {
		auto *negated = new hugeint(to_div);
		negated->negate();
		calc = negated;
	}
	else {
		calc = &to_div;
	}
	for (std::size_t index = bits.size() - 1; index < bits.size(); index--) {
		if (!rest.bits.empty() || bits[index]) {
			rest.bits.push_front(bits[index]);
			if (*calc <= rest) {
				divBinSearch(rest, *calc);
			}
		}
	}
	if (to_div.neg) {
		delete calc;
	}
	if (is_neg) {
		rest.negate();
	}
	*this = rest;
}

void hugeint::setRamdon (size_t size, bool rand_sign) {
	std::default_random_engine generator;
	std::uniform_int_distribution <uint> distribution(0, 0xffffffff);
	auto randomDigit = std::bind(distribution, generator);
	neg = rand_sign && (randomDigit() & 1);
	bits.clear();
	for (int index = 0; index < size >> 5; index++) {
		bits.push_back(randomDigit());
	}
	if (size & 0x1f) {
		bits.push_back(randomDigit() & ((1ull << (size & 0x1f)) - 1));
	}
	clearZeros();
}

void hugeint::calculateGcd (hugeint other) {
	hugeint rest;
	while (other) {
		rest = (*this) % other;
		*this = other;
		other = rest;
	}
}

void hugeint::calculatePow (ullint exponent) {
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
void hugeint::calculatePow (ullint exponent, const hugeint &to_mod) {
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

hugeint hugeint::calculateNthRoot (ullint degree) const {
	hugeint ans;
	for (size_t pos = size() / degree; size() / degree; pos--) {
		ans.flipBit(pos);
		if (::pow(ans, degree) > *this) {
			ans.flipBit(pos);
		}
	}
	return ans;
}