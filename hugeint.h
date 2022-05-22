#pragma once

#include <iostream> //overload of << and >>
#include <string>
#include <deque>
#include <cmath>
#include <cstring> // memset
#include <type_traits> // enable_if

#define HUGEINT_DECIMAL_OUTPUT

#define NOT_HUGEINT_TEMP template <typename NotHugeint, typename Dummy = std::enable_if <!std::is_same <NotHugeint, hugeint>::value, bool>>
#define INTEGER_TEMP template <typename Integer, typename Dummy = std::enable_if <std::is_integral <Integer>::value, bool>>

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

	// Aditional functions
public:
	std::string toHex () const;
	std::string toString () const;

	std::size_t size() const;

	hugeint &pow(ullint exponent);
	friend hugeint pow(hugeint base, ullint exponent);

	// Casting functions
private:
	void fromString (const std::string &to_copy);

public:
	hugeint ();
	hugeint (const hugeint &to_copy);
	hugeint (hugeint &&to_copy) noexcept;
	hugeint (const bool &to_copy);
	hugeint (const sint &to_copy);
	hugeint (const int &to_copy);
	hugeint (const llint &to_copy);
	hugeint (const usint &to_copy);
	hugeint (const uint &to_copy);
	hugeint (const ullint &to_copy);
	hugeint (const char *to_copy);
	hugeint (const std::string &to_copy);

	explicit operator bool () const;
	explicit operator short int () const;
	explicit operator int () const;
	explicit operator long long int () const;
	explicit operator unsigned short int () const;
	explicit operator unsigned int () const;
	explicit operator unsigned long long int () const;
	explicit operator const char * () const;
	explicit operator std::string () const;

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

	friend std::ostream &operator<< (std::ostream &out, const hugeint &to_show);
	friend std::istream &operator>> (std::istream &in, hugeint &to_set);

	// Mathematical functions
private:
	void resize (std::size_t new_size);
	void clearZeros ();
	void invert ();
	void negate ();

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

	bool addDeque (std::deque <uint> &nr1, const std::deque <uint> &nr2, bool addlast);
	void decDeque (std::deque <uint> &nr1, const std::deque <uint> &nr2);
	std::deque <uint> karatsuba (std::deque <uint> half11, std::deque <uint> half21, std::size_t tot_size);
	hugeint &calculateMult (hugeint to_mult);

	uint divBinSearch (hugeint &rest, const hugeint &to_div);
	hugeint &calculateDiv (hugeint to_div);
	hugeint &calculateMod (hugeint to_div);

public:
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

	friend hugeint operator- (const hugeint &lhs){
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
};

#undef INTEGER_TEMP
#undef NOT_HUGEINT_TEMP