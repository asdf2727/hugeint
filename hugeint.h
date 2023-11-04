#pragma once

#include <iostream> //overload of << and >>
#include <string>
#include <vector>
#include <type_traits> // enable_if

namespace huge {

#define NOT_HUGEINT_TEMP template <typename NotHugeint, typename = std::enable_if <!std::is_same <NotHugeint, hugeint>::value>>

	typedef uint64_t size_t;
	typedef uint64_t shift_t;
	typedef uint64_t exp_t;
	typedef uint64_t root_t;

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

	class hugeint {
	private:
		bool neg = false;
		std::vector <digit_t> digits;

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
		explicit operator uint64_t () const;
#ifdef DIGIT_64
		explicit operator __int128 () const;
		explicit operator unsigned __int128 () const;
#endif
		explicit operator float () const;
		explicit operator double () const;
		explicit operator std::string () const;
		explicit operator const char * () const;

		// Mathematical functions
	private:
		void clearZeros ();
		void invert ();

		hugeint simpleMult (hugeint &num1, hugeint &num2);
		hugeint karatsuba (hugeint &num1, hugeint &num2, size_t tot_size);
		inline hugeint doMultAlgorithm (hugeint &num1, hugeint &num2, size_t tot_size);

		bool compareSml (const hugeint &to_comp) const;

		void calculateAnd (const hugeint &to_and);
		void calculateOr (const hugeint &to_or);
		void calculateXor (const hugeint &to_xor);

		void shiftFwd (shift_t val);
		void shiftBack (shift_t val);

		void increment ();
		void decrement ();

		void calculateAdd (const hugeint &to_and);
		void calculateDec (const hugeint &to_dec);

		void calculateMult (const hugeint &to_mult);

		digit_t divBinSearch (hugeint &rest, const hugeint &to_div);
		void calculateDiv (const hugeint &to_div);
		void calculateMod (const hugeint &to_div);

		void setRamdon (size_t size, bool rand_sign);

		void calculateGcd (hugeint other);

		void calculatePow (exp_t exponent);
		void calculatePow (exp_t exponent, const hugeint &to_mod);

		void calculateNthRoot (root_t degree) const;
	public:
		size_t size () const;
		void negate ();

		bool getBit (size_t pos) const;
		void flipBit (size_t pos);
		void setBit (size_t pos, bool val);

		friend bool operator== (const hugeint &lhs, const hugeint &rhs) {
			return lhs.digits == rhs.digits && lhs.neg == rhs.neg;
		}
		NOT_HUGEINT_TEMP friend bool operator== (const hugeint &lhs, const NotHugeint &rhs) {
			return lhs == (hugeint)rhs;
		}
		NOT_HUGEINT_TEMP friend bool operator== (const NotHugeint &lhs, const hugeint &rhs) {
			return (hugeint)lhs == rhs;
		}

		friend bool operator!= (const hugeint &lhs, const hugeint &rhs) {
			return lhs.digits != rhs.digits || lhs.neg != rhs.neg;
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
			return !(lhs).compareSml((hugeint)rhs);
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
			hugeint result(lhs);
			result.calculateAnd(rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator& (hugeint &lhs, const NotHugeint &rhs) {
			hugeint result(lhs);
			result.calculateAnd((hugeint)rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator& (NotHugeint &lhs, const hugeint &rhs) {
			hugeint result(lhs);
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
			hugeint result(lhs);
			result.calculateOr(rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator| (hugeint &lhs, const NotHugeint &rhs) {
			hugeint result(lhs);
			result.calculateOr((hugeint)rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator| (NotHugeint &lhs, const hugeint &rhs) {
			hugeint result(lhs);
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
			hugeint result(lhs);
			result.calculateXor(rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator^ (hugeint &lhs, const NotHugeint &rhs) {
			hugeint result(lhs);
			result.calculateXor((hugeint)rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator^ (NotHugeint &lhs, const hugeint &rhs) {
			hugeint result(lhs);
			result.calculateXor(rhs);
			return result;
		}

		friend hugeint &operator<<= (hugeint &lhs, const shift_t &rhs) {
			if (rhs > 0) {
				lhs.shiftFwd(rhs);
			}
			else if (rhs < 0) {
				lhs.shiftBack(-rhs);
			}
			return lhs;
		}
		friend hugeint operator<< (const hugeint &lhs, const shift_t &rhs) {
			hugeint result(lhs);
			if (rhs > 0) {
				result.shiftFwd(rhs);
			}
			else if (rhs < 0) {
				result.shiftBack(-rhs);
			}
			return result;
		}
		friend hugeint &operator>>= (hugeint &lhs, const shift_t &rhs) {
			if (rhs > 0) {
				lhs.shiftBack(rhs);
			}
			else if (rhs < 0) {
				lhs.shiftFwd(-rhs);
			}
			return lhs;
		}
		friend hugeint operator>> (const hugeint &lhs, const shift_t &rhs) {
			hugeint result(lhs);
			if (rhs > 0) {
				result.shiftBack(rhs);
			}
			else if (rhs < 0) {
				result.shiftFwd(-rhs);
			}
			return result;
		}

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
			hugeint result(lhs);
			result.calculateAdd(rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator+ (const hugeint &lhs, const NotHugeint &rhs) {
			hugeint result(lhs);
			result.calculateAdd((hugeint)rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator+ (const NotHugeint &lhs, const hugeint &rhs) {
			hugeint result(lhs);
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
			hugeint result(lhs);
			result.negate();
			return result;
		}

		friend hugeint operator- (const hugeint &lhs, const hugeint &rhs) {
			hugeint result(lhs);
			result.calculateDec(rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator- (const hugeint &lhs, const NotHugeint &rhs) {
			hugeint result(lhs);
			result.calculateDec((hugeint)rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend NotHugeint operator- (const NotHugeint &lhs, const hugeint &rhs) {
			hugeint result(lhs);
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
			hugeint result(lhs);
			result.calculateMult(rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator* (const hugeint &lhs, const NotHugeint &rhs) {
			hugeint result(lhs);
			result.calculateMult((hugeint)rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator* (const NotHugeint &lhs, const hugeint &rhs) {
			hugeint result(lhs);
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
			hugeint result(lhs);
			result.calculateDiv(rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator/ (const hugeint &lhs, const NotHugeint &rhs) {
			hugeint result(lhs);
			result.calculateDiv((hugeint)rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator/ (const NotHugeint &lhs, const hugeint &rhs) {
			hugeint result(lhs);
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
			hugeint result(lhs);
			result.calculateMod(rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator% (const hugeint &lhs, const NotHugeint &rhs) {
			hugeint result(lhs);
			result.calculateMod((hugeint)rhs);
			return result;
		}
		NOT_HUGEINT_TEMP friend hugeint operator% (const NotHugeint &lhs, const hugeint &rhs) {
			hugeint result(lhs);
			result.calculateMod(rhs);
			return result;
		}

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
			calculateMult(other);
			calculateDiv(gcd);
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

	hugeint pow (hugeint base, exp_t exponent) {
		base.pow(exponent);
		return base;
	}
	hugeint pow (hugeint base, exp_t exponent, const hugeint &modulo) {
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
