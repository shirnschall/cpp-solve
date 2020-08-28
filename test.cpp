#include "solve.h"
#include "vector.h"
#include <iostream>

int main(){
    const char* testeq = "7/(3*1/5)-7*13/(3-(2-1)*5)-13/(12-2/3)+15";

    Vector<float> test;
    *test.at(5)=7;

    //std::cout<<test.at(5)<<std::endl<<*test.at(5)<<std::endl;

    test.resize(20);
    *test.at(15)=17;

    //std::cout<<test.at(15)<<std::endl<<*test.at(15);


    const char* t="(3-(2-1)*5)";


    std::cout<<std::endl<<testeq<<std::endl<<"\t\t\t="<<solve(testeq,0,41)<<std::endl;

}