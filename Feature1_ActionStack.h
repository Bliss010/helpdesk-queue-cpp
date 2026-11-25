// Feature 1: ActionStack - our own stack using an array (no std::stack)
#ifndef ACTIONSTACK_H
#define ACTIONSTACK_H
#include "Ticket.h"

enum ActionType { ACTION_ADD, ACTION_SERVE, ACTION_CANCEL };

struct Action {
    ActionType type;
    Ticket ticket;
    int position;
};

class ActionStack {
private:
    Action* data;
    int capacity;
    int topIndex;

public:
    explicit ActionStack(int capacity = 1000);
    ~ActionStack();
    ActionStack(const ActionStack&) = delete;
    ActionStack& operator=(const ActionStack&) = delete;

    bool isEmpty() const;
    bool isFull() const;
    int size() const;
    bool push(const Action& a);
    bool pop(Action& removed);
    bool peek(Action& a) const;
};
#endif
