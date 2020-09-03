#include "solve.h"
#include "vector.h"

#include <cmath>

#include <iostream>

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

float solve(const char* eq,char start,char end, float ans){
    //check if eq is valid
    //if not, return NaN
    if(end<=start)
        return std::nanf("");

    //create new arrays to store numbers and operators
    Vector<float> numbers;
    Vector<char> plusIndex;
    Vector<char> multIndex;
    Vector<char> powIndex;

    auto tmp = (char*)malloc(MAX_NUMBER_LENGTH*sizeof(char));
    if(!tmp)
        return std::nanf("");
    char tmpc=0;
    char* reversed;

    //parse string eq
    //parsing from right to left allows us to
    //e.g. multiply the number to the right of binary operators with -1 for subtraction
    for (char i = end-1; i >= start; --i) {
        //check if current char is number or decimalpoint '.' using ascii
        if((eq[i]>47 && eq[i]<58) || eq[i]==46)
        {
            if(tmpc>=MAX_NUMBER_LENGTH)
                return std::nanf("");

            tmp[tmpc++]=eq[i];
        }
        //if current char is + or - add tmp to numbers array if it is not empty
        //if it is empty: ignore if current char is '+' and mult the last number in numbers with -1 if it is '-'.
        //->do not add to plusIndex array
        else if(eq[i] == '+')
        {
            if(tmpc>0)
            {
                reversed=reverseString(tmp,tmpc);
                if(!reversed)
                    return std::nanf("");
                plusIndex.push(numbers.push(strtof(reversed,nullptr)));
                free(reversed);
                tmpc=0;
            }
            //handling wrong or weird inputs
            else if(i==end-1)
            {
                return std::nanf("");
            }
            //these two extra cases are necessary because a calculation like a++--+b is valid and equal to a+b
            else if(plusIndex.size() == 0 || (plusIndex.size() > 0 && numbers.size() != *plusIndex.at(
                    plusIndex.size() - 1))){
                plusIndex.push(numbers.size());
            }
        }
        else if(eq[i] == '-')
        {
            if(tmpc>0)
            {
                reversed=reverseString(tmp,tmpc);
                if(!reversed)
                    return std::nanf("");
                plusIndex.push(numbers.push(-strtof(reversed,nullptr)));
                free(reversed);
                tmpc=0;
            }
            //handling wrong or weird inputs
            else if(i==end-1)
            {
                return std::nanf("");
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
        else if(eq[i]=='*'){
            if(tmpc>0)
            {
                reversed=reverseString(tmp,tmpc);
                if(!reversed)
                    return std::nanf("");
                multIndex.push(numbers.push(strtof(reversed,nullptr)));
                free(reversed);
                tmpc=0;
            }else if(i==end-1 || i==start)
            {
                return std::nanf("");
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
        else if(eq[i]=='/'){
            if(tmpc>0)
            {
                reversed=reverseString(tmp,tmpc);
                if(!reversed)
                    return std::nanf("");
                multIndex.push(numbers.push((float)1/strtof(reversed,nullptr)));
                free(reversed);
                tmpc=0;
            }else if(i==end-1 || i==start)
            {
                return std::nanf("");
            }
            //this case is for a/-b. because - is pushed into the plusIndex array we need to remove it.
            else if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()){
                std::cout<<(int)plusIndex.size()<<std::endl;
                plusIndex.pop();
                (*numbers.at(numbers.size() - 1)) = 1 / (*numbers.at(numbers.size() - 1));
                multIndex.push(numbers.size());
            }else{
                (*numbers.at(numbers.size() - 1)) = 1 / (*numbers.at(numbers.size() - 1));
                multIndex.push(numbers.size());
            }
        }else if(eq[i]=='^'){
            if(tmpc>0)
            {
                reversed=reverseString(tmp,tmpc);
                if(!reversed)
                    return std::nanf("");
                multIndex.push(numbers.push(strtof(reversed,nullptr)));
                powIndex.push(numbers.size());
                free(reversed);
                tmpc=0;
            }else if(i==end-1 || i==start)
            {
                return std::nanf("");
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
        else if(eq[i]==')'){
            //try to find a matching '(':
            char numClosingBrackets=0;
            char foundMatching=0;
            for(char j=i-1;j>=start;--j){
                if(eq[j]==')')
                    ++numClosingBrackets;
                else if(eq[j]=='(' && numClosingBrackets>0)
                    --numClosingBrackets;
                else if(eq[j]=='(' && numClosingBrackets==0)
                {
                    //matching '(' found
                    if(!foundMatching) {
                        numbers.push(solve(eq, j + 1, i));
                        i = j;//skip the part between () in parsing
                        foundMatching = 1;
                    }
                }
            }
            if(!foundMatching)
                return std::nanf("");
        }
        else{
            //unary operators:
            //trig functions work with rad not deg!
            if(i>2 && eq[i]=='n' && eq[i-1]=='i' && eq[i-2]=='s' && eq[i-3]=='a'){
                if(numbers.size())
                    *numbers.at(numbers.size()-1) = asin(*numbers.at(numbers.size()-1));
                i-=3;
                if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()) {
                    plusIndex.pop();
                }
            }
            else if(i>1 && eq[i]=='n' && eq[i-1]=='i' && eq[i-2]=='s'){
                if(numbers.size())
                    *numbers.at(numbers.size()-1) = sin(*numbers.at(numbers.size()-1));
                i-=2;
                if(plusIndex.size()>0 && *plusIndex.at(plusIndex.size()-1) == numbers.size()) {
                    plusIndex.pop();
                }
            }


            //constants
            else if(i>0 && eq[i]=='i' && eq[i-1]=='p'){
                numbers.push(M_PI);
                i-=1;
            }else if(eq[i]=='e'){
                numbers.push(M_E);
            }else if(i>1 && eq[i]=='s' && eq[i-1]=='n' && eq[i-2]=='a'){
                numbers.push(ans);
                i-=2;
            }
            else
                return std::nanf("");
        }
    }

    //push to numbers array one last time
    if(tmpc>0)
    {
        reversed=reverseString(tmp,tmpc);
        if(!reversed)
            return std::nanf("");
        numbers.push(strtof(reversed,nullptr));
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
        return std::nanf("");

    //we start with power a^b
    if(powIndex.size() > 0) {
        for (char i = powIndex.size()-1;i>=0; --i){
            //check if '*' is associated with two numbers:
            if(*powIndex.at(i)>= numbers.size())
                return std::nanf("");
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
            if(*multIndex.at(i)>= numbers.size())
                return std::nanf("");
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
    for (char i=0;i< plusIndex.size(); ++i){
        result+=*numbers.at(*plusIndex.at(i));
    }
    free(tmp);

    return result;
}
