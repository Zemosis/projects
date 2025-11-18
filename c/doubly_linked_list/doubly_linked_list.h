#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

typedef struct Node {
        int data;
        struct Node *prev;
        struct Node *next;
} Node;

typedef struct DoublyLinkedList {
        Node *head;
        Node *tail;
        int size;
} DoublyLinkedList;

DoublyLinkedList *createList();
Node *createNode(int data);
void insertAtHead(DoublyLinkedList *list, int data);
void insertAtTail(DoublyLinkedList *list, int data);
void insertAt(DoublyLinkedList *list, int data, int index);
int deleteAtHead(DoublyLinkedList *list);
int deleteAtTail(DoublyLinkedList *list);
int deleteAt(DoublyLinkedList *list, int index);
int search(DoublyLinkedList *list, int data);
void printForward(DoublyLinkedList *list);
void printBackward(DoublyLinkedList *list);
void freeList(DoublyLinkedList *list);
int getSize(DoublyLinkedList *list);
int isEmpty(DoublyLinkedList *list);

#endif
