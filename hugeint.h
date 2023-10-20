#pragma once

#include <iostream> //overload of << and >>
#include <string>
#include <deque>
#include <cmath>
#include <climits>
#include <type_traits> // enable_if
#include <random>
#include <functional>

#define NOT_HUGEINT_TEMP template <typename NotHugeint, typename Dummy = std::enable_if <!std::is_same <NotHugeint, hugeint>::value, bool>>

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

	bool neg = false;
	std::deque <uint> bits;

	// Casting functions
private:
	void fromHex (const std::string::const_iterator &begin, const std::string::const_iterator &end, size_t &errPos);
	void fromDec (const std::string::const_iterator &begin, const std::string::const_iterator &end, size_t &errPos);
	void fromOct (const std::string::const_iterator &begin, const std::string::const_iterator &end, size_t &errPos);
	void fromBin (const std::string::const_iterator &begin, const std::string::const_iterator &end, size_t &errPos);

public:
	// Reads a string beetwen the two iterators (end exclusive) and interprets it as a number in hexadecimal, decimal, octal or binary.
	// It returns 0xffffffff if no parsing errors were found, else it returns the position of the first error.
	size_t fromString (std::string::const_iterator begin, std::string::const_iterator end);

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
	hugeint (const hugeint &to_copy) = default;
	hugeint (bool to_copy);
	hugeint (sint to_copy);
	hugeint (int to_copy);
	hugeint (llint to_copy);
	hugeint (usint to_copy);
	hugeint (uint to_copy);
	hugeint (ullint to_copy);
	hugeint (float to_copy);
	hugeint (double to_copy);
	hugeint (const std::string &to_copy);
	hugeint (const char *to_copy);

	hugeint &operator= (hugeint &&to_copy) noexcept;
	hugeint &operator= (const hugeint &to_copy) = default;
	hugeint &operator= (bool to_copy);
	hugeint &operator= (sint to_copy);
	hugeint &operator= (int to_copy);
	hugeint &operator= (llint to_copy);
	hugeint &operator= (usint to_copy);
	hugeint &operator= (uint to_copy);
	hugeint &operator= (ullint to_copy);
	hugeint &operator= (float to_copy);
	hugeint &operator= (double to_copy);
	hugeint &operator= (const std::string &to_copy);
	hugeint &operator= (const char *to_copy);

	explicit operator bool () const;
	explicit operator short int () const;
	explicit operator int () const;
	explicit operator long int () const;
	explicit operator long long int () const;
	explicit operator unsigned short int () const;
	explicit operator unsigned int () const;
	explicit operator unsigned long int () const;
	explicit operator unsigned long long int () const;
	explicit operator float () const;
	explicit operator double () const;
	explicit operator std::string () const;
	explicit operator const char * () const;

	// Mathematical functions
private:
	void clearZeros ();
	void resize (std::size_t new_size);
	void invert ();

	static hugeint simpleMult (const hugeint &num1, const hugeint &num2);
	hugeint karatsuba (hugeint &num1, hugeint &num2, std::size_t tot_size);
	inline hugeint doMultAlgorithm (hugeint &num1, hugeint &num2, std::size_t tot_size);

	bool compareSml (const hugeint &to_comp) const;

	void calculateAnd (const hugeint &to_and);
	void calculateOr (const hugeint &to_or);
	void calculateXor (const hugeint &to_xor);

	void shiftFwd (ullint val);
	void shiftBack (ullint val);

	void increment ();
	void decrement ();

	void calculateAdd (const hugeint &to_and);
	void calculateDec (const hugeint &to_dec);

	void calculateMult (const hugeint &to_mult);

	uint divBinSearch (hugeint &rest, const hugeint &to_div);
	void calculateDiv (const hugeint &to_div);
	void calculateMod (const hugeint &to_div);

	void setRamdon (size_t size, bool rand_sign);

	void calculateGcd (hugeint other);

	void calculatePow (ullint exponent);
	void calculatePow (ullint exponent, const hugeint &to_mod);

	hugeint calculateNthRoot (ullint degree) const;
