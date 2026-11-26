# Pseudocode – Help Desk Ticket System (Features 1 to 4)

---

## Feature 1: Ticket, TicketQueue and ActionStack

### 1.1 Ticket – constructor
```
BEGIN Ticket(id, name, category, minutes)
    SET this.id = id
    SET this.studentName = name
    SET this.category = category
    SET this.minutesNeeded = minutes
END
```

### 1.2 Ticket – getters
```
BEGIN getId()
    RETURN id
END

BEGIN getStudentName()
    RETURN studentName
END

BEGIN getCategory()
    RETURN category
END

BEGIN getMinutesNeeded()
    RETURN minutesNeeded
END
```

### 1.3 Ticket – print
```
BEGIN print()
    PRINT id, studentName, category, minutesNeeded in one row
END
```

### 1.4 TicketQueue – constructor
```
BEGIN TicketQueue(capacity)
    IF capacity < 1 THEN
        SET capacity = 1
    END IF
    CREATE array data of size capacity
    SET front = 0
    SET rear = capacity - 1      // so the first ticket goes to index 0
    SET count = 0
END
```

### 1.5 TicketQueue – indexOf
```
BEGIN indexOf(position)
    // changes a place in the line into an array index
    RETURN (front + position) MOD capacity
END
```

### 1.6 TicketQueue – isEmpty, isFull, size
```
BEGIN isEmpty()
    IF count = 0 THEN RETURN true
    ELSE RETURN false
END

BEGIN isFull()
    IF count = capacity THEN RETURN true
    ELSE RETURN false
END

BEGIN size()
    RETURN count
END
```

### 1.7 TicketQueue – enqueue
```
BEGIN enqueue(ticket)
    IF isFull() THEN
        RETURN false
    END IF
    SET rear = (rear + 1) MOD capacity     // go back to 0 at the end
    SET data[rear] = ticket
    SET count = count + 1
    RETURN true
END
```

### 1.8 TicketQueue – dequeue
```
BEGIN dequeue(removed)
    IF isEmpty() THEN
        RETURN false
    END IF
    SET removed = data[front]
    SET front = (front + 1) MOD capacity
    SET count = count - 1
    RETURN true
END
```

### 1.9 TicketQueue – peekFront
```
BEGIN peekFront(ticket)
    IF isEmpty() THEN
        RETURN false
    END IF
    SET ticket = data[front]
    RETURN true
END
```

### 1.10 TicketQueue – getAt
```
BEGIN getAt(position)
    IF position < 0 OR position >= count THEN
        SHOW error "bad position"
    END IF
    RETURN data[indexOf(position)]
END
```

### 1.11 ActionStack – constructor
```
BEGIN ActionStack(capacity)
    IF capacity < 1 THEN
        SET capacity = 1
    END IF
    CREATE array data of size capacity
    SET top = -1                 // -1 means the stack is empty
END
```

### 1.12 ActionStack – isEmpty, isFull, size
```
BEGIN isEmpty()
    IF top = -1 THEN RETURN true
    ELSE RETURN false
END

BEGIN isFull()
    IF top = capacity - 1 THEN RETURN true
    ELSE RETURN false
END

BEGIN size()
    RETURN top + 1
END
```

### 1.13 ActionStack – push
```
BEGIN push(action)
    IF isFull() THEN
        RETURN false
    END IF
    SET top = top + 1
    SET data[top] = action
    RETURN true
END
```

### 1.14 ActionStack – pop
```
BEGIN pop(removed)
    IF isEmpty() THEN
        RETURN false
    END IF
    SET removed = data[top]
    SET top = top - 1
    RETURN true
END
```

### 1.15 ActionStack – peek
```
BEGIN peek(action)
    IF isEmpty() THEN
        RETURN false
    END IF
    SET action = data[top]
    RETURN true
END
```

---

## Feature 2: add and serve

### 2.1 Reading a positive number
```
BEGIN askPositiveInt(message)
    REPEAT
        PRINT message
        INPUT line
        IF line is a whole number AND line > 0 THEN
            RETURN line as a number
        ELSE
            PRINT "Please enter a whole number greater than 0"
        END IF
    UNTIL a valid number is entered
END
```

### 2.2 add
```
BEGIN add()
    IF queue is full THEN
        PRINT "Sorry, the queue is full"
        STOP
    END IF

    INPUT name                       // cannot be empty
    INPUT category                   // cannot be empty
    SET minutes = askPositiveInt("Minutes needed: ")

    CREATE ticket with (nextId, name, category, minutes)
    CALL queue.enqueue(ticket)
    SET nextId = nextId + 1

    // save the action for undo later
    CREATE action with type = ADD, ticket, position = queue.size() - 1
    CALL actions.push(action)

    PRINT "Ticket", ticket id, "added. Position in line:", queue.size()
END
```

