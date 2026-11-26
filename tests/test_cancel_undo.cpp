//M2 and M3 tests: cancel at the front, middle and rear, and three undos in a row
#include <iostream>
#include <cassert>
#include "Ticket.h"
#include "TicketQueue.h"
using namespace std;

// puts tickets 1 to 5 in the line
void fillFive(TicketQueue& q) {
    q.enqueue(Ticket(1, "Ana", "WiFi", 10));
    q.enqueue(Ticket(2, "Ben", "Printer", 15));
    q.enqueue(Ticket(3, "Cara", "Password", 5));
    q.enqueue(Ticket(4, "Dev", "WiFi", 20));
    q.enqueue(Ticket(5, "Eli", "Email", 10));
}

void testCancelFront() {
    TicketQueue q(10);
    Ticket out;
    fillFive(q);
    assert(q.removeAt(q.findById(1), out));
    assert(out.getId() == 1);
    assert(q.size() == 4);
    assert(q.getAt(0).getId() == 2);
    assert(q.getAt(1).getId() == 3);
    assert(q.getAt(2).getId() == 4);
    assert(q.getAt(3).getId() == 5);
    cout << "PASS  cancel at the front\n";
}

void testCancelMiddle() {
    TicketQueue q(10);
    Ticket out;
    fillFive(q);
    assert(q.removeAt(q.findById(3), out));
    assert(out.getId() == 3);
    assert(q.size() == 4);
    assert(q.getAt(0).getId() == 1);
    assert(q.getAt(1).getId() == 2);
    assert(q.getAt(2).getId() == 4);
    assert(q.getAt(3).getId() == 5);
    cout << "PASS  cancel in the middle\n";
}

void testCancelRear() {
    TicketQueue q(10);
    Ticket out;
    fillFive(q);
    assert(q.removeAt(q.findById(5), out));
    assert(out.getId() == 5);
    assert(q.size() == 4);
    assert(q.getAt(3).getId() == 4);
    assert(q.enqueue(Ticket(6, "Fay", "WiFi", 5)));   // rear still works
    assert(q.getAt(4).getId() == 6);
    cout << "PASS  cancel at the rear\n";
}

void testCancelWrapAround() {
    TicketQueue q(3);
    Ticket out;
    q.enqueue(Ticket(1, "A", "WiFi", 5));
    q.enqueue(Ticket(2, "B", "WiFi", 5));
    q.enqueue(Ticket(3, "C", "WiFi", 5));
    q.dequeue(out);
    q.dequeue(out);
    q.enqueue(Ticket(4, "D", "WiFi", 5));
    q.enqueue(Ticket(5, "E", "WiFi", 5));   // line is 3 4 5 and wraps around the array
    assert(q.removeAt(1, out) && out.getId() == 4);
    assert(q.getAt(0).getId() == 3);
    assert(q.getAt(1).getId() == 5);
    cout << "PASS  cancel with wrap-around\n";
}

// Does serve, cancel and add, then reverses them in the opposite order,
// the same way cmdUndo does.
void testThreeUndos() {
    TicketQueue q(10);
    Ticket served, cancelled, removed;
    fillFive(q);

    q.dequeue(served);                              // serve ticket 1
    int pos = q.findById(3);
    q.removeAt(pos, cancelled);                     // cancel ticket 3
    q.enqueue(Ticket(6, "Fay", "WiFi", 10));        // add ticket 6

    q.removeAt(q.size() - 1, removed);              // undo add
    assert(removed.getId() == 6);
    q.insertAt(pos, cancelled);                     // undo cancel
    q.insertAt(0, served);                          // undo serve

    assert(q.size() == 5);
    for (int i = 0; i < 5; i++)
        assert(q.getAt(i).getId() == i + 1);        // back to 1 2 3 4 5
    cout << "PASS  three undos restore the earlier queue\n";
}

void testInsertWhenFull() {
    TicketQueue q(2);
    q.enqueue(Ticket(1, "A", "WiFi", 5));
    q.enqueue(Ticket(2, "B", "WiFi", 5));
    assert(!q.insertAt(0, Ticket(3, "C", "WiFi", 5)));
    cout << "PASS  insert into a full queue fails\n";
}

int main() {
    testCancelFront();
    testCancelMiddle();
    testCancelRear();
    testCancelWrapAround();
    testThreeUndos();
    testInsertWhenFull();
    cout << "\nAll tests passed.\n";
    return 0;
}