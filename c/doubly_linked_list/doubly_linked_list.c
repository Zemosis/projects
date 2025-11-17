#include "doubly_linked_list.h"
#include <stdio.h>
#include <stdlib.h>

// Create a new doubly linked list
DoublyLinkedList *createList() {
    DoublyLinkedList *list =
        (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));
    if (list == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

// Create a new node
Node *createNode(int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Insert at the head
void insertAtHead(DoublyLinkedList *list, int data) {
    Node *newNode = createNode(data);

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        newNode->next = list->head;
        list->head->prev = newNode;
        list->head = newNode;
    }
    list->size++;
}

// Insert at the tail
void insertAtTail(DoublyLinkedList *list, int data) {
    Node *newNode = createNode(data);

    if (list->tail == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->size++;
}

// Insert at a specific index
void insertAt(DoublyLinkedList *list, int data, int index) {
    if (index < 0 || index > list->size) {
        printf("Invalid index!\n");
        return;
    }

    if (index == 0) {
        insertAtHead(list, data);
        return;
    }

    if (index == list->size) {
        insertAtTail(list, data);
        return;
    }

    Node *newNode = createNode(data);
    Node *current = list->head;

    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    newNode->next = current;
    newNode->prev = current->prev;
    current->prev->next = newNode;
    current->prev = newNode;
    list->size++;
}

// Delete from head
int deleteAtHead(DoublyLinkedList *list) {
    if (list->head == NULL) {
        printf("List is empty!\n");
        return -1;
    }

    Node *temp = list->head;
    int data = temp->data;

    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->head = list->head->next;
        list->head->prev = NULL;
    }

    free(temp);
    list->size--;
    return data;
}

// Delete from tail
int deleteAtTail(DoublyLinkedList *list) {
    if (list->tail == NULL) {
        printf("List is empty!\n");
        return -1;
    }

    Node *temp = list->tail;
    int data = temp->data;

    if (list->head == list->tail) {
        list->head = NULL;
        list->tail = NULL;
    } else {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }

    free(temp);
    list->size--;
    return data;
}

// Delete at specific index
int deleteAt(DoublyLinkedList *list, int index) {
    if (index < 0 || index >= list->size) {
        printf("Invalid index!\n");
        return -1;
    }

    if (index == 0) {
        return deleteAtHead(list);
    }

    if (index == list->size - 1) {
        return deleteAtTail(list);
    }

    Node *current = list->head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    int data = current->data;
    current->prev->next = current->next;
    current->next->prev = current->prev;
    free(current);
    list->size--;
    return data;
}

// Search for a value
int search(DoublyLinkedList *list, int data) {
    Node *current = list->head;
    while (current != NULL) {
        if (current->data == data) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// Print list forward
void printForward(DoublyLinkedList *list) {
    if (list->head == NULL) {
        printf("List is empty!\n");
        return;
    }

    Node *current = list->head;
    printf("From Head: ");
    while (current != NULL) {
        printf("%d", current->data);
        if (current->next != NULL) {
            printf(" <-> ");
        }
        current = current->next;
    }
    printf("\n");
}

// Print list backward
void printBackward(DoublyLinkedList *list) {
    if (list->tail == NULL) {
        printf("List is empty!\n");
        return;
    }

    Node *current = list->tail;
    printf("From Tail: ");
    while (current != NULL) {
        printf("%d", current->data);
        if (current->prev != NULL) {
            printf(" <-> ");
        }
        current = current->prev;
    }
    printf("\n");
}

// Free entire list
void freeList(DoublyLinkedList *list) {
    Node *current = list->head;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    free(list);
}

int getSize(DoublyLinkedList *list) { return list->size; }

int isEmpty(DoublyLinkedList *list) { return list->size == 0; }
