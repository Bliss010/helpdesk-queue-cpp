//Help Desk Ticket System [Features 1 to 7]
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
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

// ===================== Feature 5: cancel =====================

void cmdCancel(TicketQueue& queue, ActionStack& actions, const string& args) {
    int id;
    if (!parsePositiveInt(args, id)) {
        cout << "Usage: cancel <id>\n";
        return;
    }
    int pos = queue.findById(id);
    if (pos == -1) {
        cout << "Ticket #" << id << " is not in the line.\n";
        return;
    }
    Ticket t;
    queue.removeAt(pos, t);

    // save the old position so undo can put the ticket back in the same place
    Action a;
    a.type = ACTION_CANCEL;
    a.ticket = t;
    a.position = pos;
    actions.push(a);

    cout << "Cancelled ticket #" << t.getId() << " (" << t.getStudentName() << ").\n";
}

// ===================== Feature 6: undo =====================

void cmdUndo(TicketQueue& queue, ActionStack& actions, vector<Ticket>& served, int& nextId) {
    Action a;
    if (!actions.pop(a)) {
        cout << "Nothing to undo.\n";
        return;
    }

    if (a.type == ACTION_ADD) {
        // the added ticket is still the last one in line
        Ticket removed;
        queue.removeAt(queue.size() - 1, removed);
        nextId--;
        cout << "Undo add: ticket #" << a.ticket.getId() << " removed from the line.\n";
    }
    else if (a.type == ACTION_SERVE) {
        // the served ticket goes back to the front of the line
        queue.insertAt(0, a.ticket);
        served.pop_back();
        cout << "Undo serve: ticket #" << a.ticket.getId() << " is back at the front.\n";
    }
    else if (a.type == ACTION_CANCEL) {
        // the cancelled ticket goes back to its old position
        queue.insertAt(a.position, a.ticket);
        cout << "Undo cancel: ticket #" << a.ticket.getId() << " is back at position "
             << a.position + 1 << ".\n";
    }
}

// ===================== Feature 7: load and save =====================

// reads one CSV line "id,studentName,category,minutes,arrivalTime" into a ticket.
// arrival is the arrival time in minutes after midnight ("09:30" becomes 570).
bool readTicketLine(const string& line, Ticket& t, int& arrival) {
    istringstream in(line);
    string idText, name, category, minutesText, timeText;
    getline(in, idText, ',');
    getline(in, name, ',');
    getline(in, category, ',');
    getline(in, minutesText, ',');
    getline(in, timeText, ',');

    int id, minutes;
    if (!parsePositiveInt(idText, id) || !parsePositiveInt(minutesText, minutes))
        return false;   // broken line

    t = Ticket(id, trim(name), trim(category), minutes);

    arrival = 0;
    timeText = trim(timeText);
    if (timeText.size() == 5 && timeText[2] == ':') {   // looks like HH:MM
        int hours = stoi(timeText.substr(0, 2));
        int mins = stoi(timeText.substr(3, 2));
        arrival = hours * 60 + mins;
    }
    return true;
}

void cmdLoad(TicketQueue& queue, ActionStack& actions, vector<Ticket>& served,
             int& nextId, const string& args) {
    if (args.empty()) {
        cout << "Usage: load <file>\n";
        return;
    }
    ifstream file(args);
    if (!file.is_open()) {
        cout << "Could not open " << args << ".\n";
        return;
    }

    // a new day: empty the line, the served list and the undo history
    Ticket t;
    while (!queue.isEmpty())
        queue.dequeue(t);
    Action a;
    while (!actions.isEmpty())
        actions.pop(a);
    served.clear();

    string line;
    getline(file, line);   // skip the header line

    nextId = 1;
    int loaded = 0;
    while (getline(file, line)) {
        int arrival;
        if (!readTicketLine(line, t, arrival))
            continue;            // skip broken lines
        if (!queue.enqueue(t))
            break;               // the line is full
        loaded++;
        if (t.getId() >= nextId)
            nextId = t.getId() + 1;
    }
    cout << "Loaded " << loaded << " tickets from " << args << ".\n";
}

void cmdSave(const TicketQueue& queue, const string& args) {
    if (args.empty()) {
        cout << "Usage: save <file>\n";
        return;
    }
    ofstream file(args);
    if (!file.is_open()) {
        cout << "Could not write " << args << ".\n";
        return;
    }
    file << "id,studentName,category,minutes\n";
    for (int i = 0; i < queue.size(); i++) {
        Ticket t = queue.getAt(i);
        file << t.getId() << "," << t.getStudentName() << ","
             << t.getCategory() << "," << t.getMinutesNeeded() << "\n";
    }
    cout << "Saved " << queue.size() << " tickets to " << args << ".\n";
}

// ===================== M4: simulation =====================

// Serves a day of tickets from a CSV file with 1 or 2 agents.
// Each ticket goes to the agent who is free first. It starts once the
// student has arrived and that agent is free.
void simulate(const string& filename, int agents) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open " << filename << ".\n";
        return;
    }

    // read the day into our own queue, and each ticket's arrival time into an array
    TicketQueue day(100);
    int arrivals[100];
    int count = 0;
    string line;
    getline(file, line);   // skip the header line
    while (getline(file, line) && count < 100) {
        Ticket t;
        int arrival;
        if (!readTicketLine(line, t, arrival))
            continue;
        day.enqueue(t);
        arrivals[count] = arrival;
        count++;
    }
    if (count == 0) {
        cout << "No tickets in " << filename << ".\n";
        return;
    }

    int freeAt[2] = {0, 0};   // minute when each agent is next free
    int totalWait = 0;
    int longestWait = 0;

    for (int i = 0; i < count; i++) {
        Ticket t;
        day.dequeue(t);

        // pick the agent who is free first
        int agent = 0;
        if (agents == 2 && freeAt[1] < freeAt[0])
            agent = 1;

        // the ticket starts when the student is here and the agent is free
        int start = arrivals[i];
        if (freeAt[agent] > start)
            start = freeAt[agent];

        int wait = start - arrivals[i];
        freeAt[agent] = start + t.getMinutesNeeded();

        totalWait += wait;
        if (wait > longestWait)
            longestWait = wait;
    }

    double average = (double)totalWait / count;
    cout << agents << " agent(s), " << count << " tickets: average wait "
         << fixed << setprecision(1) << average << " min, longest wait "
         << longestWait << " min\n";
}

void cmdSimulate(const string& args) {
    if (args.empty()) {
        cout << "Usage: simulate <file>\n";
        return;
    }
    simulate(args, 1);
    simulate(args, 2);
}

// ===================== main =====================

int main() {
    TicketQueue queue(100);
    ActionStack actions(1000);
    vector<Ticket> served;
    int nextId = 1;

    cout << "=== University IT Help Desk ===\n";
    cout << "Commands: add, serve, print, search id <n>, search name <name>,\n"
         << "          cancel <id>, undo, load <file>, save <file>, simulate <file>, quit\n";
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
                else if (command == "cancel")   cmdCancel(queue, actions, args);         //Feature 5
        else if (command == "undo")     cmdUndo(queue, actions, served, nextId);         //Feature 6
        else if (command == "load")     cmdLoad(queue, actions, served, nextId, args);   //Feature 7
        else if (command == "save")     cmdSave(queue, args);                            //Feature 7
        else if (command == "simulate") cmdSimulate(args);                               //M4
        else if (command == "quit")   break;
        else cout << "Unknown command.\n";
    }
    return 0;
}
