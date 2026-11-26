// Feature 1: Ticket class (id, student name, category, minutes needed)
#ifndef TICKET_H
#define TICKET_H
#include <string>

class Ticket {
private:
    int id;
    std::string studentName;
    std::string category;
    int minutesNeeded;

public:
    Ticket();
    Ticket(int id, const std::string& studentName,
           const std::string& category, int minutesNeeded);

    int getId() const;
    std::string getStudentName() const;
    std::string getCategory() const;
    int getMinutesNeeded() const;
    void print() const;
};
#endif
