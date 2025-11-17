#include "doubly_linked_list.h"
#include <stdio.h>
#include <string.h>

void displayMenu() {
    printf("\n=== Doubly Linked List Operations ===\n");
    printf("1.  Insert at head\n");
    printf("2.  Insert at tail\n");
    printf("3.  Insert at index\n");
    printf("4.  Delete from head\n");
    printf("5.  Delete from tail\n");
    printf("6.  Delete at index\n");
    printf("7.  Search for value\n");
    printf("8.  Print forward\n");
    printf("9.  Print backward\n");
    printf("10. Get size\n");
    printf("11. Check if empty\n");
    printf("12. Clear list\n");
    printf("quit - Exit program\n");
    printf("======================================\n");
    printf("Enter your choice: ");
}

int main() {
    DoublyLinkedList *list_1 = createList();

    printf("=== Doubly Linked List Demo ===\n\n");

    // Insert demos
    printf("Inserting at head:\n");
    insertAtHead(list_1, 3);
    insertAtHead(list_1, 2);
    insertAtHead(list_1, 1);
    printForward(list_1);
    printBackward(list_1);

    printf("\nInserting at tail:\n");
    insertAtTail(list_1, 4);
    insertAtTail(list_1, 5);
    insertAtTail(list_1, 6);
    printForward(list_1);

    printf("\nInserting 10 at index 3\n");
    insertAt(list_1, 10, 3);
    printForward(list_1);

    printf("\nSize: %d\n", getSize(list_1));

    // Search demos
    printf("\nSearching for 10: %s\n",
           search(list_1, 10) ? "Found" : "Not found");
    printf("Searching for 99: %s\n",
           search(list_1, 99) ? "Found" : "Not found");

    // Delete demos
    printf("\nDeleting from head: %d\n", deleteAtHead(list_1));
    printForward(list_1);

    printf("\nDeleting from tail: %d\n", deleteAtTail(list_1));
    printForward(list_1);

    printf("\nDeleting at index 2: %d\n", deleteAt(list_1, 2));
    printForward(list_1);
    printBackward(list_1);

    printf("\nFinal size: %d\n", getSize(list_1));
    printf("Is empty: %s\n", isEmpty(list_1) ? "Yes" : "No");

    // Free memory
    freeList(list_1);

    DoublyLinkedList *list_2 = createList();
    char buffer[69];
    char choice[10];
    int data, index, result, option;

    printf("=== Doubly Linked List ===\n");

    strcpy(choice, "");

    while (strcmp(choice, "quit") != 0) {
        displayMenu();

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = 0;

        if (sscanf(buffer, " %d", &option) == 1) {
            strcpy(choice, buffer);
        } else {
            sscanf(buffer, " %s", choice);
        }

        switch (option) {
        case 1:
            printf("Enter value to insert at head: ");
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                if (sscanf(buffer, "%d", &data) == 1) {
                    insertAtHead(list_2, data);
                    printf("Inserted %d at head\n", data);
                } else {
                    printf("Invalid input!\n");
                }
            }
            break;

        case 2:
            printf("Enter value to insert at tail: ");
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                if (sscanf(buffer, "%d", &data) == 1) {
                    insertAtTail(list_2, data);
                    printf("Inserted %d at tail\n", data);
                } else {
                    printf("Invalid input!\n");
                }
            }
            break;

        case 3:
            printf("Enter value to insert: ");
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                if (sscanf(buffer, "%d", &data) == 1) {
                    printf("Enter index: ");
                    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                        if (sscanf(buffer, "%d", &index) == 1) {
                            insertAt(list_2, data, index);
                            printf("Inserted %d at index %d\n", data, index);
                        } else {
                            printf("Invalid index!\n");
                        }
                    }
                } else {
                    printf("Invalid input!\n");
                }
            }
            break;

        case 4:
            result = deleteAtHead(list_2);
            if (result != -1) {
                printf("Deleted %d from head\n", result);
            }
            break;

        case 5:
            result = deleteAtTail(list_2);
            if (result != -1) {
                printf("Deleted %d from tail\n", result);
            }
            break;

        case 6:
            printf("Enter index to delete: ");
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                if (sscanf(buffer, "%d", &index) == 1) {
                    result = deleteAt(list_2, index);
                    if (result != -1) {
                        printf("Deleted %d at index %d\n", result, index);
                    }
                } else {
                    printf("Invalid input!\n");
                }
            }
            break;

        case 7:
            printf("Enter value to search: ");
            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                if (sscanf(buffer, "%d", &data) == 1) {
                    if (search(list_2, data)) {
                        printf("Value %d found in the list\n", data);
                    } else {
                        printf("Value %d not found in the list\n", data);
                    }
                } else {
                    printf("Invalid input!\n");
                }
            }
            break;

        case 8:
            printForward(list_2);
            break;

        case 9:
            printBackward(list_2);
            break;

        case 10:
            printf("Size: %d\n", getSize(list_2));
            break;

        case 11:
            if (isEmpty(list_2)) {
                printf("List is empty\n");
            } else {
                printf("List is not empty (size: %d)\n", getSize(list_2));
            }
            break;

        case 12:
            freeList(list_2);
            list_2 = createList();
            printf("List cleared\n");
            break;

        default:
            if (strcmp(choice, "quit") != 0) {
                printf("Invalid choice! Please try again.\n");
            }
            break;
        }

        option = 0;
    }

    freeList(list_2);

    printf("Exiting the program.\n");

    return 0;
}
