#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <stdbool.h>

typedef struct Node {
        int data;
        struct Node *prev;
        struct Node *next;
} Node;

typedef struct {
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
bool search(DoublyLinkedList *list, int data);
void freeList(DoublyLinkedList *list);
int getSize(DoublyLinkedList *list);
bool isEmpty(DoublyLinkedList *list);

#endif
