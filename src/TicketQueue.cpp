#include "TicketQueue.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cctype>

// ===================== Feature 1 =====================

TicketQueue::TicketQueue(int cap) {
    if (cap < 1) cap = 1;
    capacity = cap;
    data = new Ticket[capacity];
    frontIndex = 0;
    rearIndex = capacity - 1;
    count = 0;
}

TicketQueue::~TicketQueue() { delete[] data; }

// position 0 = front of the line
int TicketQueue::indexOf(int position) const {
    return (frontIndex + position) % capacity;
}

bool TicketQueue::isEmpty() const { return count == 0; }
bool TicketQueue::isFull() const  { return count == capacity; }
int TicketQueue::size() const     { return count; }

bool TicketQueue::enqueue(const Ticket& t) {
    if (isFull()) return false;
    rearIndex = (rearIndex + 1) % capacity;
    data[rearIndex] = t;
    count++;
    return true;
}

bool TicketQueue::dequeue(Ticket& removed) {
    if (isEmpty()) return false;
    removed = data[frontIndex];
    frontIndex = (frontIndex + 1) % capacity;
    count--;
    return true;
}

bool TicketQueue::peekFront(Ticket& t) const {
    if (isEmpty()) return false;
    t = data[frontIndex];
    return true;
}

Ticket TicketQueue::getAt(int position) const {
    if (position < 0 || position >= count)
        throw std::out_of_range("bad position");
    return data[indexOf(position)];
}

// ===================== Feature 3 =====================

int TicketQueue::totalMinutes() const {
    int total = 0;
    for (int i = 0; i < count; i++)
        total += data[indexOf(i)].getMinutesNeeded();
    return total;
}

void TicketQueue::print() const {
    if (isEmpty()) {
        std::cout << "The queue is empty.\n";
        return;
    }
    std::cout << std::left
              << std::setw(5)  << "Pos"
              << std::setw(6)  << "ID"
              << std::setw(20) << "Student"
              << std::setw(15) << "Category"
              << std::right << std::setw(9) << "Minutes" << "\n";
    std::cout << std::string(55, '-') << "\n";
    for (int i = 0; i < count; i++) {
        std::cout << std::left << std::setw(5) << (i + 1);
        data[indexOf(i)].print();
        std::cout << "\n";
    }
    std::cout << std::string(55, '-') << "\n";
    std::cout << "Total waiting minutes: " << totalMinutes() << "\n";
}

// ===================== Feature 4 =====================

static bool sameNameIgnoreCase(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++)
        if (std::tolower((unsigned char)a[i]) != std::tolower((unsigned char)b[i]))
            return false;
    return true;
}

// sum of the minutes of everyone in front of this position
int TicketQueue::waitBefore(int position) const {
    int wait = 0;
    for (int i = 0; i < position && i < count; i++)
        wait += data[indexOf(i)].getMinutesNeeded();
    return wait;
}

int TicketQueue::findById(int id) const {
    for (int i = 0; i < count; i++)
        if (data[indexOf(i)].getId() == id)
            return i;
    return -1;
}

int TicketQueue::findByName(const std::string& name, int positions[], int maxResults) const {
    int found = 0;
    for (int i = 0; i < count && found < maxResults; i++) {
        if (sameNameIgnoreCase(data[indexOf(i)].getStudentName(), name)) {
            positions[found] = i;
            found++;
        }
    }
    return found;
}
// ===================== Feature 5 and 6 =====================

// takes a ticket out of the line; everyone behind it moves up one place
bool TicketQueue::removeAt(int position, Ticket& removed) {
    if (position < 0 || position >= count) return false;
    removed = data[indexOf(position)];
    for (int i = position; i < count - 1; i++)
        data[indexOf(i)] = data[indexOf(i + 1)];
    count--;
    rearIndex = (frontIndex + count - 1 + capacity) % capacity;
    return true;
}

// puts a ticket into the line at this position; everyone from there back moves down one place
bool TicketQueue::insertAt(int position, const Ticket& t) {
    if (isFull() || position < 0 || position > count) return false;
    for (int i = count; i > position; i--)
        data[indexOf(i)] = data[indexOf(i - 1)];
    data[indexOf(position)] = t;
    count++;
    rearIndex = (frontIndex + count - 1) % capacity;
    return true;
}

