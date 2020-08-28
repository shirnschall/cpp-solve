#include "solve.h"
#include "vector.h"
#include <iostream>

int main(){
    const char* testeq = "7/(3*1/5)-7";

    Vector<float> test;
    *test.at(5)=7;

    //std::cout<<test.at(5)<<std::endl<<*test.at(5)<<std::endl;

    test.resize(20);
    *test.at(15)=17;

    //std::cout<<test.at(15)<<std::endl<<*test.at(15);


    const char* t="5+(2-3)*15.7";


    std::cout<<std::endl<<testeq<<std::endl<<"\t\t"<<solve(testeq,0,11)<<std::endl;

}