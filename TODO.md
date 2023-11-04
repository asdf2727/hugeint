# TODO

Get ideas form other implementations like this one: https://faheel.github.io/BigInt/

### examples:

- [x] add ^ to the example calulator

### features:

- [x] repair octal output
- [x] redo example
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
- [x] add rbegin instead of wierd for from n-1 to overflow
- [ ] add factorial

### faster algorithms:

- general hardware optimizations
    - [ ] implement 64 bit numbers
    - use std::vector cuz it's faster
- adition and subtraction
    - [ ] do a separate function for hugeint + ullint
- decimal to hex and inverse casting
    - [ ] divide and conquer
- division
    - [ ] see if there even is an algorithm
- multiplication
    - [ ] do a separate function for hugeint * int
    - [x] add simple multiplication for small * big
    - [ ] if you have any sanity left try
      Schonhage-Strassen: https://en.wikipedia.org/wiki/Sch%C3%B6nhage%E2%80%93Strassen_algorithm

### to test:

- [ ] example (all)
- [ ] multiplication
- [ ] simple multiplication