#include<iostream>
#include"TQueue.h"
#include"TMultiStack.h"


int main(){
    TMultiStack<int> s(3,2);
    s.Push(0,1);
    s.Push(0,2);

    s.Push(1,1);

    s.Push(2,1);
    //репак типа
    s.Push(0,3);
    s.Push(0,4);

    std::cout<< s;
}