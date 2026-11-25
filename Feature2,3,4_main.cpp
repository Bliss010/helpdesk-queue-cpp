// Help Desk Ticket System - Features 1 to 4
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Ticket.h"
#include "TicketQueue.h"
#include "ActionStack.h"
using namespace std;

// ===================== input helpers =====================

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool parsePositiveInt(const string& s, int& value) {
    string t = trim(s);
    if (t.empty() || t.size() > 9) return false;
    for (char c : t)
        if (c < '0' || c > '9') return false;
    value = stoi(t);
    return value > 0;
}

bool askText(const string& prompt, string& answer) {
    while (true) {
        cout << prompt;
        if (!getline(cin, answer)) return false;
        answer = trim(answer);
        if (!answer.empty()) return true;
        cout << "  Please type something.\n";
    }
}

bool askPositiveInt(const string& prompt, int& value) {
    string line;
    while (true) {
        cout << prompt;
        if (!getline(cin, line)) return false;
        if (parsePositiveInt(line, value)) return true;
        cout << "  Please enter a whole number greater than 0.\n";
    }
}

// ===================== Feature 2: add and serve =====================

void cmdAdd(TicketQueue& queue, ActionStack& actions, int& nextId) {
    if (queue.isFull()) {
        cout << "Sorry, the queue is full.\n";
        return;
    }
    string name, category;
    int minutes;
    if (!askText("Student name: ", name)) return;
    if (!askText("Category (Password, Printer, WiFi): ", category)) return;
    if (!askPositiveInt("Minutes needed: ", minutes)) return;

    Ticket t(nextId, name, category, minutes);
    queue.enqueue(t);
    nextId++;

    Action a;
    a.type = ACTION_ADD;
    a.ticket = t;
    a.position = queue.size() - 1;
    actions.push(a);

    cout << "Ticket #" << t.getId() << " added for " << name
         << ". Position in line: " << queue.size() << "\n";
}

void cmdServe(TicketQueue& queue, ActionStack& actions, vector<Ticket>& served) {
    Ticket t;
    if (!queue.dequeue(t)) {
        cout << "No tickets to serve. The queue is empty.\n";
        return;
    }
    served.push_back(t);

    Action a;
    a.type = ACTION_SERVE;
    a.ticket = t;
    a.position = 0;
    actions.push(a);

    cout << "Now serving ticket #" << t.getId() << ": " << t.getStudentName()
         << " (" << t.getCategory() << ", " << t.getMinutesNeeded() << " min)\n";
}

// ===================== Feature 4: search =====================

void showResult(const TicketQueue& queue, int position) {
    Ticket t = queue.getAt(position);
    cout << "Ticket #" << t.getId() << " - " << t.getStudentName()
         << " (" << t.getCategory() << ", " << t.getMinutesNeeded() << " min)\n"
         << "  Position in line: " << position + 1 << " of " << queue.size() << "\n"
         << "  Estimated wait:   " << queue.waitBefore(position) << " min\n";
}

void cmdSearch(const TicketQueue& queue, const string& args) {
    istringstream in(args);
    string kind, rest;
    in >> kind;
    getline(in, rest);
    rest = trim(rest);

    if (kind == "id") {
        int id;
        if (!parsePositiveInt(rest, id)) { cout << "Usage: search id <number>\n"; return; }
        int pos = queue.findById(id);
        if (pos == -1) cout << "Ticket #" << id << " is not in the line.\n";
        else showResult(queue, pos);
    }
    else if (kind == "name") {
        if (rest.empty()) { cout << "Usage: search name <name>\n"; return; }
        int positions[100];
        int found = queue.findByName(rest, positions, 100);
        if (found == 0) cout << "No ticket for \"" << rest << "\".\n";
        else
            for (int i = 0; i < found; i++)
                showResult(queue, positions[i]);
    }
    else {
        cout << "Usage: search id <number>  OR  search name <name>\n";
    }
}

// ===================== main =====================

int main() {
    TicketQueue queue(100);
    ActionStack actions(1000);
    vector<Ticket> served;
    int nextId = 1;

    cout << "=== University IT Help Desk ===\n";
    cout << "Commands: add, serve, print, search id <n>, search name <name>, quit\n";

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;
        line = trim(line);
        if (line.empty()) continue;

        istringstream in(line);
        string command, args;
        in >> command;
        getline(in, args);
        args = trim(args);

        if (command == "add")         cmdAdd(queue, actions, nextId);      // Feature 2
        else if (command == "serve")  cmdServe(queue, actions, served);    // Feature 2
        else if (command == "print")  queue.print();                       // Feature 3
        else if (command == "search") cmdSearch(queue, args);              // Feature 4
        else if (command == "quit")   break;
        else cout << "Unknown command.\n";
    }
    return 0;
}
