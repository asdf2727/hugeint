# TODO

Get ideas form other implementations like this one: https://faheel.github.io/BigInt/

### examples:

- [x] add ^ to the example calulator

### features:

- [x] add ' inside number reading
- [x] add index for broken character for "fromString"
- [x] add parsing for string conversion to allow for things like 0x..., b..., x..., etc.
- [x] implement nth root
- [x] implement abs
- [x] implement gcd and lcm
- [x] implement random
- [ ] include parser in a separate class or in the same class
- [x] add float and double to casting
- [x] add float and double to constructors
- [x] add throw to div if you divide by 0
- [ ] add factorial

### faster algorithms:

- general optimizations
	- [x] implement 64 bit numbers
	- [x] use std::vector cuz it's faster
	- [x] negate values instead of creating copy even if value is const reference
	- [ ] use simd when possible (search intel intrinsics guide)
	- [x] avoid min and max in loop conditions
	- [ ] add Rvalue to operators
- decimal to hex and inverse casting
	- [ ] divide and conquer (does it even work?)
- division
	- [ ] look over these:
		- https://en.wikipedia.org/wiki/Division_algorithm#Newton%E2%80%93Raphson_division
		- https://en.wikipedia.org/wiki/Barrett_reduction
		- https://en.wikipedia.org/wiki/Montgomery_modular_multiplication
		- https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.47.565&rep=rep1&type=pdf
- multiplication
	- [x] add simple multiplication for small * big
	- [ ] rewrite karatsuba to use pointers instead of mem copy (alloc the least possible)
	- [ ] if you have any sanity left try Schonhage-Strassen: https://en.wikipedia.org/wiki/Sch%C3%B6nhage%E2%80%93Strassen_algorithm

### to test:

- [ ] example (all)
- [ ] multiplication
- [ ] simple multiplication