# Pseudocode (Features 1 to 4)

## Feature 1: Ticket, TicketQueue and ActionStack

```
CLASS Ticket
    id, studentName, category, minutesNeeded
    CONSTRUCTOR sets all fields
    getters return each field

CLASS TicketQueue (circular array)
    data[capacity], front ← 0, rear ← capacity − 1, count ← 0

    indexOf(pos)  : RETURN (front + pos) MOD capacity
    isEmpty()     : RETURN count = 0
    isFull()      : RETURN count = capacity

    enqueue(t):
        IF isFull() RETURN false
        rear ← (rear + 1) MOD capacity
        data[rear] ← t
        count ← count + 1
        RETURN true

    dequeue(OUT t):
        IF isEmpty() RETURN false
        t ← data[front]
        front ← (front + 1) MOD capacity
        count ← count − 1
        RETURN true

CLASS ActionStack (array)
    data[capacity], top ← −1

    push(a):
        IF top = capacity − 1 RETURN false
        top ← top + 1
        data[top] ← a
        RETURN true

    pop(OUT a):
        IF top = −1 RETURN false
        a ← data[top]
        top ← top − 1
        RETURN true
```

## Feature 2: add and serve

```
nextId ← 1

PROCEDURE add
    IF queue is full: print "queue full", RETURN
    READ name, category, minutes (minutes must be > 0)
    t ← Ticket(nextId, name, category, minutes)
    queue.enqueue(t)
    nextId ← nextId + 1
    actions.push(ADD, t, queue.size − 1)
    print id and position

PROCEDURE serve
    IF NOT queue.dequeue(t): print "queue empty", RETURN
    add t to servedList
    actions.push(SERVE, t, 0)
    print "Now serving" + t
```

## Feature 3: print

```
FUNCTION totalMinutes
    total ← 0
    FOR i ← 0 TO count − 1
        total ← total + minutes of ticket i
    RETURN total

PROCEDURE print
    IF queue is empty: print "queue empty", RETURN
    print header
    FOR i ← 0 TO count − 1
        print i + 1 and ticket i
    print "Total waiting minutes: " + totalMinutes()
```

## Feature 4: search id and search name

```
FUNCTION waitBefore(pos)
    wait ← 0
    FOR i ← 0 TO pos − 1
        wait ← wait + minutes of ticket i
    RETURN wait

FUNCTION findById(id)
    FOR i ← 0 TO count − 1
        IF ticket i has this id: RETURN i
    RETURN −1

FUNCTION findByName(name, OUT positions[])
    found ← 0
    FOR i ← 0 TO count − 1
        IF ticket i has this name (ignore case)
            positions[found] ← i
            found ← found + 1
    RETURN found

PROCEDURE search
    IF "id":   pos ← findById(id)
               IF pos = −1: print "not found"
               ELSE print position (pos + 1) and waitBefore(pos)
    IF "name": found ← findByName(name, positions)
               IF found = 0: print "not found"
               ELSE print position and wait for each match
```

## Main loop

```
LOOP
    READ command
    "add"    → add
    "serve"  → serve
    "print"  → print
    "search" → search
    "quit"   → stop
```