### 2.3 serve
```
BEGIN serve()
    IF queue.dequeue(ticket) is false THEN
        PRINT "No tickets to serve. The queue is empty"
        STOP
    END IF

    ADD ticket to servedList

    CREATE action with type = SERVE, ticket, position = 0
    CALL actions.push(action)

    PRINT "Now serving ticket", ticket id, name, category, minutes
END
```

---

## Feature 3: print

### 3.1 totalMinutes
```
BEGIN totalMinutes()
    SET total = 0
    FOR i = 0 TO count - 1
        SET total = total + data[indexOf(i)].minutesNeeded
    END FOR
    RETURN total
END
```

### 3.2 print
```
BEGIN print()
    IF isEmpty() THEN
        PRINT "The queue is empty"
        STOP
    END IF

    PRINT heading: Pos, ID, Student, Category, Minutes
    FOR i = 0 TO count - 1
        PRINT i + 1
        CALL data[indexOf(i)].print()
    END FOR
    PRINT "Total waiting minutes:", totalMinutes()
END
```

---

## Feature 4: search id and search name

### 4.1 waitBefore
```
BEGIN waitBefore(position)
    // adds the minutes of everyone in front of this position
    SET wait = 0
    FOR i = 0 TO position - 1
        SET wait = wait + data[indexOf(i)].minutesNeeded
    END FOR
    RETURN wait
END
```

### 4.2 findById
```
BEGIN findById(id)
    FOR i = 0 TO count - 1
        IF data[indexOf(i)].id = id THEN
            RETURN i
        END IF
    END FOR
    RETURN -1                        // not found
END
```

### 4.3 sameNameIgnoreCase
```
BEGIN sameNameIgnoreCase(a, b)
    IF length of a is not equal to length of b THEN
        RETURN false
    END IF
    FOR i = 0 TO length of a - 1
        IF lowercase(a[i]) is not equal to lowercase(b[i]) THEN
            RETURN false
        END IF
    END FOR
    RETURN true
END
```

### 4.4 findByName
```
BEGIN findByName(name, positions)
    SET found = 0
    FOR i = 0 TO count - 1
        IF sameNameIgnoreCase(data[indexOf(i)].studentName, name) THEN
            SET positions[found] = i
            SET found = found + 1
        END IF
    END FOR
    RETURN found                     // how many matches
END
```

### 4.5 showResult
```
BEGIN showResult(position)
    SET ticket = queue.getAt(position)
    PRINT ticket id, name, category, minutes
    PRINT "Position in line:", position + 1, "of", queue.size()
    PRINT "Estimated wait:", queue.waitBefore(position), "min"
END
```

### 4.6 search
```
BEGIN search(kind, value)
    IF kind = "id" THEN
        SET pos = queue.findById(value)
        IF pos = -1 THEN
            PRINT "Ticket is not in the line"
        ELSE
            CALL showResult(pos)
        END IF

    ELSE IF kind = "name" THEN
        SET found = queue.findByName(value, positions)
        IF found = 0 THEN
            PRINT "No ticket for this name"
        ELSE
            FOR k = 0 TO found - 1
                CALL showResult(positions[k])
            END FOR
        END IF

    ELSE
        PRINT "Usage: search id <number> OR search name <name>"
    END IF
END
```

---

## Main program

```
BEGIN main()
    CREATE queue with capacity 100
    CREATE actions stack with capacity 1000
    CREATE empty servedList
    SET nextId = 1

    PRINT list of commands

    REPEAT
        INPUT command
        IF command = "add" THEN
            CALL add()
        ELSE IF command = "serve" THEN
            CALL serve()
        ELSE IF command = "print" THEN
            CALL queue.print()
        ELSE IF command = "search" THEN
            CALL search(kind, value)
        ELSE IF command = "quit" THEN
            EXIT loop
        ELSE
            PRINT "Unknown command"
        END IF
    UNTIL user types quit
END
```

## Feature 5: cancel
 
### 5.1 TicketQueue – removeAt
```
BEGIN removeAt(position, removed)
    IF position < 0 OR position >= count THEN
        RETURN false
    END IF
    SET removed = data[indexOf(position)]
    // everyone behind the ticket moves up one place
    FOR i = position TO count - 2
        SET data[indexOf(i)] = data[indexOf(i + 1)]
    END FOR
    SET count = count - 1
    SET rear = (front + count - 1) MOD capacity
    RETURN true
END
```
 
### 5.2 cancel
```
BEGIN cancel(id)
    SET pos = queue.findById(id)
    IF pos = -1 THEN
        PRINT "Ticket is not in the line"
        STOP
    END IF
    CALL queue.removeAt(pos, ticket)
 
    // save the old position so undo can put the ticket back
    CREATE action with type = CANCEL, ticket, position = pos
    CALL actions.push(action)
 
    PRINT "Cancelled ticket", ticket id
END
```
 
---
 
## Feature 6: undo
 
