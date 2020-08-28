#include "solve.h"
#include "vector.h"
#include <iostream>

int main(){
    const char* testeq = "15*2+8/2";

    Vector<float> test;
    *test.at(5)=7;

    //std::cout<<test.at(5)<<std::endl<<*test.at(5)<<std::endl;

    test.resize(20);
    *test.at(15)=17;

    //std::cout<<test.at(15)<<std::endl<<*test.at(15);


    const char* t="5+(2-3)*15.7";


    std::cout<<std::endl<<testeq<<std::endl<<"\t\t"<<solve(testeq,0,8)<<std::endl;

}