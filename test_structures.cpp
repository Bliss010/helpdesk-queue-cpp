// M1 unit tests: empty, one element and full structures
#include <iostream>
#include <cassert>
#include "Ticket.h"
#include "TicketQueue.h"
#include "ActionStack.h"
using namespace std;

void testQueueEmpty() {
    TicketQueue q(3);
    Ticket out;
    assert(q.isEmpty());
    assert(!q.isFull());
    assert(!q.dequeue(out));
    assert(q.totalMinutes() == 0);
    assert(q.findById(1) == -1);
    cout << "PASS  queue empty\n";
}

void testQueueOne() {
    TicketQueue q(3);
    Ticket out;
    assert(q.enqueue(Ticket(1, "Sara", "Printer", 10)));
    assert(q.size() == 1);
    assert(q.findById(1) == 0);
    assert(q.waitBefore(0) == 0);
    assert(q.dequeue(out) && out.getId() == 1);
    assert(q.isEmpty());
    cout << "PASS  queue one element\n";
}

void testQueueFull() {
    TicketQueue q(3);
    q.enqueue(Ticket(1, "A", "WiFi", 5));
    q.enqueue(Ticket(2, "B", "Password", 10));
    q.enqueue(Ticket(3, "C", "Printer", 20));
    assert(q.isFull());
    assert(!q.enqueue(Ticket(4, "D", "WiFi", 1)));
    assert(q.totalMinutes() == 35);
    assert(q.waitBefore(2) == 15);
    cout << "PASS  queue full\n";
}

void testQueueWrapAround() {
    TicketQueue q(3);
    Ticket out;
    q.enqueue(Ticket(1, "A", "WiFi", 5));
    q.enqueue(Ticket(2, "B", "WiFi", 6));
    q.enqueue(Ticket(3, "C", "WiFi", 7));
    q.dequeue(out);
    q.dequeue(out);
    q.enqueue(Ticket(4, "D", "WiFi", 8));
    q.enqueue(Ticket(5, "E", "WiFi", 9));
    assert(q.getAt(0).getId() == 3);
    assert(q.getAt(1).getId() == 4);
    assert(q.getAt(2).getId() == 5);
    cout << "PASS  queue wrap-around\n";
}

void testSearchName() {
    TicketQueue q(5);
    q.enqueue(Ticket(1, "Ali Khan", "WiFi", 5));
    q.enqueue(Ticket(2, "Sara", "Printer", 10));
    q.enqueue(Ticket(3, "ali khan", "Password", 3));
    int pos[5];
    assert(q.findByName("ALI KHAN", pos, 5) == 2);
    assert(pos[0] == 0 && pos[1] == 2);
    cout << "PASS  search by name\n";
}

void testStack() {
    ActionStack s(2);
    Action a, out;
    assert(s.isEmpty());
    assert(!s.pop(out));                                  // empty
    a.type = ACTION_ADD; a.ticket = Ticket(1, "A", "WiFi", 5); a.position = 0;
    assert(s.push(a) && s.size() == 1);                   // one element
    a.type = ACTION_SERVE;
    assert(s.push(a) && s.isFull());                      // full
    assert(!s.push(a));
    assert(s.pop(out) && out.type == ACTION_SERVE);       // last in, first out
    assert(s.pop(out) && out.type == ACTION_ADD);
    cout << "PASS  stack empty / one / full\n";
}

int main() {
    testQueueEmpty();
    testQueueOne();
    testQueueFull();
    testQueueWrapAround();
    testSearchName();
    testStack();
    cout << "\nAll tests passed.\n";
    return 0;
}
