# hugeint

A C++ class that adds arbitrary precision integers aimed at high performance and ease of use. All you need to do is replace int with hugeint and the rest is done for you!

## Contents

- [Capabilities](#capabilities)
	- [Features](#features)
	- [Ease of Use](#ease-of-use)
	- [Efficiency](#efficiency)
- [Usage](#usage)
	- [How to use it in your project](#how-to-use-it-in-your-project)
		- [Example](#example)
	- [Input and Output](#input-and-output)
		- [Iostream Compatibility](#iostream-compatibility)
		- [Constructors and Assignments](#constructors-and-assignments)
		- [String Assignment](#string-assignment)
		- [Casting](#casting)
	- [Operators](#operators)
		- [Comparison Operators](#comparison-operators)
		- [Boolean Logic](#boolean-logic)
		- [Incrementing and Decrementing](#incrementing-and-decrementing)
		- [Addition and Subtraction](#adition-and-subtraction)
		- [Multiplication, Division and Modulus](#multiplication-division-and-modulus)
	- [Functions](#functions)
		- [String Functions](#string-functions)
		- [Base Functions](#base-functions)
		- [Bit Functions](#bit-functions)
		- [Mathematical Functions](#mathematical-functions)
			- [Absolute Value](#absolute-value)
			- [Random Number](#random-number)
			- [Greatest Common Divisor](#greatest-common-divisor)
			- [Least Common Multiple](#least-common-multiple)
			- [Exponentiation](#exponentiation)
			- [Roots](#roots)
- [Work in Progress](#work-in-progress)

# Capabilities

### Features

- bitwise operators
- automatic base detection for input and multiple base options for output
- extra functions such as gcd and sqrt
- string parser of arithmetic operations present as an example

### Ease of Use

- simple integration in your project
- complete with base types such as integers, floating points or strings

### Efficiency

- unlike other public implementations, numbers are stored in base 2^32 via two's complement for speed of computation
- quick computation until 100000s of digits, but no hard limit exists.
- small asymptotic complexity algorithms such as Karatsuba's algorithm

# Usage

## How to use it in your project

Add ```hugeint_math.cpp```, ```hugeint_cast.cpp``` and ```hugeint.h``` to your project and include the header file
where you use the class.

After that, declare a variable with ```hugeint my_val;``` and you're good to go!

### Example

The example present in the repository is ```arithmetic.cpp```, an extensive parser using hugeint, capable of ```+```, ```-```, ```*```, ```/```, ```%```, ```^```, parantheses and order of operations. It also comes with some functions such as ```sqrt()``` and ```gcd()```.

This is how to create a new project, using this example:
1. Download all .cpp and .h files and put them in a folder.
2. Run ```g++ arithmetic.cpp hugeint_math.cpp hugeint_cast.cpp hugeint.h -o arithmetic``` in the folder if you use g++, or the equivalent command in any other compiler of your choosing.
3. Now there shoud be an executable in the same folder which you can run. The program waits until a line was written then outputs the result in 4 bases.

## Input and Output

hugeint is well integrated with the base types of C++, having many options of reading and returning data.

### Iostream Compatibility

you can read and print hugeints via istream and ostream objects, similar to any int.

```c++
hugeint num;
std::cin >> num;
std::cout << num * 2;
```

### Constructors and Assignments

Here are all the ways hugeints can be initialised. All assignments work with the same types.

```c++
hugeint num;             // default constructor. Default value is 0.
hugeint num2(num);       // const reference constructor/assignment.
hugeint num3(sqrt(num)); // rvalue reference constructor/assignment.
hugeint num4(true);      // bool constructor/assignment. False is 0 and true is 1.
hugeint num5(-5);        // integer constructor/assignment. Works with all integer types,including shorts and long longs, signed or unsigned.
hugeint num5(4.2e10);    // floating point constructor/assignment. Works with floats anddoubles.
hugeint num6("-345");    // string constructor/assignment. Works with char* and std::string.
  
num2 = num;              // Assignments can be done with the same types as constructors
num3 = sqrt(num);
num4 = true;
num5 = -5;
num4 = 4.2e10;
num6 = "-345";
```

### String Assignment

This is used in constructors, assignments and reading with istream.

The conversion between string and hugeint can be done in hexadecimal, decimal, octal or binary. The base is determined automaticly when converting.

>- hexadecimal notation: ```0x2df```, ```x2df```, ```2dfx```.
>- decimal notation: ```735```, ```0d735```, ```d735```, ```735d```.
>- octal notation: ```01337```, ```0o1337```, ```o1337```, ```1337o```.
>- binary notation: ```0b1337```, ```b1337```, ```1337b```.

All letters can be lower or upper case, including the numbers for hexadecimal notation.

Spaces (``` ```) or apostrophes (```'```) are allowed at any point inside the number, exluding prefixes or suffixes.

Any number of `+` and `-` symbols can be present, as long as they are at the beginning of the string. For example, ```-x2d9``` is allowed, but ```x-2d9``` is not.

**Warning:** leading zeros are allowed, but if the first character after the sign is a zero and the next is a decimal digit, octal base is assumed.

The functions related to string handling are explained [here](#string-functions).

### Casting

Casting works with the same types as assignment:

```c++
hugeint num = "14367289165784932";

bool val = (bool)num;               // false if 0, true otherwise.
int val2 = (int)num;                // works with any integer type
float val2 = (float)num;            // also works with double
std::string val3 = (std::string)num // also works with char*
```

## Operators

Hugeint includes all operators used by int, each of them made to work with itself or other types.

- ### Comparison Operators

	They include ```==```, ```!=```, ```<```, ```>```, ```<=``` and ```>=```. All of them work as expected.

	```c++
	if (pow(3, 100) < 10e50) {
		std::cout << "Smaller";
	}
	// output: Smaller
	```

- ### Boolean Logic

	I also added ```&&``` and ```||```, but they work by first casting both numbers to bool and then doing the operation.

	```c++
	hugeint num1 = "543782659", num2 = "0";
	std::cout << (num1 && num2 ? "True" : "False");
	// output: False
	```

- ### Bitwise Operators

	They include ```~```, ```&```, ```&=```, ```|```, ```|=```, ```^```, ```^=```, ```<<```, ```<<=```, ```>>``` and ```>>=```. All of them work as expected.

	```c++
	hugeint num1 = "0x8", num2 = 15;
	std::cout << ((~num1) & num2);
	// output: 7
	```

 - ### Incrementing and Decrementing

	It includes ```++``` and ```--```, so both pre- and post- increment or decrement. All 4 work as expected.

	```c++
	hugeint num = 3;
	std::cout << ++num;
	// output: 4
	```

- ### Adition and Subtraction

	It includes ```+```, ```+=```, ```-```, ```-=``` and the unary versions of ```+``` and ```-```. All of them work as expected. Unary ```+``` does nothing and unary ```-``` negates the number.

	```c++
	hugeint num1 = 3, num2 = 4;
	std::cout << num1 - num2;
	// output: -1
	```

- ### Multiplication, Division and Modulus

	It includes ```*```, ```*=```, ```/```, ```/=```, ```%``` and ```%=```. All of them work as expected.

	**note:** multiplication switches automaticly bewteen simple multiplication and Karatsuba's algorithm of, taking the one that's faster.

	```c++
	hugeint num1 = 3, num2 = 4, num3 = 7;
	std::cout << (num1 * num2) % num3;
	// output: 5
	```

## Functions

The library also includes some functions for ease of use, with more coming in the future. If you have any requests, you can find my mail in the bio of my profile.

- ### String Methods

	- ```size_t fromString (std::string::const_iterator begin, std::string::const_iterator end)``` - the same as an assignment of std::string

	If you want to output in a base another than decimal, use one of these funtions:

	- ```std::string toHex ()``` - create a string in hexadecimal. Includes ```0x``` as a prefix.
	- ```std::string toDec ()``` - create a string in decimal.
	- ```std::string toOct ()``` - create a string in octal. Includes ```o``` as a prefix.
	- ```std::string toBin ()``` - create a string in octal. Includes ```b``` as a suffix.

	```c++
	hugeint num;
	std::string str = "456";
	num.fromString(str.begin(), str.end());
	std::cout << num.toHex();
	//output: 0x1c8
	```

- ### Base Methods
	- ```ullint size ()``` - returns the size of the number in bits.
	- ```void negate ()``` - the same as unary ```-```. Negates the number.

	```c++
	hugeint num = "7";
	num.negate();
	std::cout << num.size() << ' ' << num;
	//output: 3 -7
	```

- ### Bit Methods

	A more direct way of modifying and reading bits of the number. All functions allow positions outside written values (i.e. leading zeros), but due to two's complement negative numbers have leading ones. The position represents the bit index (e.g. 1000b is the 3rd bit).

	- ```bool getBit (size_t pos)``` - returns the value of bit pos.
	- ```void flipBit (size_t pos)``` - flips the bit at position pos.
	- ```void setBit (size_t pos, bool val)``` - sets the bit at position pos with the value val.

- ### Mathematical Functions

	These functions add extra functionality to the class. They can be called in two ways, either as member functions or as extrnal functions.
	
	**ALL** member functions modify the instance on which they are called and return the modified value.
	
	**ALL** External functions create and return new instances.

	**note:** All external functions are declared in the 'huge' namespace to avoid ambiguity bewteen std functions and hugeint custom ones.

	- #### Absolute Value

		Computes the absolute value of a hugeint.

		- ```hugeint abs ()``` - member function. Turns 'this' into it's absolute value.
		- ```hugeint huge::abs (hugeint to_abs)``` - external function. Returns to_abs in absolute value.

	- #### Random Number

		Computes a random hugeint. The sign is randomised if rand_sign is true, otherwise it is positive.
		
		- ```hugeint rand (size_t size, bool rand_sign)``` - member function. Turns 'this' into the random value.
		- ```hugeint huge::rand (size_t size, bool rand_sign)``` - external function. Returns the random value.

	- #### Greatest Common Divisor

		Computes the greatest common divisor between 2 hugeints.

		- ```hugeint gcd (const hugeint &other)``` - member function. Turns 'this' into the gcd between it and 'other'.
		- ```hugeint huge::gcd (const hugeint &num1, const hugeint &num2)``` - external function. returns the gcd between num1 and num2.

	- #### Least Common Multiple

		Computes the least common multiple between 2 hugeints.

		- ```hugeint lcm (const hugeint &other)``` - member function. Turns 'this' into the lcm between itself and 'other'.
		- ```hugeint huge::lcm (const hugeint &num1, const hugeint &num2)``` - external function. returns the lcm between num1 and num2.

	- #### Exponentiation

		Computes a hugeint raised to an exponent, with or without modulo.

		- ```template <typename T> hugeint pow (T exponent)``` - member function. raises 'this' to the exponent.
		- ```template <typename T> hugeint huge::pow (hugeint base, T exponent)``` - external function. returns base ^ exponent.
		- ```template <typename T> hugeint pow (T exponent, const hugeint &modulus)``` - member function. raises 'this' to the exponent, then aplies the modulo.
		- ```template <typename T> hugeint huge::pow (hugeint base, T exponent, const hugeint &modulus)``` - external funtion. reutrns (base ^ exponent) % modulo.

	- #### Roots

		Returns the floor of the root of degree 2, 3 or n.

		- ```hugeint sqrt ()``` - member function. Turns 'this' into the floor of the square root.
		- ```hugeint huge::sqrt (const hugeint &num)``` - external function. Returns the floor of the square root of num.
		- ```hugeint cbrt ()``` - member function. Turns 'this' into the floor of the cube root.
		- ```hugeint huge::cbrt (const hugeint &num)``` - external function. Returns the floor of the cube root of num.
		- ```hugeint nthroot (T degree)``` - member function. Turns 'this' into the floor of the n-th degree root. T can be any type that casts to uint64_t.
		- ```hugeint huge::nthroot (const hugeint &num, T degree)``` - external function. Returns the floor of the n-th degree root of num. T can be any type that casts to uint64_t.

# Work in progress

This is still a work in progress and I'm not sure I'll ever consider it fully completed, so I'm open to requests and
suggestions. If you find a bug, please open an issue on github. If you otherwise wish to contact me, you can find my mail in the bio of my profile.

I hope you'll enjoy working with it as much as I did making it. Thank you for using this library and good luck on your project!