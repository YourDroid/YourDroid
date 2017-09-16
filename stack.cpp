#include "stack.h"
#include <stdlib.h>

template<typename type>
void Stack<type>::push(type var) {
    array = (type*)realloc(array, sizeof(++top));
    array[top] = var;
}

template<typename type>
type Stack<type>::pop() {
    type last = array[top];
    array = (type*)realloc(array, sizeof(--top));
    return last;
}

//template<typename type>
//type Stack<type>::operator[](int i) {
//    return array[i];
//}
