#pragma once
#include <iostream>
#include "list.h"
#include "Card.h"
using namespace std;

template <typename T>
class Stack 
{
public:
    Dlist <T> list;

    void push(T d) 
    {
        list.InsertAtStart(d);
    }

    T pop() 
    {
        return list.DeleteStart();
    }

   /* Card* retLast() {
        return *list.peekTail();
    }*/

    bool isEmpty() 
    {
        return list.isEmpty();
    }
    
    void display() 
    {
        list.displayTopCard();
    }

    T revealTop()
    {
        Card* top;
        top = list.head-> next->data;
        return top;
    }

    T& deleteElement(T& d)
    {
        T elem = list.DeleteElement(d);
        return elem;
    }

    int countElem()
    {
        int i = 0; 
        for (Dlist <Card*> :: Iterator it = list.begin(); it != list.end(); it++)
        {
            i++;
        }
        return i;
    }
};