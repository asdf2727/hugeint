# TODO

Get ideas form other implementations like this one: https://faheel.github.io/BigInt/

### examples:

- [x] add ^ to the example calulator

### features:

- [ ] include parser in a separate class or in the same class
- [ ] add factorial

### faster algorithms:

- general optimizations
	- copy even if value is const reference
	- use simd when possible (search intel intrinsics guide)
	- add Rvalue to operators
- decimal to hex and inverse casting
	- divide and conquer (does it even work?)
- division
	- look over these:
		- https://en.wikipedia.org/wiki/Division_algorithm#Newton%E2%80%93Raphson_division
		- https://en.wikipedia.org/wiki/Barrett_reduction
		- https://en.wikipedia.org/wiki/Montgomery_modular_multiplication
		- https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.47.565&rep=rep1&type=pdf
- multiplication
	- if you have any sanity left try Schonhage-Strassen: https://en.wikipedia.org/wiki/Sch%C3%B6nhage%E2%80%93Strassen_algorithm

### to test: