// Feature 1: TicketQueue - our own queue using a circular array (no std::queue)
#ifndef TICKETQUEUE_H
#define TICKETQUEUE_H
#include "Ticket.h"
#include <string>

class TicketQueue {
private:
    Ticket* data;
    int capacity;
    int frontIndex;
    int rearIndex;
    int count;
    int indexOf(int position) const;

public:
    explicit TicketQueue(int capacity = 100);
    ~TicketQueue();
    TicketQueue(const TicketQueue&) = delete;
    TicketQueue& operator=(const TicketQueue&) = delete;

    // Feature 1
    bool isEmpty() const;
    bool isFull() const;
    int size() const;
    bool enqueue(const Ticket& t);
    bool dequeue(Ticket& removed);
    bool peekFront(Ticket& t) const;
    Ticket getAt(int position) const;

    // Feature 3
    int totalMinutes() const;
    void print() const;

    // Feature 4
    int waitBefore(int position) const;
    int findById(int id) const;
    int findByName(const std::string& name, int positions[], int maxResults) const;
};
#endif
