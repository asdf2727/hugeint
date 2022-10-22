#pragma once

#include <iostream> //overload of << and >>
#include <string>
#include <deque>
#include <cmath>
#include <cstring> // memset
#include <type_traits> // enable_if


#define NOT_HUGEINT_TEMP template <typename NotHugeint, typename Dummy = std::enable_if <!std::is_same <NotHugeint, hugeint>::value, bool>>
#define INTEGER_TEMP template <typename Integer, typename Dummy = std::enable_if <std::is_integral <Integer>::value, bool>>
#define UNSIGNED_TEMP template <typename IntegerU, typename Dummy = std::enable_if <std::is_unsigned <IntegerU>::value, bool>>

class hugeint {
	// General declarations
private:
	typedef long long int llint;
	typedef long int lint;
	typedef short int sint;
	typedef unsigned long long int ullint;
	typedef unsigned long int ulint;
	typedef unsigned int uint;
	typedef unsigned short int usint;

	bool neg;
	std::deque <uint> bits;

	// Casting functions
private:
	void fromHex (const std::string::const_iterator &start, const std::string::const_iterator &stop, size_t &errPos);
	void fromDec (const std::string::const_iterator &start, const std::string::const_iterator &stop, size_t &errPos);
	void fromOct (const std::string::const_iterator &start, const std::string::const_iterator &stop, size_t &errPos);
	void fromBin (const std::string::const_iterator &start, const std::string::const_iterator &stop, size_t &errPos);

public:
	// Reads a string beetwen the two iterators (stop exclusive) and interprets it as a number in hexadecimal, decimal, octal or binary.
	size_t fromString (const std::string::const_iterator &start, const std::string::const_iterator &stop);

	// Returns a string in hexadecimal equal to self.
	std::string toHex () const;
	// Returns a string in decimal equal to self.
	std::string toDec () const;
	// Returns a string in octal equal to self.
	std::string toOct () const;
	// Returns a string in binary equal to self.
	std::string toBin () const;

	friend std::ostream &operator<< (std::ostream &out, const hugeint &to_show);
	friend std::istream &operator>> (std::istream &in, hugeint &to_set);

	hugeint ();
	hugeint (hugeint &&to_copy) noexcept;
	hugeint (const hugeint &to_copy);
	hugeint (const bool &to_copy);
	hugeint (const sint &to_copy);
	hugeint (const int &to_copy);
	hugeint (const llint &to_copy);
	hugeint (const usint &to_copy);
	hugeint (const uint &to_copy);
	hugeint (const ullint &to_copy);
	hugeint (const std::string &to_copy);

	operator bool () const;
	operator short int () const;
	operator int () const;
	operator long long int () const;
	operator unsigned short int () const;
	operator unsigned int () const;
	operator unsigned long long int () const;
	operator std::string () const;

	hugeint &operator= (hugeint &&to_copy) noexcept;
	hugeint &operator= (const hugeint &to_copy) = default;
	hugeint &operator= (const bool &to_copy);
	hugeint &operator= (const sint &to_copy);
	hugeint &operator= (const int &to_copy);
	hugeint &operator= (const llint &to_copy);
	hugeint &operator= (const usint &to_copy);
	hugeint &operator= (const uint &to_copy);
	hugeint &operator= (const ullint &to_copy);
	hugeint &operator= (const std::string &to_copy);

	// Mathematical functions
private:
	ullint size () const;
	void clearZeros ();
	void resize (std::size_t new_size);
	void invert ();

	static hugeint simpleMult (const hugeint &num1, const hugeint &num2);
	hugeint karatsuba (const hugeint &num1, const hugeint &num2, std::size_t tot_size);
	inline hugeint doMultAlgorithm (const hugeint &num1, const hugeint &num2, std::size_t tot_size);

	bool compareSml (const hugeint &to_comp) const;

	hugeint &calculateAnd (const hugeint &to_and);
	hugeint &calculateOr (const hugeint &to_or);
	hugeint &calculateXor (const hugeint &to_xor);

	void shiftFwd (ullint val);
	void shiftBack (ullint val);

	hugeint &increment ();
	hugeint &decrement ();

	hugeint &calculateAdd (const hugeint &to_and);
	hugeint &calculateDec (const hugeint &to_dec);

	hugeint &calculateMult (const hugeint &to_mult);

	uint divBinSearch (hugeint &rest, const hugeint &to_div);
	hugeint &calculateDiv (const hugeint &to_div);
	hugeint &calculateMod (const hugeint &to_div);

	void calculatePow (ullint exponent);
	void calculatePow (ullint exponent, const hugeint &to_mod);

	hugeint calculateSqrRoot () const;
	hugeint calculateNthRoot (ullint degree) const;
public:
	// Negates self
	void negate ();
	
