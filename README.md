# Help Desk Ticket System (C++)

A university IT help desk gets requests all day (password resets, broken printers, Wi-Fi problems).
Tickets are served in arrival order. This project builds the system on my own queue and stack
classes, without `std::queue` or `std::stack`.

## Features done
| Feature | What it does |
|---|---|
| F1 | `Ticket` class, `TicketQueue` (circular array) and `ActionStack` (array) |
| F2 | `add` puts a new ticket at the rear, `serve` removes the front ticket |
| F3 | `print` lists waiting tickets front to rear with total waiting minutes |
| F4 | `search id` / `search name` show position in line and estimated wait |

## Class design
- **TicketQueue (queue, FIFO):** used by add (enqueue), serve (dequeue), print and search.
- **ActionStack (stack, LIFO):** every add and serve pushes an action, ready for undo later.

## Folder structure
```
src/      source code
tests/    unit tests for the data structures
docs/     pseudocode
data/     sample day of tickets (CSV)
```

## Build and run
```
g++ -std=c++17 -Wall -o helpdesk src/*.cpp
./helpdesk
```

## Run the tests
```
g++ -std=c++17 -Wall -I src -o run_tests tests/test_structures.cpp src/Ticket.cpp src/TicketQueue.cpp src/ActionStack.cpp
./run_tests
```

## Sample session
```
> add
Student name: Ali Khan
Category (Password, Printer, WiFi): WiFi
Minutes needed: 15
Ticket #1 added for Ali Khan. Position in line: 1
> add
Student name: Sara
Category (Password, Printer, WiFi): Printer
Minutes needed: 10
Ticket #2 added for Sara. Position in line: 2
> print
Pos  ID    Student             Category         Minutes
-------------------------------------------------------
1    1     Ali Khan            WiFi              15 min
2    2     Sara                Printer           10 min
-------------------------------------------------------
Total waiting minutes: 25
> search id 2
Ticket #2 - Sara (Printer, 10 min)
  Position in line: 2 of 2
  Estimated wait:   15 min
> serve
Now serving ticket #1: Ali Khan (WiFi, 15 min)
> quit
```
