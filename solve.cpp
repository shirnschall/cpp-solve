#include "solve.h"
#include "vector.h"


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

float solve(const char* eq,char start,char end){
    //check if eq is valid
    //if not, return NaN
    if(end<=start)
        return std::nanf("");

    //create new arrays to store numbers and operators
    Vector<float> numbers;
    Vector<char> plusIndex;
    Vector<char> multIndex;

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
            else if(i==end-1)
            {
                return std::nanf("");
            }
            //these two extra cases are necessary because a calculation like a++--+b is valid and equal to a+b
            else if(plusIndex.getLength()==0 || (plusIndex.getLength()>0 && numbers.getLength()-1!=*plusIndex.at(plusIndex.getLength()-1))){
                plusIndex.push(numbers.getLength());
            }
            else{
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
            }else if(i==end-1)
            {
                return std::nanf("");
            }
            //these two extra cases are necessary because a calculation like a++--+b is valid and equal to a+b
            else if(plusIndex.getLength()==0 || (plusIndex.getLength()>0 && numbers.getLength()-1!=*plusIndex.at(plusIndex.getLength()-1))){
                *numbers.at(numbers.getLength()-1)*=-1;
                plusIndex.push(numbers.getLength());
            }else{
                *numbers.at(numbers.getLength()-1)*=-1;
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
            }else{
                multIndex.push(numbers.getLength());
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
            }else{
                (*numbers.at(numbers.getLength()-1)) = 1/(*numbers.at(numbers.getLength()-1));
                multIndex.push(numbers.getLength());
            }
        }
        //if we find a bracket, try to find a matching one and call solve recursively
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
                    numbers.push(solve(eq,j+1,i));
                    i=j;//skip the part between () in parsing
                    foundMatching=1;
                }
            }
            if(!foundMatching)
                return std::nanf("");
        }
        //we should never find a single opening bracket '('
        else if(eq[i]=='('){
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

    //as we parsed eq from right to left, we have to go through the arrays from right to left
    //in to do calculations from left to right :)

    //because * is commutative we calculate the result for the multiplication from right to left
    //this makes it easier as we can store the result of a*b in a
    //e.g. a*(b*c)*d:
    //first the () was done in the parsing step so it is now a*e*d
    // we compute e*d=:f and replace e with the result f
    //this results in a*f*d. however we will ignore d and calculate a*f and replace a with the result.
    if(multIndex.getLength()>0) {
        for (char i = 0;i<multIndex.getLength() ;++i){
            //check if '*' is associated with two numbers:
            if(*multIndex.at(i)>=numbers.getLength())
                return std::nanf("");
            (*numbers.at(*multIndex.at(i))) *= (*numbers.at(*multIndex.at(i)-1));
        }
    }

    //we can now add the remaining numbers and return the result
    //Take a look at: a+b*c+d
    //based on the previous step we know that the result of b*c will be stored in b
    //we have to ignore c. this can be done using the value stored in the next plusIndex.
    //we sum the first number and all numbers that are to the right of a + symbol.
    //these numbers have an index of *plusIndex.at()-1
    float result=*numbers.at(numbers.getLength()-1);
    for (char i=0;i<plusIndex.getLength();++i){
        result+=*numbers.at(*plusIndex.at(i)-1);
    }
    free(tmp);

    return result;
}
