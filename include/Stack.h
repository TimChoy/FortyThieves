/**
 * \file Stack.h
 * \author Timothy Choy - choyt2
 * \brief Stack
 * \date Mar 8, 2019
 */
#ifndef A3_STACK_H_
#define A3_STACK_H_
#include <vector>

using namespace std;

template <class T>
class Stack
{
 private:
   vector<T> S;

 public:
   /**
    * \brief Default constructor for Stack ADT
    */
   Stack();

   /**
     * \brief Constructor for Stack ADT that takes a seq of T
     */
   Stack(vector<T> s);

   /**
     * \brief push takes an element and pushes it to the top of the stack
     * \param e is an element that will be pushed to the stack
     * \return a new stack with the new element push onto the stack
     */
   Stack<T> push(T e);

   /**
     * \brief pop removes the top element of the stack
     * \return a new stack with the top element removed
     * \throws out_of_range if the stack is empty

     */
   Stack<T> pop();

   /**
     * \brief top returns the element at the top of the stack
     * \return the element at the top of the stack
     * \throws out_of_range if the stack is empty
     */
   T top();

   /**
     * \brief size returns the size of the stack
     * \return a natural number showing the number of elements in the stack
     */
   unsigned int size();

   /**
     * \brief toSeq returns a sequence of elements representing the stack
     * \return a sequence of elements which are the elements in the stack
     */
   vector<T> toSeq();
};

#endif