	bool getBit (size_t pos) const;
	void flipBit (size_t pos);
	void setBit (size_t pos, bool val);

	friend bool operator== (const hugeint &lhs, const hugeint &rhs) {
		return lhs.bits == rhs.bits && lhs.neg == rhs.neg;
	}
	NOT_HUGEINT_TEMP friend bool operator== (const hugeint &lhs, const NotHugeint &rhs) {
		return lhs == (hugeint)rhs;
	}
	NOT_HUGEINT_TEMP friend bool operator== (const NotHugeint &lhs, const hugeint &rhs) {
		return (hugeint)lhs == rhs;
	}

	friend bool operator!= (const hugeint &lhs, const hugeint &rhs) {
		return lhs.bits != rhs.bits || lhs.neg != rhs.neg;
	}
	NOT_HUGEINT_TEMP friend bool operator!= (const hugeint &lhs, const NotHugeint &rhs) {
		return lhs != (hugeint)rhs;
	}
	NOT_HUGEINT_TEMP friend bool operator!= (const NotHugeint &lhs, const hugeint &rhs) {
		return (hugeint)lhs != rhs;
	}

	friend bool operator&& (const hugeint &lhs, const hugeint &rhs) {
		return (bool)lhs && (bool)rhs;
	}
	NOT_HUGEINT_TEMP friend bool operator&& (const hugeint &lhs, const NotHugeint &rhs) {
		return (bool)lhs && (bool)rhs;
	}
	NOT_HUGEINT_TEMP friend bool operator&& (const NotHugeint &lhs, const hugeint &rhs) {
		return (bool)lhs && (bool)rhs;
	}

	friend bool operator|| (const hugeint &lhs, const hugeint &rhs) {
		return (bool)lhs || (bool)rhs;
	}
	NOT_HUGEINT_TEMP friend bool operator|| (const hugeint &lhs, const NotHugeint &rhs) {
		return (bool)lhs || (bool)rhs;
	}
	NOT_HUGEINT_TEMP friend bool operator|| (const NotHugeint &lhs, const hugeint &rhs) {
		return (bool)lhs || (bool)rhs;
	}

	friend bool operator< (const hugeint &lhs, const hugeint &rhs) {
		return lhs.compareSml(rhs);
	}
	NOT_HUGEINT_TEMP friend bool operator< (const hugeint &lhs, const NotHugeint &rhs) {
		return lhs.compareSml((hugeint)rhs);
	}
	NOT_HUGEINT_TEMP friend bool operator< (const NotHugeint &lhs, const hugeint &rhs) {
		return ((hugeint)lhs).compareSml(rhs);
	}

	friend bool operator> (const hugeint &lhs, const hugeint &rhs) {
		return rhs.compareSml(lhs);
	}
	NOT_HUGEINT_TEMP friend bool operator> (const hugeint &lhs, const NotHugeint &rhs) {
		return ((hugeint)rhs).compareSml(lhs);
	}
	NOT_HUGEINT_TEMP friend bool operator> (const NotHugeint &lhs, const hugeint &rhs) {
		return rhs.compareSml((hugeint)lhs);
	}

	friend bool operator<= (const hugeint &lhs, const hugeint &rhs) {
		return !rhs.compareSml(lhs);
	}
	NOT_HUGEINT_TEMP friend bool operator<= (const hugeint &lhs, const NotHugeint &rhs) {
		return !((hugeint)rhs).compareSml(lhs);
	}
	NOT_HUGEINT_TEMP friend bool operator<= (const NotHugeint &lhs, const hugeint &rhs) {
		return !rhs.compareSml((hugeint)lhs);
	}

	friend bool operator>= (const hugeint &lhs, const hugeint &rhs) {
		return !lhs.compareSml(rhs);
	}
	NOT_HUGEINT_TEMP friend bool operator>= (const hugeint &lhs, const NotHugeint &rhs) {
		return !((hugeint)lhs).compareSml(rhs);
	}
	NOT_HUGEINT_TEMP friend bool operator>= (const NotHugeint &lhs, const hugeint &rhs) {
		return !((hugeint)lhs).compareSml(rhs);
	}

	friend hugeint operator~ (const hugeint &rhs) {
		hugeint to_mod = rhs;
		to_mod.invert();
		return to_mod;
	}

	friend hugeint &operator&= (hugeint &lhs, const hugeint &rhs) {
		return lhs.calculateAnd(rhs);
	}
	NOT_HUGEINT_TEMP friend hugeint &operator&= (hugeint &lhs, const NotHugeint &rhs) {
		return lhs.calculateAnd((hugeint)rhs);
	}
	NOT_HUGEINT_TEMP friend NotHugeint &operator&= (NotHugeint &lhs, const hugeint &rhs) {
		return lhs &= (NotHugeint)rhs;
	}

