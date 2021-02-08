//
// Created by Sebastian Hirnschall on 28.08.20.
//

#ifndef SOLVE_VECTOR_H
#define SOLVE_VECTOR_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_ARRAY_SIZE 10
#define DEFAULT_INCREMENT 10
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
    T* getData();
    char insert(T value, char index);
    char remove(char startIndex, char endIndex); //start and endindex will also be removed! return number of elements after removal
    char clear();
};

template <typename T>
char Vector<T>::clear() {
    currentIndex=0;
    //check if array exists. if not use malloc
    if(!data)
    {
        data = (T*)malloc(DEFAULT_ARRAY_SIZE*sizeof(T));
    }else{
        data = (T*)realloc(data,DEFAULT_ARRAY_SIZE*sizeof(T));
    }
    if(!data) {
        length = 0;
        return 0;
    }else {
        length = DEFAULT_ARRAY_SIZE;
        return length;
    }
}

template <typename T>
char Vector<T>::insert(T value, char index) {
    if(currentIndex>=length) {
        if (!resize(length + DEFAULT_INCREMENT))
            return 0;
    }
    if(index!=currentIndex && index >=0 && index <currentIndex)
    {
        memmove(at(index+1),at(index),sizeof(T)*(currentIndex-index));
        data[index]=value;
    }else if(index == currentIndex)
        data[index]=value;

    return ++currentIndex;
}

template<typename T>
char Vector<T>::remove(char startIndex, char endIndex){
    if(startIndex >=0 && startIndex<size() && endIndex>=startIndex && endIndex<size()){

        //we do not overwrite values that have index greater than currentIndex! we memcpy all elements after endindex to starrtindex
        memmove(at(startIndex),at(endIndex),sizeof(T)*(currentIndex-1-endIndex));

        currentIndex -= (endIndex-startIndex)+1;//adjust number of elements counter
    }
    return currentIndex;
}

template<typename T>
T* Vector<T>::getData(){
    return data;
}

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
    //check if array exists. if not use malloc
    if(!data)
    {
        data = (T*)malloc(DEFAULT_ARRAY_SIZE*sizeof(T));
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
