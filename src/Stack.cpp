// Implementation here
#include "Stack.h"
#include "CardTypes.h"
#include <stdexcept>

template <class T>
Stack<T>::Stack()
{
    vector<T> s;
    this->S = s;
}

template <class T>
Stack<T>::Stack(vector<T> s)
{
    this->S = s;
}

template <class T>
Stack<T> Stack<T>::push(T e)
{
    vector<T> new_vec(this->S);
    new_vec.push_back(e);
    return Stack<T>(new_vec);
}

template <class T>
Stack<T> Stack<T>::pop()
{
    // raise out_of_range error if stack is empty.
    if (this->S.empty())
        throw out_of_range("Cannot pop from empty stack!");

    vector<T> new_vec(this->S);
    new_vec.pop_back();
    return Stack<T>(new_vec);
}

template <class T>
T Stack<T>::top()
{
    if (this->S.empty())
        throw out_of_range("Cannot find top of empty stack!");

    return this->S.back();
}

template <class T>
unsigned int Stack<T>::size()
{
    return this->S.size();
}

template <class T>
vector<T> Stack<T>::toSeq()
{
    return this->S;
}

// Keep this at bottom
template class Stack<CardT>;