	friend hugeint operator& (const hugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateAnd(rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator& (hugeint &lhs, const NotHugeint &rhs) {
		hugeint result = lhs;
		result.calculateAnd((hugeint)rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator& (NotHugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateAnd(rhs);
		return result;
	}

	friend hugeint &operator|= (hugeint &lhs, const hugeint &rhs) {
		return lhs.calculateOr(rhs);
	}
	NOT_HUGEINT_TEMP friend hugeint &operator|= (hugeint &lhs, const NotHugeint &rhs) {
		return lhs.calculateOr((hugeint)rhs);
	}
	NOT_HUGEINT_TEMP friend NotHugeint &operator|= (NotHugeint &lhs, const hugeint &rhs) {
		return lhs |= (NotHugeint)rhs;
	}

	friend hugeint operator| (const hugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateOr(rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator| (hugeint &lhs, const NotHugeint &rhs) {
		hugeint result = lhs;
		result.calculateOr((hugeint)rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator| (NotHugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateOr(rhs);
		return result;
	}

	friend hugeint &operator^= (hugeint &lhs, const hugeint &rhs) {
		return lhs.calculateXor(rhs);
	}
	NOT_HUGEINT_TEMP friend hugeint &operator^= (hugeint &lhs, const NotHugeint &rhs) {
		return lhs.calculateXor((hugeint)rhs);
	}
	NOT_HUGEINT_TEMP friend NotHugeint &operator^= (NotHugeint &lhs, const hugeint &rhs) {
		return lhs ^= (NotHugeint)rhs;
	}

	friend hugeint operator^ (const hugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateXor(rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator^ (hugeint &lhs, const NotHugeint &rhs) {
		hugeint result = lhs;
		result.calculateXor((hugeint)rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator^ (NotHugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateXor(rhs);
		return result;
	}

	INTEGER_TEMP friend hugeint &operator<<= (hugeint &lhs, const Integer &rhs) {
		if (rhs > 0) {
			lhs.shiftFwd(rhs);
		}
		else if (rhs < 0) {
			lhs.shiftBack(-rhs);
		}
		return lhs;
	}
	INTEGER_TEMP friend hugeint operator<< (const hugeint &lhs, const Integer &rhs) {
		hugeint calc = lhs;
		calc <<= rhs;
		return calc;
	}
	INTEGER_TEMP friend hugeint &operator>>= (hugeint &lhs, const Integer &rhs) {
		if (rhs > 0) {
			lhs.shiftBack(rhs);
		}
		else if (rhs < 0) {
			lhs.shiftFwd(-rhs);
		}
		return lhs;
	}
	INTEGER_TEMP friend hugeint operator>> (const hugeint &lhs, const Integer &rhs) {
		hugeint calc = lhs;
		calc >>= rhs;
		return calc;
	}

	friend hugeint &operator++ (hugeint &rhs) {
		return rhs.increment();
	}
	friend hugeint operator++ (hugeint &lhs, int) {
		hugeint sav = lhs;
		lhs.increment();
		return sav;
	}
	friend hugeint &operator-- (hugeint &rhs) {
		return rhs.decrement();
	}
	friend hugeint operator-- (hugeint &lhs, int) {
		hugeint sav = lhs;
		lhs.decrement();
		return sav;
	}

	friend hugeint &operator+= (hugeint &lhs, const hugeint &rhs) {
		return lhs.calculateAdd(rhs);
	}
	NOT_HUGEINT_TEMP friend hugeint &operator+= (hugeint &lhs, const NotHugeint &rhs) {
		return lhs.calculateAdd((hugeint)rhs);
	}
	NOT_HUGEINT_TEMP friend NotHugeint &operator+= (NotHugeint &lhs, const hugeint &rhs) {
		return lhs += (NotHugeint)rhs;
	}

	friend hugeint operator+ (const hugeint &lhs) {
		return lhs;
	}

	friend hugeint operator+ (const hugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateAdd(rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator+ (const hugeint &lhs, const NotHugeint &rhs) {
		hugeint result = lhs;
		result.calculateAdd((hugeint)rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator+ (const NotHugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateAdd(rhs);
		return result;
	}

	friend hugeint &operator-= (hugeint &lhs, const hugeint &rhs) {
		return lhs.calculateDec(rhs);
	}
	NOT_HUGEINT_TEMP friend hugeint &operator-= (hugeint &lhs, const NotHugeint &rhs) {
		return lhs.calculateDec((hugeint)rhs);
	}
	NOT_HUGEINT_TEMP friend NotHugeint &operator-= (NotHugeint &lhs, const hugeint &rhs) {
		return lhs -= (NotHugeint)rhs;
	}

	friend hugeint operator- (const hugeint &lhs) {
		hugeint result = lhs;
		result.negate();
		return result;
	}

	friend hugeint operator- (const hugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateDec(rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator- (const hugeint &lhs, const NotHugeint &rhs) {
		hugeint result = lhs;
		result.calculateDec((hugeint)rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend NotHugeint operator- (const NotHugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateDec(rhs);
		return result;
	}

	friend hugeint &operator*= (hugeint &lhs, const hugeint &rhs) {
		return lhs.calculateMult(rhs);
	}
	NOT_HUGEINT_TEMP friend hugeint &operator*= (hugeint &lhs, const NotHugeint &rhs) {
		return lhs.calculateMult((hugeint)rhs);
	}
	NOT_HUGEINT_TEMP friend NotHugeint &operator*= (NotHugeint &lhs, const hugeint &rhs) {
		return lhs *= (NotHugeint)rhs;
	}

	friend hugeint operator* (const hugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateMult(rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator* (const hugeint &lhs, const NotHugeint &rhs) {
		hugeint result = lhs;
		result.calculateMult((hugeint)rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator* (const NotHugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateMult(rhs);
		return result;
	}

	friend hugeint &operator/= (hugeint &lhs, const hugeint &rhs) {
		return lhs.calculateDiv(rhs);
	}
	NOT_HUGEINT_TEMP friend hugeint &operator/= (hugeint &lhs, const NotHugeint &rhs) {
		return lhs.calculateDiv((hugeint)rhs);
	}
	NOT_HUGEINT_TEMP friend NotHugeint &operator/= (NotHugeint &lhs, const hugeint &rhs) {
		return lhs /= (NotHugeint)rhs;
	}

	friend hugeint operator/ (const hugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateDiv(rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator/ (const hugeint &lhs, const NotHugeint &rhs) {
		hugeint result = lhs;
		result.calculateDiv((hugeint)rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator/ (const NotHugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateDiv(rhs);
		return result;
	}

	friend hugeint &operator%= (hugeint &lhs, const hugeint &rhs) {
		return lhs.calculateMod(rhs);
	}
	NOT_HUGEINT_TEMP friend hugeint &operator%= (hugeint &lhs, const NotHugeint &rhs) {
		return lhs.calculateMod((hugeint)rhs);
	}
	NOT_HUGEINT_TEMP friend NotHugeint &operator%= (NotHugeint &lhs, const hugeint &rhs) {
		return lhs %= (NotHugeint)rhs;
	}

	friend hugeint operator% (const hugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateMod(rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator% (const hugeint &lhs, const NotHugeint &rhs) {
		hugeint result = lhs;
		result.calculateMod((hugeint)rhs);
		return result;
	}
	NOT_HUGEINT_TEMP friend hugeint operator% (const NotHugeint &lhs, const hugeint &rhs) {
		hugeint result = lhs;
		result.calculateMod(rhs);
		return result;
	}

	hugeint abs () {
		if (neg) {
			negate();
		}
		return *this;
	}
	friend hugeint abs (hugeint to_abs) {
		return to_abs.abs();
	}

	// Turns self into (self ^ exponent).
	UNSIGNED_TEMP hugeint pow (IntegerU exponent) {
		calculatePow(exponent);
		return *this;
	}
	// Turns self into (self ^ exponent % modulo).
	UNSIGNED_TEMP hugeint pow (IntegerU exponent, const hugeint &modulo) {
		calculatePow(exponent, modulo);
		return *this;
	}

	// Returns the floor of the square root.
	hugeint sqrt () const {
		return calculateSqrRoot();
	}
	friend hugeint sqrt (const hugeint &num) {
		return num.sqrt();
	}
	// Returns the floor of the cubic root.
	hugeint cbrt () const {
		return calculateNthRoot(3);
	}
	friend hugeint cbrt (const hugeint &num) {
		return num.cbrt();
	}
	// Returns the floor of the nth degree root.
	UNSIGNED_TEMP hugeint nthroot (IntegerU degree) const {
		return calculateNthRoot(degree);
	}
	UNSIGNED_TEMP friend hugeint nthroot (const hugeint &num, IntegerU degree) {
		return num.nthroot(degree);
	}
};

// Returns base ^ exponent.
UNSIGNED_TEMP hugeint pow (hugeint base, IntegerU exponent) {
	base.pow(exponent);
	return base;
}
// Returns base ^ exponent % modulo.
UNSIGNED_TEMP hugeint pow (hugeint base, IntegerU exponent, const hugeint &modulo) {
	base.pow(exponent, modulo);
	return base;
}

#undef UNSIGNED_TEMP
#undef INTEGER_TEMP
#undef NOT_HUGEINT_TEMP
