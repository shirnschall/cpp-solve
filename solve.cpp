#include "solve.h"
#include "vector.h"

#include <equation.h>

#include <math.h>

char* reverseString(const char* string,char length){
    auto tmp = (char*)malloc((length+1)*sizeof(char));
    if(!tmp)
        return nullptr;

    tmp[length]='\0';
    for (int i = 0; i < length; ++i) {
        tmp[i]=string[length-1-i];
    }
    return tmp; //make sure to free the returned pointer
}

float solve(const Equation* eq,char start,char end,const float* vars){
    //check if eq is valid
    //if not, return NaN
    if(end<=start)
        return NAN;

    //create new arrays to store numbers and operators
    Vector<float> numbers;
    Vector<char> plusIndex;
    Vector<char> multIndex;
    Vector<char> powIndex;

    char tmp[MAX_NUMBER_LENGTH];
    if(!tmp)
        return NAN;
    char tmpc=0;
    char* reversed;

    //parse string eq
    //parsing from right to left allows us to
    //e.g. multiply the number to the right of binary operators with -1 for subtraction
    for (char i = end-1; i >= start; --i) {
        //check if current char is number or decimalpoint '.'
        if((eq->at(i)->i == 2 && (eq->at(i)->j == 1 ||eq->at(i)->j == 2 ||eq->at(i)->j == 3)) ||
                (eq->at(i)->i == 3 && (eq->at(i)->j == 1 ||eq->at(i)->j == 2 ||eq->at(i)->j == 3)) ||
                (eq->at(i)->i == 4 && (eq->at(i)->j == 1 ||eq->at(i)->j == 2 ||eq->at(i)->j == 3)) ||
                (eq->at(i)->i == 5 && (eq->at(i)->j == 1 ||eq->at(i)->j == 2 )))
        {
            if(tmpc>=MAX_NUMBER_LENGTH)
                return NAN;

            tmp[tmpc++]=eq->at(i)->text[0];
        }
        //if current char is + or - add tmp to numbers array if it is not empty
        //if it is empty: ignore if current char is '+' and mult the last number in numbers with -1 if it is '-'.
        //->do not add to plusIndex array
        else if(eq->at(i)->i == 4 && eq->at(i)->j == 4) // +
        {
            if(tmpc>0)
            {
                reversed=reverseString(tmp,tmpc);
                if(!reversed)
                    return NAN;
                plusIndex.push(numbers.push(strtod(reversed,nullptr)));
                free(reversed);
                tmpc=0;
            }
            //handling wrong or weird inputs
            else if(i==end-1)
            {
                return NAN;
            }
            //these two extra cases are necessary because a calculation like a++--+b is valid and equal to a+b
            else if(plusIndex.size() == 0 || (plusIndex.size() > 0 && numbers.size() != *plusIndex.at(
                    plusIndex.size() - 1))){
                plusIndex.push(numbers.size());
            }
        }
        else if(eq->at(i)->i == 3 && eq->at(i)->j == 4) // -
        {
            if(tmpc>0)
            {
                reversed=reverseString(tmp,tmpc);
                if(!reversed)
                    return NAN;
                plusIndex.push(numbers.push(-strtod(reversed,nullptr)));
                free(reversed);
                tmpc=0;
            }
            //handling wrong or weird inputs
            else if(i==end-1)
            {
                return NAN;
            }
            //these two extra cases are necessary because a calculation like a++--+b is valid and equal to a+b
            else if(plusIndex.size() == 0 || (plusIndex.size() > 0 && numbers.size()!= *plusIndex.at(
                    plusIndex.size() - 1))){
                *numbers.at(numbers.size() - 1)*=-1;
                plusIndex.push(numbers.size());
            }else{
                *numbers.at(numbers.size() - 1)*=-1;
            }
        }
        //check for multiplication
        else if(eq->at(i)->i == 2 && eq->at(i)->j == 4){ //*
            if(tmpc>0)
            {
                reversed=reverseString(tmp,tmpc);
                if(!reversed)
                    return NAN;
                multIndex.push(numbers.push(strtod(reversed,nullptr)));
                free(reversed);
                tmpc=0;
            }else if(i==end-1 || i==start)
            {
                return NAN;
            }
            //this case is for a*-b. because - is pushed into the plusIndex array we need to remove it.
            else if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()){
                plusIndex.pop();
                multIndex.push(numbers.size());
            }
            else{
                multIndex.push(numbers.size());
            }
        }
        else if(eq->at(i)->i == 1 && eq->at(i)->j == 4){ // /
            if(tmpc>0)
            {
                reversed=reverseString(tmp,tmpc);
                if(!reversed)
                    return NAN;
                multIndex.push(numbers.push((float)1/strtod(reversed,nullptr)));
                free(reversed);
                tmpc=0;
            }else if(i==end-1 || i==start)
            {
                return NAN;
            }
            //this case is for a/-b. because - is pushed into the plusIndex array we need to remove it.
            else if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()){
                plusIndex.pop();
                (*numbers.at(numbers.size() - 1)) = 1 / (*numbers.at(numbers.size() - 1));
                multIndex.push(numbers.size());
            }else{
                (*numbers.at(numbers.size() - 1)) = 1 / (*numbers.at(numbers.size() - 1));
                multIndex.push(numbers.size());
            }
        }else if(eq->at(i)->i == 1 && eq->at(i)->j == 1 && eq->at(i)->z == 0){ // ^
            if(tmpc>0)
            {
                reversed=reverseString(tmp,tmpc);
                if(!reversed)
                    return NAN;
                multIndex.push(numbers.push(strtod(reversed,nullptr)));
                powIndex.push(numbers.size());
                free(reversed);
                tmpc=0;
            }else if(i==end-1 || i==start)
            {
                return NAN;
            }//this case is for a/-b. because - is pushed into the plusIndex array we need to remove it.
            else if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()) {
                plusIndex.pop();
                multIndex.push(numbers.size());
                powIndex.push(numbers.size());
            }
            else{
                multIndex.push(numbers.size());
                powIndex.push(numbers.size());
            }

        }
        //if we find a closing bracket, try to find a matching opening bracket and call solve recursively
        //here we have to count all operators like "sin(" as a open bracket..
        else if(eq->at(i)->i == 1 && eq->at(i)->j == 3){ // )
            //try to find a matching '(':
            char numClosingBrackets=0;
            char foundMatching=0;
            for(char j=i-1;j>=start;--j){
                if(eq->at(j)->i == 1 && eq->at(j)->j == 3)      // )
                    ++numClosingBrackets;
                else if(((eq->at(j)->i == 1 && eq->at(j)->j == 2) ||    // (
                         (eq->at(j)->i == 0 && eq->at(j)->j == 0) ||    //sin,asin,...
                         (eq->at(j)->i == 0 && eq->at(j)->j == 1) ||    //cos,acos,...
                         (eq->at(j)->i == 0 && eq->at(j)->j == 2) ||    //tan,atan,...
                         (eq->at(j)->i == 2 && eq->at(j)->j == 0) ||    //ln,log
                         (eq->at(j)->i == 1 && eq->at(j)->j == 1 && eq->at(j)->z == 1))   //sqrt
                            && numClosingBrackets>0)
                    --numClosingBrackets;
                else if(((eq->at(j)->i == 1 && eq->at(j)->j == 2) ||    // (
                         (eq->at(j)->i == 0 && eq->at(j)->j == 0) ||    //sin,asin,...
                         (eq->at(j)->i == 0 && eq->at(j)->j == 1) ||    //cos,acos,...
                         (eq->at(j)->i == 0 && eq->at(j)->j == 2) ||    //tan,atan,...
                         (eq->at(j)->i == 2 && eq->at(j)->j == 0) ||    //ln,log
                         (eq->at(j)->i == 1 && eq->at(j)->j == 1 && eq->at(j)->z == 1))   //sqrt
                         && numClosingBrackets==0)    // (,sin,...
                {
                    //matching '(' found
                    if(!foundMatching) {
                        numbers.push(solve(eq, j + 1, i,vars));
                        if(eq->at(j)->i == 1 && eq->at(j)->j == 2) // (
                            i = j;//skip the part between () in parsing
                        else    //sin,cos,...
                            i = j+1;//if sin etc is ( we do not want to skip the operator -> j+1
                        foundMatching = 1;
                    }
                }
            }
            if(!foundMatching)
                return NAN;
        }
        else{
            //unary operators:
            //trig functions work with rad not deg!
            if(eq->at(i)->i == 0 && eq->at(i)->j == 0 && eq->at(i)->z == 1){    // asin
                if(numbers.size())
                    *numbers.at(numbers.size()-1) = asin(*numbers.at(numbers.size()-1));
                if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()) {
                    plusIndex.pop();
                }
            }
            else if(eq->at(i)->i == 0 && eq->at(i)->j == 0 && eq->at(i)->z == 0){   // sin
                if(numbers.size())
                    *numbers.at(numbers.size()-1) = sin(*numbers.at(numbers.size()-1));
                if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()) {
                    plusIndex.pop();
                }
            }else if(eq->at(i)->i == 0 && eq->at(i)->j == 1 && eq->at(i)->z == 1){  //acos
                if(numbers.size())
                    *numbers.at(numbers.size()-1) = acos(*numbers.at(numbers.size()-1));
                if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()) {
                    plusIndex.pop();
                }
            }else if(eq->at(i)->i == 0 && eq->at(i)->j == 1 && eq->at(i)->z == 0){  //cos
                if(numbers.size())
                    *numbers.at(numbers.size()-1) = cos(*numbers.at(numbers.size()-1));
                if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()) {
                    plusIndex.pop();
                }
            }else if(eq->at(i)->i == 0 && eq->at(i)->j == 2 && eq->at(i)->z == 1){  //atan
                if(numbers.size())
                    *numbers.at(numbers.size()-1) = atan(*numbers.at(numbers.size()-1));
                if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()) {
                    plusIndex.pop();
                }
            }else if(eq->at(i)->i == 0 && eq->at(i)->j == 2 && eq->at(i)->z == 0){  //tan
                if(numbers.size())
                    *numbers.at(numbers.size()-1) = tan(*numbers.at(numbers.size()-1));
                if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()) {
                    plusIndex.pop();
                }
            }else if(eq->at(i)->i == 2 && eq->at(i)->j == 0 && eq->at(i)->z == 0){  //ln
                if(numbers.size())
                    *numbers.at(numbers.size()-1) = log(*numbers.at(numbers.size()-1));
                if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()) {
                    plusIndex.pop();
                }
            }else if(eq->at(i)->i == 2 && eq->at(i)->j == 0 && eq->at(i)->z == 1){  //log
                if(numbers.size())
                    *numbers.at(numbers.size()-1) = log10(*numbers.at(numbers.size()-1));
                if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()) {
                    plusIndex.pop();
                }
            }


            //constants
            else if(eq->at(i)->i == 3 && eq->at(i)->j == 0 && eq->at(i)->z == 0){   //pi
                numbers.push(M_PI);
            }else if(eq->at(i)->i == 3 && eq->at(i)->j == 0 && eq->at(i)->z == 1){   //e
                numbers.push(M_E);
            }else if(eq->at(i)->i == 5 && eq->at(i)->j == 0 && eq->at(i)->z == 0){    //ans
                if(vars)
                    numbers.push(vars[0]);
                else
                    numbers.push(NAN);
            }
            else
                return NAN;
        }
    }

    //push to numbers array one last time
    if(tmpc>0)
    {
        reversed=reverseString(tmp,tmpc);
        if(!reversed)
            return NAN;
        numbers.push(strtod(reversed,nullptr));
        free(reversed);
        tmpc=0;
    }

    //parsing eq is done.
    //start computing result:
    //rules:
    //brackets first (already done in parsing)
    //mult and div before add and sub
    //equal priority operations from left to right
    //we do not need to worry about the last point as we are replacing all divisions (a/b) with a*(1/b)
    //and all subtractions (a+b) with (a+(-b))
    //+ and * are commutative.

    if(numbers.size()==0)
        return NAN;

    //we start with power a^b
    if(powIndex.size() > 0) {
        for (char i = powIndex.size()-1;i>=0; --i){
            //check if '*' is associated with two numbers:
            if(*powIndex.at(i)>= numbers.size()|| *powIndex.at(i) == 0)
                return NAN;
            (*numbers.at(*powIndex.at(i)-1)) = pow((*numbers.at(*powIndex.at(i))),(*numbers.at(*powIndex.at(i)-1)));
            (*numbers.at(*powIndex.at(i))) = 1;
        }
    }

    //as we parsed eq from right to left, we have to go through the arrays from right to left
    //in to do calculations from left to right :)

    //because * is commutative we calculate the result for the multiplication from right to left
    //this makes it easier as we can store the result of a*b in a
    //e.g. a*(b*c)*d:
    //first the () was done in the parsing step so it is now a*e*d
    // we compute e*d=:f and replace e with the result f
    //this results in a*f*d. however we will ignore d and calculate a*f and replace a with the result.
    if(multIndex.size() > 0) {
        for (char i = multIndex.size()-1;i>=0  ; --i){
            //check if '*' is associated with two numbers:
            if(*multIndex.at(i)>= numbers.size() || *multIndex.at(i) == 0)
                return NAN;
            (*numbers.at(*multIndex.at(i)-1)) *= (*numbers.at(*multIndex.at(i)));
        }
    }

    //we can now add the remaining numbers and return the result
    //Take a look at: a+b*c+d
    //based on the previous step we know that the result of b*c will be stored in b
    //we have to ignore c. this can be done using the value stored in the next plusIndex.
    //we sum the first number and all numbers that are to the right of a + symbol.
    //these numbers have an index of *plusIndex.at()-1

    float result=*numbers.at(0);
    if(numbers.size()>1) {  //if numbers.size == 1 we have a leading +-. we ignore it.
        for (char i = 0; i < plusIndex.size(); ++i) {
            result += *numbers.at(*plusIndex.at(i));
        }
    }

    return result;
}
