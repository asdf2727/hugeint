#pragma once

#include <iostream> //overload of << and >>
#include <string>
#include <vector>
#include <cstdint>
#include <type_traits> // enable_if

namespace huge {

// #define DIGIT_64 // Uncomment this to enable manual 64 bit digit size
#define DIGIT_32 // Uncomment this to enable manual 32 bit digit size

#ifndef DIGIT_64
#ifndef DIGIT_32
#ifdef __SIZEOF_INT128__ // Automatic detection of __int128. If it doesn't work choose an option manually!
#define DIGIT_64
#else
#define DIGIT_32
#endif
#endif
#endif

#ifdef DIGIT_64
	typedef uint64_t digit_t;
	const int digit_len = 64;
	const int digit_log_len = 6;
	const digit_t digit_max = 0xffffffffffffffff;
	typedef unsigned __int128 double_t;
#endif
#ifdef DIGIT_32
	typedef uint32_t digit_t;
	const int digit_len = 32;
	const int digit_log_len = 5;
	const digit_t digit_max = 0xffffffff;
	typedef uint64_t double_t;
#endif

	typedef std::size_t size_t;
	typedef uint64_t shift_t;
	typedef uint64_t exp_t;
	typedef uint64_t root_t;

#define NOT_HUGEINT_TEMP \
template <typename NotHugeint, typename = std::enable_if <!std::is_same <NotHugeint, hugeint>::value>>

#define INTEGER_TEMP \
template <typename Integer, typename = std::enable_if <!std::is_integral <Integer>::value>>

#define COMPARE(op_name, implement) \
friend inline bool operator op_name (const hugeint &lhs, const hugeint &rhs) { return implement; } \
NOT_HUGEINT_TEMP friend inline bool operator op_name (const hugeint &lhs, const NotHugeint &rhs) { return lhs op_name (hugeint)rhs; } \
NOT_HUGEINT_TEMP friend inline bool operator op_name (const NotHugeint &lhs, const hugeint &rhs) { return (hugeint)lhs op_name rhs; }

#define OPERATION(op_name, implement) \
friend inline hugeint operator op_name (const hugeint &lhs, const hugeint &rhs) { return implement; } \
NOT_HUGEINT_TEMP friend inline hugeint operator op_name (const hugeint &lhs, const NotHugeint &rhs) { return lhs op_name (hugeint)rhs; } \
NOT_HUGEINT_TEMP friend inline hugeint operator op_name (const NotHugeint &lhs, const hugeint &rhs) { return (hugeint)lhs op_name rhs; }

#define ASSIGN(op_name, implement) \
friend inline hugeint &operator op_name (hugeint &lhs, const hugeint &rhs) { implement; return lhs; } \
NOT_HUGEINT_TEMP friend inline hugeint &operator op_name (hugeint &lhs, const NotHugeint &rhs) { return lhs op_name (hugeint)rhs; } \
NOT_HUGEINT_TEMP friend inline NotHugeint &operator op_name (NotHugeint &lhs, const hugeint &rhs) { return lhs op_name (NotHugeint)rhs; }

	class hugeint {
	private:
		bool neg = false;
		std::vector <digit_t> digits;

		// ----- Casting functions -----
	private:
		size_t fromHex (const std::string::const_iterator &begin, const std::string::const_iterator &end);
		size_t fromDec (const std::string::const_iterator &begin, const std::string::const_iterator &end);
		size_t fromOct (const std::string::const_iterator &begin, const std::string::const_iterator &end);
		size_t fromBin (const std::string::const_iterator &begin, const std::string::const_iterator &end);

	public:
		// Reads a string beetwen [begin, end) and interprets it as a number in hexadecimal, decimal, octal or binary.
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
		explicit operator int64_t () const;
		operator uint64_t () const;
#ifdef DIGIT_64
		explicit operator __int128 () const;
		explicit operator unsigned __int128 () const;
#endif
		explicit operator float () const;
		explicit operator double () const;
		explicit operator std::string () const;

		// ----- Mathematical functions -----

		// Utilty functions
	private:
		void clearZeros ();
		void resize (size_t new_size);
		void invert ();

	public:
		size_t size () const;
		void negate ();

		// Bit manip. functions
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

		static hugeint calculateDiv (const hugeint &lhs, const hugeint &rhs);
		static hugeint calculateMod (const hugeint &lhs, const hugeint &rhs);

		void setRamdon (size_t size, bool rand_sign);

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
		friend inline hugeint operator~ (const hugeint &rhs) {
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

		INTEGER_TEMP friend inline hugeint &operator<<= (hugeint &lhs, const Integer &rhs) {
			if (rhs > 0) {
				lhs.shiftFwd(rhs);
			}
			else if (rhs < 0) {
				lhs.shiftBack(-rhs);
			}
			return lhs;
		}
		INTEGER_TEMP friend inline hugeint operator<< (const hugeint &lhs, const Integer &rhs) {
			hugeint result(lhs);
			result <<= rhs;
			return result;
		}
		INTEGER_TEMP friend inline hugeint &operator>>= (hugeint &lhs, const Integer &rhs) {
			if (rhs > 0) {
				lhs.shiftBack(rhs);
			}
			else if (rhs < 0) {
				lhs.shiftFwd(-rhs);
			}
			return lhs;
		}
		INTEGER_TEMP friend inline hugeint operator>> (const hugeint &lhs, const Integer &rhs) {
			hugeint result(lhs);
			result >>= rhs;
			return result;
		}

		// Addition and subtration operators
		friend inline hugeint &operator++ (hugeint &rhs) {
			rhs.increment();
			return rhs;
		}
		friend inline const hugeint operator++ (hugeint &lhs, int) {
			hugeint result(lhs);
			lhs.increment();
			return result;
		}
		friend inline hugeint &operator-- (hugeint &rhs) {
			rhs.decrement();
			return rhs;
		}
		friend inline const hugeint operator-- (hugeint &lhs, int) {
			hugeint result(lhs);
			lhs.decrement();
			return result;
		}

		friend inline hugeint operator+ (const hugeint &lhs) {
			return lhs;
		}
		friend inline hugeint operator- (const hugeint &lhs) {
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
		hugeint pow (exp_t exponent, const hugeint &modulo) {
			calculatePow(exponent, modulo);
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
			*this = calculateMult(*this, other);
			*this = calculateDiv(*this, gcd);
			return *this;
		}

		hugeint sqrt () {
			calculateNthRoot(2);
			return *this;
		}
		hugeint cbrt () {
			calculateNthRoot(3);
			return *this;
		}

		hugeint nthroot (root_t degree) {
			calculateNthRoot(degree);
			return *this;
		}
	};

	inline hugeint pow (hugeint base, exp_t exponent) {
		base.pow(exponent);
		return base;
	}
	inline hugeint pow (hugeint base, exp_t exponent, const hugeint &modulo) {
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
	inline hugeint nthroot (const hugeint &num, root_t degree) {
		hugeint ret = num;
		return ret.nthroot(degree);
	}

#undef NOT_HUGEINT_TEMP
}

inline std::ostream &operator<< (std::ostream &out, const huge::hugeint &to_show) {
	out << (std::string)to_show;
	return out;
}
inline std::ostream &operator<< (std::ostream &out, huge::hugeint &&to_show) {
	out << (std::string)to_show;
	return out;
}
inline std::istream &operator>> (std::istream &in, huge::hugeint &to_set) {
	std::string input_string;
	in >> input_string;
	to_set.fromString(input_string.begin(), input_string.end());
	return in;
}