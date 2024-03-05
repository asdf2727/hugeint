#pragma once

#include <iostream> //overload of << and >>
#include <string>
#include <vector>
#include <cstdint>
#include <type_traits> // enable_if

// #define DIGIT_64 // Uncomment this to enable manual 64 bit digit size
// #define DIGIT_32 // Uncomment this to enable manual 32 bit digit size

#ifndef DIGIT_64
#ifndef DIGIT_32
#ifdef __SIZEOF_INT128__ // Automatic detection of __int128. If it doesn't work choose an option manually!
#define DIGIT_64
#else
#define DIGIT_32
#endif
#endif
#endif

#define NOT_HUGEINT_TEMP \
template <typename NotHugeint, typename = std::enable_if <!std::is_same <NotHugeint, hugeint>::value>>

#define INTEGER_TEMP \
template <typename Integer, typename = std::enable_if <!std::is_integral <Integer>::value>>

#define COMPARE(op_name, implement) \
friend bool operator op_name (const hugeint &lhs, const hugeint &rhs) { return implement; } \
NOT_HUGEINT_TEMP friend bool operator op_name (const hugeint &lhs, const NotHugeint &rhs) { return lhs op_name (hugeint)rhs; } \
NOT_HUGEINT_TEMP friend bool operator op_name (const NotHugeint &lhs, const hugeint &rhs) { return (hugeint)lhs op_name rhs; }

#define OPERATION(op_name, implement) \
friend hugeint operator op_name (const hugeint &lhs, const hugeint &rhs) { return implement; } \
NOT_HUGEINT_TEMP friend hugeint operator op_name (const hugeint &lhs, const NotHugeint &rhs) { return lhs op_name (hugeint)rhs; } \
NOT_HUGEINT_TEMP friend hugeint operator op_name (const NotHugeint &lhs, const hugeint &rhs) { return (hugeint)lhs op_name rhs; }

#define ASSIGN(op_name, implement) \
friend hugeint &operator op_name (hugeint &lhs, const hugeint &rhs) { implement; return lhs; } \
NOT_HUGEINT_TEMP friend hugeint &operator op_name (hugeint &lhs, const NotHugeint &rhs) { return lhs op_name (hugeint)rhs; } \
NOT_HUGEINT_TEMP friend NotHugeint &operator op_name (NotHugeint &lhs, const hugeint &rhs) { return lhs op_name (NotHugeint)rhs; }

class hugeint {
private:
#ifdef DIGIT_64
	typedef uint64_t digit_t;
	static const int digit_len = 64;
	static const int digit_log_len = 6;
	static const digit_t digit_max = 0xffffffffffffffff;
	typedef unsigned __int128 double_t;
#endif
#ifdef DIGIT_32
	typedef uint32_t digit_t;
	static const int digit_len = 32;
	static const int digit_log_len = 5;
	static const digit_t digit_max = 0xffffffff;
	typedef uint64_t double_t;
#endif

	typedef std::size_t size_t;
	typedef uint64_t shift_t;
	typedef uint64_t exp_t;
	typedef uint64_t root_t;

	bool neg = false;
	std::vector <digit_t> digits;

	// ----- Casting functions -----
private:
	size_t fromHex (const std::string::const_iterator &begin, const std::string::const_iterator &end);
	size_t fromDec (const std::string::const_iterator &begin, const std::string::const_iterator &end);
	size_t fromOct (const std::string::const_iterator &begin, const std::string::const_iterator &end);
	size_t fromBin (const std::string::const_iterator &begin, const std::string::const_iterator &end);

public:
	// Reads a string between [begin, end) and interprets it as a number in hexadecimal, decimal, octal or binary.
	// It returns 0xffffffffffffffff if no parsing errors were found, else it returns the position of the first error.
	size_t fromString (std::string::const_iterator begin, std::string::const_iterator end);

	std::string toHex () const;
	std::string toDec () const;
	std::string toOct () const;
	std::string toBin () const;

	hugeint ();
	hugeint (hugeint &&to_copy) noexcept;
	hugeint (const hugeint &to_copy) = default;
	hugeint (bool to_copy);
	hugeint (int16_t to_copy);
	hugeint (uint16_t to_copy);
	hugeint (int32_t to_copy);
	hugeint (uint32_t to_copy);
	hugeint (int64_t to_copy);
	hugeint (uint64_t to_copy);
#ifdef DIGIT_64
	hugeint (__int128 to_copy);
	hugeint (unsigned __int128 to_copy);
#endif
	hugeint (float to_copy);
	hugeint (double to_copy);
	hugeint (const std::string &to_copy);
	hugeint (const char *to_copy);