### 6.1 TicketQueue – insertAt
```
BEGIN insertAt(position, ticket)
    IF isFull() OR position < 0 OR position > count THEN
        RETURN false
    END IF
    // everyone from this position to the rear moves down one place
    FOR i = count DOWN TO position + 1
        SET data[indexOf(i)] = data[indexOf(i - 1)]
    END FOR
    SET data[indexOf(position)] = ticket
    SET count = count + 1
    SET rear = (front + count - 1) MOD capacity
    RETURN true
END
```
 
### 6.2 undo
```
BEGIN undo()
    IF actions.pop(action) is false THEN
        PRINT "Nothing to undo"
        STOP
    END IF
 
    IF action.type = ADD THEN
        // the added ticket is still the last one in line
        CALL queue.removeAt(queue.size() - 1, ticket)
        SET nextId = nextId - 1
    ELSE IF action.type = SERVE THEN
        CALL queue.insertAt(0, action.ticket)          // back to the front
        REMOVE the last ticket from servedList
    ELSE IF action.type = CANCEL THEN
        CALL queue.insertAt(action.position, action.ticket)   // back to its old place
    END IF
 
    PRINT what was undone
END
```
 
---
 
## Feature 7: load and save
 
### 7.1 readTicketLine
```
BEGIN readTicketLine(line)
    SPLIT line at commas into id, studentName, category, minutes, arrivalTime
    IF id or minutes is not a whole number > 0 THEN
        RETURN false                       // broken line
    END IF
    CREATE ticket with (id, studentName, category, minutes)
    SET arrival = 0
    IF arrivalTime looks like HH:MM THEN
        SET arrival = HH * 60 + MM         // "09:30" becomes 570
    END IF
    RETURN true
END
```
 
### 7.2 load
```
BEGIN load(filename)
    OPEN filename
    IF the file cannot be opened THEN
        PRINT "Could not open file"
        STOP
    END IF
 
    // a new day: empty the line, the served list and the undo history
    WHILE queue is not empty: DEQUEUE
    WHILE actions is not empty: POP
    CLEAR servedList
 
    SKIP the header line
    SET nextId = 1
    FOR each line in the file
        IF readTicketLine(line) is false THEN
            SKIP this line
        END IF
        IF queue.enqueue(ticket) is false THEN
            STOP reading                   // the line is full
        END IF
        IF ticket id >= nextId THEN
            SET nextId = ticket id + 1
        END IF
    END FOR
    PRINT how many tickets were loaded
END
```
 
### 7.3 save
```
BEGIN save(filename)
    OPEN filename for writing
    IF it cannot be opened THEN
        PRINT "Could not write file"
        STOP
    END IF
    WRITE "id,studentName,category,minutes"
    FOR i = 0 TO queue.size() - 1
        SET ticket = queue.getAt(i)
        WRITE ticket id, name, category, minutes separated by commas
    END FOR
END
```
 
---
 
## M4: simulation
 
```
BEGIN simulate(filename, agents)
    READ every ticket of the file into dayQueue, and its arrival time into arrivals[]
    SET freeAt[0] = 0, freeAt[1] = 0        // minute when each agent is next free
    SET totalWait = 0, longestWait = 0
 
    FOR i = 0 TO count - 1
        CALL dayQueue.dequeue(ticket)
        SET agent = 0
        IF agents = 2 AND freeAt[1] < freeAt[0] THEN
            SET agent = 1                    // the agent who is free first
        END IF
        SET start = the later of arrivals[i] and freeAt[agent]
        SET wait = start - arrivals[i]
        SET freeAt[agent] = start + ticket.minutesNeeded
        SET totalWait = totalWait + wait
        IF wait > longestWait THEN SET longestWait = wait
    END FOR
 
    PRINT average wait = totalWait / number of tickets, and longestWait
END
```
 
---
 
## Main program
 
```
BEGIN main()
    CREATE queue with capacity 100
    CREATE actions stack with capacity 1000
    CREATE empty servedList
    SET nextId = 1
 
    PRINT list of commands
 
    REPEAT
        INPUT command
        IF command = "add" THEN
            CALL add()
        ELSE IF command = "serve" THEN
            CALL serve()
        ELSE IF command = "print" THEN
            CALL queue.print()
        ELSE IF command = "search" THEN
            CALL search(kind, value)
        ELSE IF command = "cancel" THEN
            CALL cancel(id)
        ELSE IF command = "undo" THEN
            CALL undo()
        ELSE IF command = "load" THEN
            CALL load(filename)
        ELSE IF command = "save" THEN
            CALL save(filename)
        ELSE IF command = "simulate" THEN
            CALL simulate(filename, 1) and simulate(filename, 2)
        ELSE IF command = "quit" THEN
            EXIT loop
        ELSE
            PRINT "Unknown command"
        END IF
    UNTIL user types quit
END
```