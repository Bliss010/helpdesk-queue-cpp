// Feature 1: Ticket class
#include "Ticket.h"
#include <iostream>
#include <iomanip>

Ticket::Ticket() : id(0), studentName(""), category(""), minutesNeeded(0) {}

Ticket::Ticket(int id, const std::string& studentName,
               const std::string& category, int minutesNeeded)
    : id(id), studentName(studentName), category(category),
      minutesNeeded(minutesNeeded) {}

int Ticket::getId() const { return id; }
std::string Ticket::getStudentName() const { return studentName; }
std::string Ticket::getCategory() const { return category; }
int Ticket::getMinutesNeeded() const { return minutesNeeded; }

void Ticket::print() const {
    std::cout << std::left
              << std::setw(6)  << id
              << std::setw(20) << studentName
              << std::setw(15) << category
              << std::right << std::setw(5) << minutesNeeded << " min";
}
