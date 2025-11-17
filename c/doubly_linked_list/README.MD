# Doubly Linked List Implementation in C

A complete implementation of a doubly linked list data structure with comprehensive testing and an interactive demo program.

## Features

- **Bidirectional traversal** with both `prev` and `next` pointers
- **Dynamic memory management** with proper allocation and deallocation
- **Comprehensive operations** for insertion, deletion, and searching
- **Interactive CLI** for hands-on exploration
- **Full test suite** with 50+ test cases

## Operations Supported

### Insertion
- `insertAtHead()` - Insert element at the beginning
- `insertAtTail()` - Insert element at the end
- `insertAt()` - Insert element at specific index

### Deletion
- `deleteAtHead()` - Remove element from the beginning
- `deleteAtTail()` - Remove element from the end
- `deleteAt()` - Remove element at specific index

### Traversal
- `printForward()` - Display list from head to tail
- `printBackward()` - Display list from tail to head

### Utility
- `search()` - Find if element exists in list
- `getSize()` - Get current number of elements
- `isEmpty()` - Check if list is empty
- `freeList()` - Free all allocated memory

## File Structure

```
doubly-linked-list/
├── doubly_linked_list.h          # Header file with declarations
├── doubly_linked_list.c          # Core implementation
├── main.c                        # Interactive demo program
├── test_doubly_linked_list.c     # Test suite
├── Makefile                      # Build automation
└── README.md                     # This file
```

## Building and Running

### Using Makefile (Recommended)

```bash
# Build everything
make

# Build and run tests
make test

# Build and run interactive program
make run

# Build only the interactive program
make dll_program

# Build only the test suite
make test_dll

# Clean build files
make clean
```

### Manual Compilation (Without Make)

```bash
# Compile interactive program
gcc -Wall -Wextra -std=c11 -g doubly_linked_list.c main.c -o dll_program

# Compile test suite
gcc -Wall -Wextra -std=c11 -g doubly_linked_list.c test_doubly_linked_list.c -o test_dll

# Run tests
./test_dll

# Run interactive program
./dll_program
```

## Usage Example

```c
#include "doubly_linked_list.h"

int main() {
    // Create a new list
    DoublyLinkedList* list = createList();
    
    // Insert elements
    insertAtHead(list, 10);
    insertAtTail(list, 20);
    insertAt(list, 15, 1);
    
    // Print the list
    printForward(list);   // Output: From Head: 10 <-> 15 <-> 20
    printBackward(list);  // Output: From Tail: 20 <-> 15 <-> 10
    
    // Search for element
    if (search(list, 15)) {
        printf("Found 15 in the list\n");
    }
    
    // Delete elements
    deleteAtHead(list);
    deleteAtTail(list);
    
    // Check size
    printf("Size: %d\n", getSize(list));
    
    // Clean up
    freeList(list);
    
    return 0;
}
```

## Interactive Program

The interactive program provides a menu-driven interface to explore all list operations:

```
=== Doubly Linked List Operations ===
1.  Insert at head
2.  Insert at tail
3.  Insert at index
4.  Delete from head
5.  Delete from tail
6.  Delete at index
7.  Search for value
8.  Print forward
9.  Print backward
10. Get size
11. Check if empty
12. Clear list
quit - Exit program
```

## Test Suite

The test suite includes comprehensive tests for:

- List creation and initialization
- Insert operations (head, tail, index)
- Delete operations (head, tail, index)
- Search functionality
- Size and empty state tracking
- Bidirectional link integrity
- Edge cases (empty list, single element, invalid indices)
- Mixed operations

Run tests to verify implementation:
```bash
./test_dll
```

Expected output:
```
========================================
  Doubly Linked List Test
========================================

Testing List Creation
PASS: List creation
PASS: Head is NULL on creation
...

========================================
  Test Results
========================================
Passed: 73
Failed: 0
Total:  73

All tests passed.
```

## Time Complexity

| Operation | Average Case | Worst Case |
|-----------|--------------|------------|
| Insert at head | O(1) | O(1) |
| Insert at tail | O(1) | O(1) |
| Insert at index | O(n) | O(n) |
| Delete at head | O(1) | O(1) |
| Delete at tail | O(1) | O(1) |
| Delete at index | O(n) | O(n) |
| Search | O(n) | O(n) |
| Get size | O(1) | O(1) |

## Space Complexity

- **Per node**: O(1) - Each node stores data, prev pointer, and next pointer
- **Overall**: O(n) - Where n is the number of elements in the list

## Memory Management

All memory is dynamically allocated using `malloc()` and must be freed using `freeList()` to prevent memory leaks. The implementation includes:

- Proper error handling for allocation failures
- Complete cleanup of all nodes when freeing the list
- No memory leaks (verified with valgrind)

## Requirements

- GCC compiler (or any C11 compatible compiler)
- Standard C library

## Author

Created as part of a data structures learning project.

## License

Free to use for educational purposes.
