# Help Desk Ticket System (C++)

A university IT help desk gets requests all day (password resets, broken printers, Wi-Fi problems).
Tickets are served in arrival order. This project builds the system on our own queue and stack
classes, without `std::queue` or `std::stack`.

## Features done
| Feature | What it does |
|---|---|
| F1 | `Ticket` class, `TicketQueue` (circular array) and `ActionStack` (array) |
| F2 | `add` puts a new ticket at the rear, `serve` removes the front ticket |
| F3 | `print` lists waiting tickets front to rear with total waiting minutes |
| F4 | `search id` / `search name` show position in line and estimated wait |
| F5 | `cancel <id>` removes a ticket from the middle without changing the order of the others |
| F6 | `undo` reverses the last add, serve or cancel (a cancelled ticket goes back to its old position) |
| F7 | `load <file>` reads a day of tickets from a CSV file, `save <file>` writes the line at closing time |
| M4 | `simulate <file>` reports the average and longest wait for 1 and 2 agents |

## Class design
- **TicketQueue (queue, FIFO):** used by add (enqueue), serve (dequeue), print, search,
  cancel (`removeAt`), load/save and the simulation (each agent takes the next ticket with dequeue).
- **ActionStack (stack, LIFO):** every add, serve and cancel pushes an action; undo pops the most
  recent one and reverses it:
  - undo **add**: the new ticket is still last in line, so it is removed from the rear and the next id goes back by one
  - undo **serve**: the ticket goes back to the front of the line (`insertAt(0)`)
  - undo **cancel**: the ticket goes back to its saved position (`insertAt(position)`)

  Because the stack is last in, first out, each undo sees the line exactly as it was right after that action.

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

## Commands
```
add                  add a new ticket
serve                serve the ticket at the front
print                list waiting tickets and total waiting minutes
search id <n>        position in line and estimated wait
search name <name>   same, by student name
cancel <id>          remove a ticket from the line
undo                 undo the last add, serve or cancel
load <file>          load a day of tickets (replaces the line and clears undo history)
save <file>          save the waiting tickets to a CSV file
simulate <file>      average and longest wait for 1 and 2 agents
quit
```

CSV format (see `data/`). `arrivalTime` is used by the simulation; `load` accepts files with or without it.
```
id,studentName,category,minutes,arrivalTime
1,Aisha Khan,Software,22,09:03
```

## Run the tests
```
g++ -std=c++17 -Wall -I src -o run_tests tests/test_structures.cpp src/Ticket.cpp src/TicketQueue.cpp src/ActionStack.cpp
./run_tests

g++ -std=c++17 -Wall -I src -o run_cancel_undo_tests tests/test_cancel_undo.cpp src/Ticket.cpp src/TicketQueue.cpp src/ActionStack.cpp
./run_cancel_undo_tests
```
`test_structures.cpp` covers M1 (empty, one element and full structures). `test_cancel_undo.cpp` covers
M2 (cancel at the front, middle and rear, order of the others unchanged) and M3 (three undos in a row
restore the earlier queue).

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

Cancel, undo, load and save:
```
> load data/tickets_small.csv
Loaded 10 tickets from data/tickets_small.csv.
> cancel 3
Cancelled ticket #3 (Aisha Khan).
> search id 5
Ticket #5 - Ali Brown (Email, 10 min)
  Position in line: 4 of 9
  Estimated wait:   38 min
> serve
Now serving ticket #1: Aisha Khan (Software, 22 min)
> add
Student name: Sara
Category (Password, Printer, WiFi): Printer
Minutes needed: 10
Ticket #11 added for Sara. Position in line: 9
> undo
Undo add: ticket #11 removed from the line.
> undo
Undo serve: ticket #1 is back at the front.
> undo
Undo cancel: ticket #3 is back at position 3.
> save closing.csv
Saved 10 tickets to closing.csv.
```

## Waiting-time results (M4)

Tickets are served in arrival order. Each ticket goes to the agent who is free first, and service
starts once the student has arrived and that agent is free. Wait = start time − arrival time.

| Day | Tickets | 1 agent: average | 1 agent: longest | 2 agents: average | 2 agents: longest |
|---|---|---|---|---|---|
| `tickets_small.csv` | 10 | 29.5 min | 61 min | 3.2 min | 9 min |
| `tickets_day.csv` | 40 | 154.9 min | 310 min | 12.4 min | 49 min |
| `tickets_busy_day.csv` | 80 | 403.8 min | 762 min | 95.5 min | 176 min |

On a normal day, a second agent cuts the average wait from about 2.5 hours to about 12 minutes.
On the busy day, even two agents fall behind, so the desk would need a third agent at peak times.