#ifndef SOLVE_SOLVE_H
#define SOLVE_SOLVE_H

#include <equation.h>

#include <math.h>



float solve(const Equation* eq,char start,char end,const float* vars = nullptr);

char* reverseString(const char* string,char length);


#endif //SOLVE_SOLVE_H
