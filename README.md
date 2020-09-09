# cpp-solve
![GitHub release](https://img.shields.io/badge/release-v0.1-blue.svg)

Lightweight library to compute the result of a calculation given as an input string.
You can find a detailed explanaition of the code at [my Blog](https://blog.hirnschall.net/programming-an-advanced-cpp-calculator/)


### Order of operations:
1. Brackets first
2. Exponentiation and root extraction
3. Multiplication and division
4. Addition and subtraction
5. Same priority operations are calculated from left to right


### Build
include solve.h in your project and call       
```float solve(const char* eq,char start, char end)```.       
Internally solve.h uses vector.h and vector.cpp so make sure to add them to your project!
As the auto keyword is used you'll have to use C++11 or newer (or replace the auto keyword with the correct type).


### Usage
include solve.h in your project and call       
```float solve(const char* eq,char start, char end)```.  
start is the index of the first char that is part of your calculation and end is the first char index that is no longer part of your calculation.     
E.g. if you want to solve "3*5+(2-3)*7" call:       
```solve("3*5+(2-3)*7",0, 11)```.       

In general: If the whole string is part of your calculation, start=0 and end=(length of your string)

### Features
- [x] Multiplication and division
- [x] Addition and subtraction
- [x] Brackets
- [x] Correct order of operations (not like windows calc.exe o.O)
- [x] Function (sin,cos,etc.)
- [x] Power (a^b)
- [x] Constants (pi,e, etc.)
- [x] variables (ans)