	hugeint &operator= (hugeint &&to_copy) noexcept;
	hugeint &operator= (const hugeint &to_copy) = default;
	hugeint &operator= (bool to_copy);
	hugeint &operator= (int16_t to_copy);
	hugeint &operator= (uint16_t to_copy);
	hugeint &operator= (int32_t to_copy);
	hugeint &operator= (uint32_t to_copy);
	hugeint &operator= (int64_t to_copy);
	hugeint &operator= (uint64_t to_copy);
#ifdef DIGIT_64
	hugeint &operator= (__int128 to_copy);
	hugeint &operator= (unsigned __int128 to_copy);
#endif
	hugeint &operator= (float to_copy);
	hugeint &operator= (double to_copy);
	hugeint &operator= (const std::string &to_copy);
	hugeint &operator= (const char *to_copy);

	explicit operator bool () const;
	explicit operator int16_t () const;
	explicit operator uint16_t () const;
	explicit operator int32_t () const;
	explicit operator uint32_t () const;
	operator int64_t () const;
	operator uint64_t () const;
#ifdef DIGIT_64
	explicit operator __int128 () const;
	explicit operator unsigned __int128 () const;
#endif
	explicit operator float () const;
	explicit operator double () const;
	explicit operator std::string () const;

	// ----- Mathematical functions -----

	// Utility functions
private:
	void clearZeros ();
	void resize (size_t new_size);
	void invert ();

public:
	size_t size () const;
	void negate ();

	// Bit manipulation functions
	bool getBit (size_t pos) const;
	void flipBit (size_t pos);
	void setBit (size_t pos, bool val);

	// operator calculation functions
private:
	bool compareSml (const hugeint &to_comp) const;

	static hugeint calculateAnd (const hugeint &lhs, const hugeint &rhs);
	void calculateAnd (const hugeint &rhs);

	static hugeint calculateOr (const hugeint &lhs, const hugeint &rhs);
	void calculateOr (const hugeint &rhs);

	static hugeint calculateXor (const hugeint &lhs, const hugeint &rhs);
	void calculateXor (const hugeint &rhs);

	void shiftFwd (shift_t val);
	void shiftBack (shift_t val);

	void increment ();
	void decrement ();

	static hugeint calculateAdd (const hugeint &lhs, const hugeint &rhs, const bool diff = false);
	void calculateAdd (const hugeint &rhs, const bool diff = false);

	static hugeint simpleMult (std::vector <digit_t>::iterator begin1, std::vector <digit_t>::iterator end1,
	                           std::vector <digit_t>::iterator begin2, std::vector <digit_t>::iterator end2);
	static hugeint addKaratsuba (std::vector <digit_t>::iterator begin1, std::vector <digit_t>::iterator end1,
	                             std::vector <digit_t>::iterator begin2, std::vector <digit_t>::iterator end2);
	static hugeint karatsuba (std::vector <digit_t>::iterator begin1, std::vector <digit_t>::iterator end1,
	                          std::vector <digit_t>::iterator begin2, std::vector <digit_t>::iterator end2, size_t block_size);
	static hugeint multAlgorithm (std::vector <digit_t>::iterator begin1, std::vector <digit_t>::iterator end1,
	                              std::vector <digit_t>::iterator begin2, std::vector <digit_t>::iterator end2, size_t block_size);
	static hugeint calculateMult (const hugeint &lhs, const hugeint &rhs);

	static hugeint simpleDiv (hugeint lhs, hugeint rhs, bool rem = false);
	static hugeint calculateDiv (const hugeint &lhs, const hugeint &rhs);
	static hugeint calculateMod (const hugeint &lhs, const hugeint &rhs);

	void setRamdon (size_t size, bool rand_sign); // Do not change

	void calculateGcd (hugeint other);

	void calculatePow (exp_t exponent);
	void calculatePow (exp_t exponent, const hugeint &to_mod);

	void calculateNthRoot (root_t degree);

	// Comparison operators
public:
	COMPARE(&&, (bool)lhs && (bool)rhs)
	COMPARE(||, (bool)lhs || (bool)rhs)

	COMPARE(==, lhs.neg == rhs.neg && lhs.digits == rhs.digits)
	COMPARE(!=, lhs.neg != rhs.neg || lhs.digits != rhs.digits)

	COMPARE(<, lhs.compareSml(rhs))
	COMPARE(>, rhs.compareSml(lhs))
	COMPARE(<=, !rhs.compareSml(lhs))
	COMPARE(>=, !lhs.compareSml(rhs))

	// Bit operators
	friend hugeint operator~ (const hugeint &rhs) {
		hugeint to_mod = rhs;
		to_mod.invert();
		return to_mod;
	}

