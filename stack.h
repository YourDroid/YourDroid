#ifndef STACK_H
#define STACK_H

template<typename type>
class Stack {
    type *array;
    int top = 1;
public:
    Stack() : array(new type[1]) {}
    void push(type);
    type pop();
    type operator[](int i){ return array[i]; }
    int length() { return top - 1; }
};

#include "stack.hpp"

#endif // STACK_H
