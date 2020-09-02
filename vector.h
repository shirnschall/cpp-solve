//
// Created by Sebastian Hirnschall on 28.08.20.
//

#ifndef SOLVE_VECTOR_H
#define SOLVE_VECTOR_H


#include <cstdio>
#include <cstdlib>

#define DEFAULT_ARRAY_SIZE 10
#define DEFAULT_INCREMENT DEFAULT_ARRAY_SIZE
#define MAX_NUMBER_LENGTH 10

template<typename T>
class Vector{
    T* data;
    char length;
    char currentIndex;

public:
    Vector();
    ~Vector();
    char resize(char size=0);
    T *at(int index);
    char push(T value); //return size() after adding new element
    char pop();
    char size(); //return number of elements in data array
};


template<typename T>
Vector<T>::Vector():currentIndex(0){
    data = (T*)malloc(DEFAULT_ARRAY_SIZE*sizeof(T));
    if(!data)
        length=0;
    else
        length=DEFAULT_ARRAY_SIZE;
}
template<typename T>
Vector<T>::~Vector(){
    free(data);
}
template<typename T>
T * Vector<T>::at(int index) {
    if(index<0 || index>= length)
        return NULL;
    return &data[index];
}
template<typename T>
char Vector<T>::resize(char size) {
    if(size==-1)
        size=DEFAULT_ARRAY_SIZE;
    //check if array exists. if not use malloc
    if(!data)
    {
        data = (T*)malloc(size*sizeof(T));
    }else{
        data = (T*)realloc(data,size*sizeof(T));
    }
    if(!data) {
        length = 0;
        return 0;
    }else {
        length = size;
        return size;
    }

}

template <typename T>
char Vector<T>::size() {
    return currentIndex;
}

template <typename T>
char Vector<T>::push(T value) {
    if(currentIndex>=length) {
        if (!resize(length + DEFAULT_INCREMENT))
            return 0;
    }

    data[currentIndex]=value;
    return ++currentIndex;
}

template <typename T>
char Vector<T>::pop() {
    if(currentIndex>0) {
        return --currentIndex;
    }
    return 0;
}

#endif //SOLVE_VECTOR_H