	ASSIGN(&=, lhs.calculateAnd(rhs))
	OPERATION(&, calculateAnd(lhs, rhs))
	ASSIGN(|=, lhs.calculateOr(rhs))
	OPERATION(|, calculateOr(lhs, rhs))
	ASSIGN(^=, lhs.calculateXor(rhs))
	OPERATION(^, calculateXor(lhs, rhs))

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
		hugeint result(lhs);
		result <<= rhs;
		return result;
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
		hugeint result(lhs);
		result >>= rhs;
		return result;
	}

	// Addition and subtraction operators
	friend hugeint &operator++ (hugeint &rhs) {
		rhs.increment();
		return rhs;
	}
	friend const hugeint operator++ (hugeint &lhs, int) {
		hugeint result(lhs);
		lhs.increment();
		return result;
	}
	friend hugeint &operator-- (hugeint &rhs) {
		rhs.decrement();
		return rhs;
	}
	friend const hugeint operator-- (hugeint &lhs, int) {
		hugeint result(lhs);
		lhs.decrement();
		return result;
	}

	friend hugeint operator+ (const hugeint &lhs) {
		return lhs;
	}
	friend hugeint operator- (const hugeint &lhs) {
		hugeint result(lhs);
		result.negate();
		return result;
	}

	ASSIGN(+=, lhs.calculateAdd(rhs, false))
	OPERATION(+, calculateAdd(lhs, rhs, false))
	ASSIGN(-=, lhs.calculateAdd(rhs, true))
	OPERATION(-, calculateAdd(lhs, rhs, true))

	// Multiply and division operators
	ASSIGN(*=, lhs = calculateMult(lhs, rhs))
	OPERATION(*, calculateMult(lhs, rhs))
	ASSIGN(/=, lhs = calculateDiv(lhs, rhs))
	OPERATION(/, calculateDiv(lhs, rhs))
	ASSIGN(%=, lhs = calculateMod(lhs, rhs))
	OPERATION(%, calculateMod(lhs, rhs))

	// Extra functions
	hugeint pow (exp_t exponent) {
		calculatePow(exponent);
		return *this;
	}
	static hugeint pow (hugeint base, exp_t exponent) {
		base.calculatePow(exponent);
		return base;
	}
	hugeint powMod (exp_t exponent, const hugeint &modulo) {
		calculatePow(exponent, modulo);
		return *this;
	}
	static hugeint powMod (hugeint base, exp_t exponent, const hugeint &modulo) {
		base.calculatePow(exponent, modulo);
		return base;
	}

	hugeint abs () {
		if (neg) {
			negate();
		}
		return *this;
	}
	static hugeint abs (hugeint to_abs) {
		if (to_abs.neg) {
			to_abs.negate();
		}
		return to_abs;
	}
	hugeint randomise (size_t size, bool rand_sign) {
		setRamdon(size, rand_sign);
		return *this;
	}
	static hugeint rand (size_t size, bool rand_sign) {
		hugeint ans;
		ans.setRamdon(size, rand_sign);
		return ans;
	}

	hugeint gcd (const hugeint &other) {
		calculateGcd(other);
		return *this;
	}
	static hugeint gcd (hugeint num1, const hugeint &num2) {
		num1.calculateGcd(num2);
		return num1;
	}

	hugeint lcm (const hugeint &other) {
		hugeint gcd = *this;
		gcd.calculateGcd(other);
		return *this / gcd * other;
	}
	static hugeint lcm (const hugeint &num1, const hugeint &num2) {
		hugeint gcd = num1;
		gcd.calculateGcd(num2);
		return num1 / gcd * num2;
	}

	hugeint sqrt () {
		calculateNthRoot(2);
		return *this;
	}
	static hugeint sqrt (hugeint num) {
		num.calculateNthRoot(2);
		return num;
	}
	hugeint cbrt () {
		calculateNthRoot(3);
		return *this;
	}
	static hugeint cbrt (hugeint num) {
		num.calculateNthRoot(3);
		return num;
	}
	hugeint nthroot (root_t degree) {
		calculateNthRoot(degree);
		return *this;
	}
	static hugeint nthroot (hugeint num, root_t degree) {
		num.calculateNthRoot(degree);
		return num;
	}
};

#undef NOT_HUGEINT_TEMP
#undef INTEGER_TEMP

#undef COMPARE
#undef OPERATION
#undef ASSIGN

std::ostream &operator<< (std::ostream &out, const hugeint &to_show);
std::ostream &operator<< (std::ostream &out, hugeint &&to_show);
std::istream &operator>> (std::istream &in, hugeint &to_set);