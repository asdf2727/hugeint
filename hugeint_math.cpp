#include "hugeint.h"

// General

// Size also works as log2 (gives the biggest bit 1)
unsigned long long int hugeint::size() const {
	ullint size = bits.size() << 5;
	for(uint index = 0x80000000; (index & (neg ? ~bits.back() : bits.back())) == 0; index >>= 1, size--);
	return size;
}
void hugeint::resize (std::size_t new_size) {
	std::size_t old_size = bits.size();
	bits.resize(new_size);
	if (neg && new_size > old_size) {
		for (; old_size < new_size; old_size++) {
			bits[old_size] = 0xffffffff;
		}
	}
}
void hugeint::clearZeros () {
	while (!bits.empty() && bits.back() == (neg ? 0xffffffff : 0)) {
		bits.pop_back();
	}
}
void hugeint::invert () {
	for (uint &word : bits) {
		word = ~word;
	}
}
void hugeint::negate () {
	neg = !neg;
	invert();
	this->increment();
	clearZeros();
}

// Explination for the Karatsuba fast multiplication agorithm: https://en.wikipedia.org/wiki/Karatsuba_algorithm
bool hugeint::addDeque (std::deque <uint> &nr1, const std::deque <uint> &nr2, bool addlast) {
	llint rez = 0;
	for (std::size_t index = 0; index < nr1.size(); index++) {
		rez += (llint)nr1[index] + (index < nr2.size() ? nr2[index] : 0);
		nr1[index] = rez;
		rez >>= 32;
	}
	if (addlast) {
		if (rez != 0) {
			nr1.push_back(1);
		}
	}
	return rez != 0;
}
void hugeint::decDeque (std::deque <uint> &nr1, const std::deque <uint> &nr2) {
	llint rez;
	for (std::size_t index = 0; index < nr2.size(); index++) {
		rez = (llint)nr1[index] - nr2[index];
		if (rez < 0) {
			nr1[index + 1]--;
			rez += 0x0000000100000000;
		}
		nr1[index] = rez;
	}
}
std::deque <unsigned int> hugeint::karatsuba (std::deque <uint> half11, std::deque <uint> half21, std::size_t tot_size) {
	if (tot_size <= 1) {
		half11.push_back(((ullint)half11[0] * half21[0]) >> 32);
		half11[0] *= half21[0];
		return half11;
	}
	else {
		std::deque <uint> half12, half22, calc1, calc2, calc3;
		bool add1, add2;
		for (std::size_t index = tot_size >> 1; index < tot_size; index++) {
			half12.push_back(half11[index]);
			half22.push_back(half21[index]);
		}
		tot_size >>= 1;
		half11.resize(tot_size);
		half21.resize(tot_size);
		calc1 = karatsuba(half11, half21, tot_size);
		calc2 = karatsuba(half12, half22, tot_size);
		add1 = addDeque(half11, half12, false);
		add2 = addDeque(half21, half22, false);
		calc3 = karatsuba(half11, half21, tot_size);
		if (add1) {
			for (std::size_t index = 0; index < tot_size; index++) {
				half21.push_front(0);
			}
			addDeque(calc3, half21, true);
		}
		if (add2) {
			for (std::size_t index = 0; index < tot_size; index++) {
				half11.push_front(0);
			}
			addDeque(calc3, half11, true);
		}
		if (add1 && add2) {
			if (calc3.size() < (tot_size << 1) + 1) {
				calc3.push_back(1);
			}
			else {
				calc3.back()++;
			}
		}
		decDeque(calc3, calc1);
		decDeque(calc3, calc2);
		for (std::size_t index = 0; index < tot_size << 1; index++) {
			calc2.push_front(0);
		}
		addDeque(calc2, calc1, false);
		for (std::size_t index = 0; index < tot_size; index++) {
			calc3.push_front(0);
		}
		addDeque(calc2, calc3, false);
		return calc2;
	}
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

// Called by publics
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

hugeint &hugeint::calculateAnd (const hugeint &to_and) {
	if (to_and.bits.size() < bits.size()) {
		resize(to_and.bits.size());
	}
	for (std::size_t index = 0; index < bits.size(); index++) {
		bits[index] &= to_and.bits[index];
	}
	neg = neg && to_and.neg;
	clearZeros();
	return *this;
}
hugeint &hugeint::calculateOr (const hugeint &to_or) {
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
	return *this;
}
hugeint &hugeint::calculateXor (const hugeint &to_xor) {
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
	return *this;
}

void hugeint::shiftFwd (ullint val) {
	uint digadd = val >> 5;
	uint bitshift = val % 32;
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
	for (uint index = 0; index < digadd; bits.push_front(0), index++);
}
void hugeint::shiftBack (ullint val) {
	uint digdel = val >> 5;
	uint bitshift = val % 32;
	for (uint index = 0; !bits.empty() && index < digdel; bits.pop_front(), index++);
	if (!bits.empty() && bitshift) {
		for (std::size_t index = 0; index < bits.size() - 1; index++) {
			bits[index] = (bits[index] >> bitshift) + (bits[index + 1] << (32 - bitshift));
		}
		bits.back() = bits.back() >> bitshift;
		if (bits.back() == 0) {
			bits.pop_back();
		}
	}
}

hugeint &hugeint::increment () {
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
	return *this;
}
hugeint &hugeint::decrement () {
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
	return *this;
}

hugeint &hugeint::calculateAdd (const hugeint &to_add) {
	ullint sav = 0;
	resize(std::max(bits.size(), to_add.bits.size()) + 1);
	for (std::size_t index = 0; index < bits.size(); index++) {
		sav += (ullint)bits[index] + (index < to_add.bits.size() ? to_add.bits[index] : (to_add.neg ? 0xffffffff : 0));
		bits[index] = (uint)sav;
		sav >>= 32;
	}
	neg = bits.back() & 0x80000000;
	clearZeros();
	return *this;
}
hugeint &hugeint::calculateDec (const hugeint &to_dec) {
	ullint sav = 0;
	resize(std::max(bits.size(), to_dec.bits.size()) + 1);
	for (std::size_t index = 0; index < bits.size(); index++) {
		sav += (ullint)bits[index] - (index < to_dec.bits.size() ? to_dec.bits[index] : (to_dec.neg ? 0xffffffff : 0));
		bits[index] = (uint)sav;
		sav = (sav & 0xffffffff00000000) + (sav >> 32); // keep the negative for carry
	}
	neg = bits.back() & 0x80000000;
	clearZeros();
	return *this;
}

hugeint &hugeint::calculateMult (const hugeint &to_mult) {
	bool is_neg = neg ^ to_mult.neg;
	if (neg) {
		negate();
	}
	std::deque <uint> num1, num2;
	int max_size = 0;
	num1 = bits;
	if (to_mult.neg) {
		auto *negated = new hugeint(to_mult);
		negated->negate();
		num2 = negated->bits;
		delete negated;
	}
	else {
		num2 = to_mult.bits;
	}
	for (max_size = 1; max_size < std::max(num1.size(), num2.size()); max_size <<= 1);
	num1.resize(max_size);
	num2.resize(max_size);
	bits = karatsuba(num1, num2, max_size);
	clearZeros();
	if (is_neg) {
		negate();
	}
	return *this;
}

hugeint &hugeint::calculateDiv (const hugeint &to_div) {
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
	if(to_div.neg){
		delete calc;
	}
	if (is_neg) {
		ans.negate();
	}
	return *this = ans;
}
hugeint &hugeint::calculateMod (const hugeint &to_div) {
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
		rest.bits.push_front(bits[index]);
		if (*calc <= rest) {
			divBinSearch(rest, *calc);
		}
	}
	if(to_div.neg) {
		delete calc;
	}
	if (is_neg) {
		rest.negate();
	}
	return *this = rest;
}

hugeint hugeint::calculatePow(ullint exponent) {
	hugeint result = 1;
	for(ullint bit = 1; bit <= exponent; bit <<= 1) {
		if (exponent & bit) {
			result *= *this;
		}
		*this *= *this;
	}
	return *this = result;
}
hugeint hugeint::calculateModPow(ullint exponent, const hugeint &to_div) {
	hugeint result = 1;
	for(ullint bit = 1; bit <= exponent; bit <<= 1) {
		if (exponent & bit) {
			result *= *this;
			result %= to_div;
		}
		*this *= *this;
		*this %= to_div;
	}
	return *this = result;
}