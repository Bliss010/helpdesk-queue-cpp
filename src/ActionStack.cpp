// Feature 1: ActionStack
#include "ActionStack.h"

ActionStack::ActionStack(int cap) {
    if (cap < 1) cap = 1;
    capacity = cap;
    data = new Action[capacity];
    topIndex = -1;
}

ActionStack::~ActionStack() { delete[] data; }

bool ActionStack::isEmpty() const { return topIndex == -1; }
bool ActionStack::isFull() const  { return topIndex == capacity - 1; }
int ActionStack::size() const     { return topIndex + 1; }

bool ActionStack::push(const Action& a) {
    if (isFull()) return false;
    topIndex++;
    data[topIndex] = a;
    return true;
}

bool ActionStack::pop(Action& removed) {
    if (isEmpty()) return false;
    removed = data[topIndex];
    topIndex--;
    return true;
}

bool ActionStack::peek(Action& a) const {
    if (isEmpty()) return false;
    a = data[topIndex];
    return true;
}