public:
	// Negates self
	ullint size () const;
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
		lhs.calculateAnd(rhs);
		return lhs;
	}
	NOT_HUGEINT_TEMP friend hugeint &operator&= (hugeint &lhs, const NotHugeint &rhs) {
		lhs.calculateAnd((hugeint)rhs);
		return lhs;
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
		lhs.calculateOr(rhs);
		return lhs;
	}
	NOT_HUGEINT_TEMP friend hugeint &operator|= (hugeint &lhs, const NotHugeint &rhs) {
		lhs.calculateOr((hugeint)rhs);
		return lhs;
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
		lhs.calculateXor(rhs);
		return lhs;
	}
	NOT_HUGEINT_TEMP friend hugeint &operator^= (hugeint &lhs, const NotHugeint &rhs) {
		lhs.calculateXor((hugeint)rhs);
		return lhs;
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

	template <typename T> friend hugeint &operator<<= (hugeint &lhs, const T &rhs) {
		llint shift = rhs;
		if (shift > 0) {
			lhs.shiftFwd(shift);
		}
		else if (shift < 0) {
			lhs.shiftBack(-shift);
		}
		return lhs;
	}
	template <typename T> friend hugeint operator<< (const hugeint &lhs, const T &rhs) {
		hugeint calc = lhs;
		calc <<= rhs;
		return calc;
	}
	template <typename T> friend hugeint &operator>>= (hugeint &lhs, const T &rhs) {
		llint shift = rhs;
		if (shift > 0) {
			lhs.shiftBack(rhs);
		}
		else if (shift < 0) {
			lhs.shiftFwd(-rhs);
		}
		return lhs;
	}
	template <typename T> friend hugeint operator>> (const hugeint &lhs, const T &rhs) {
		hugeint calc = lhs;
		calc >>= rhs;
		return calc;
	}

	friend hugeint &operator++ (hugeint &rhs) {
		rhs.increment();
		return rhs;
	}
	friend const hugeint operator++ (hugeint &lhs, int) {
		hugeint sav = lhs;
		lhs.increment();
		return sav;
	}
	friend hugeint &operator-- (hugeint &rhs) {
		rhs.decrement();
		return rhs;
	}
	friend const hugeint operator-- (hugeint &lhs, int) {
		hugeint sav = lhs;
		lhs.decrement();
		return sav;
	}

	friend hugeint &operator+= (hugeint &lhs, const hugeint &rhs) {
		lhs.calculateAdd(rhs);
		return lhs;
	}
	NOT_HUGEINT_TEMP friend hugeint &operator+= (hugeint &lhs, const NotHugeint &rhs) {
		lhs.calculateAdd((hugeint)rhs);
		return lhs;
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
		lhs.calculateDec(rhs);
		return lhs;
	}
	NOT_HUGEINT_TEMP friend hugeint &operator-= (hugeint &lhs, const NotHugeint &rhs) {
		lhs.calculateDec((hugeint)rhs);
		return lhs;
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
		lhs.calculateMult(rhs);
		return lhs;
	}
	NOT_HUGEINT_TEMP friend hugeint &operator*= (hugeint &lhs, const NotHugeint &rhs) {
		lhs.calculateMult((hugeint)rhs);
		return lhs;
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
		lhs.calculateDiv(rhs);
		return lhs;
	}
	NOT_HUGEINT_TEMP friend hugeint &operator/= (hugeint &lhs, const NotHugeint &rhs) {
		lhs.calculateDiv((hugeint)rhs);
		return lhs;
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
		lhs.calculateMod(rhs);
		return lhs;
	}
	NOT_HUGEINT_TEMP friend hugeint &operator%= (hugeint &lhs, const NotHugeint &rhs) {
		lhs.calculateMod((hugeint)rhs);
		return lhs;
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

	template <typename T> hugeint pow (T exponent) {
		calculatePow((ullint)exponent);
		return *this;
	}
	template <typename T> hugeint pow (T exponent, const hugeint &modulo) {
		calculatePow((ullint)exponent, modulo);
		return *this;
	}

	inline hugeint abs () {
		if (neg) {
			negate();
		}
		return *this;
	}
	inline hugeint rand (size_t size, bool rand_sign) {
		setRamdon(size, rand_sign);
		return *this;
	}

	inline hugeint gcd (const hugeint &other) {
		calculateGcd(other);
		return *this;
	}
	hugeint lcm (const hugeint &other) {
		hugeint gcd = *this;
		gcd.calculateGcd(other);
		calculateMult(other);
		calculateDiv(gcd);
		return *this;
	}

	hugeint sqrt () {
		*this = calculateNthRoot(2);
		return *this;
	}
	hugeint cbrt () {
		*this = calculateNthRoot(3);
		return *this;
	}

	template <typename T> hugeint nthroot (T degree) {
		*this = calculateNthRoot((ullint)degree);
		return *this;
	}
};

namespace huge {
	template <typename T> hugeint pow (hugeint base, T exponent) {
		base.pow(exponent);
		return base;
	}
	template <typename T> hugeint pow (hugeint base, T exponent, const hugeint &modulo) {
		base.pow(exponent, modulo);
		return base;
	}

	inline hugeint abs (hugeint to_abs) {
		return to_abs.abs();
	}
	inline hugeint rand (size_t size, bool rand_sign) {
		hugeint ans;
		return ans.rand(size, rand_sign);
	}

	inline hugeint gcd (const hugeint &num1, const hugeint &num2) {
		hugeint ret = num1;
		return ret.gcd(num2);
	}
	inline hugeint lcm (const hugeint &num1, const hugeint &num2) {
		return num1 / gcd(num1, num2) * num2;
	}

	inline hugeint sqrt (const hugeint &num) {
		hugeint ret = num;
		return ret.sqrt();
	}
	inline hugeint cbrt (const hugeint &num) {
		hugeint ret = num;
		return ret.cbrt();
	}
	template <typename T> inline hugeint nthroot (const hugeint &num, T degree) {
		hugeint ret = num;
		return ret.nthroot(degree);
	}
}

#undef NOT_HUGEINT_TEMP