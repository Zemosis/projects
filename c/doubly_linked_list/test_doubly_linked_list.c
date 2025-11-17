#include "doubly_linked_list.h"
#include <stdbool.h>
#include <stdio.h>

int tests_passed = 0;
int tests_failed = 0;

void test_assert(bool condition, const char *test_name) {
    if (condition) {
        printf("PASS: %s\n", test_name);
        tests_passed++;
    } else {
        printf("FAIL: %s\n", test_name);
        tests_failed++;
    }
}

void test_create_list() {
    printf("\nTesting List Creation\n");
    DoublyLinkedList *list = createList();
    test_assert(list != NULL, "List creation");
    test_assert(list->head == NULL, "Head is NULL on creation");
    test_assert(list->tail == NULL, "Tail is NULL on creation");
    test_assert(list->size == 0, "Size is 0 on creation");
    test_assert(isEmpty(list), "isEmpty returns true for new list");
    freeList(list);
}

void test_insert_at_head() {
    printf("\nTesting Insert at Head\n");
    DoublyLinkedList *list = createList();

    insertAtHead(list, 10);
    test_assert(list->head->data == 10, "First insert at head");
    test_assert(list->tail->data == 10,
                "Head and tail same after first insert");
    test_assert(list->size == 1, "Size is 1 after first insert");

    insertAtHead(list, 20);
    test_assert(list->head->data == 20, "Second insert at head");
    test_assert(list->tail->data == 10, "Tail unchanged after second insert");
    test_assert(list->size == 2, "Size is 2 after second insert");

    insertAtHead(list, 30);
    test_assert(list->head->data == 30, "Third insert at head");
    test_assert(list->size == 3, "Size is 3 after third insert");

    freeList(list);
}

void test_insert_at_tail() {
    printf("\nTesting Insert at Tail\n");
    DoublyLinkedList *list = createList();

    insertAtTail(list, 10);
    test_assert(list->tail->data == 10, "First insert at tail");
    test_assert(list->head->data == 10,
                "Head and tail same after first insert");
    test_assert(list->size == 1, "Size is 1 after first insert");

    insertAtTail(list, 20);
    test_assert(list->tail->data == 20, "Second insert at tail");
    test_assert(list->head->data == 10, "Head unchanged after second insert");
    test_assert(list->size == 2, "Size is 2 after second insert");

    insertAtTail(list, 30);
    test_assert(list->tail->data == 30, "Third insert at tail");
    test_assert(list->size == 3, "Size is 3 after third insert");

    freeList(list);
}

void test_insert_at_index() {
    printf("\nTesting Insert at Index\n");
    DoublyLinkedList *list = createList();

    insertAt(list, 10, 0);
    test_assert(list->head->data == 10, "Insert at index 0 on empty list");

    insertAt(list, 30, 1);
    test_assert(list->tail->data == 30, "Insert at index 1 at end");

    insertAt(list, 20, 1);
    test_assert(list->head->next->data == 20, "Insert at index 1 in middle");
    test_assert(list->size == 3, "Size is 3 after insertions");

    int oldSize = list->size;
    insertAt(list, 99, -1);
    test_assert(list->size == oldSize,
                "Insert at invalid negative index does nothing");

    insertAt(list, 99, 100);
    test_assert(list->size == oldSize,
                "Insert at invalid large index does nothing");

    freeList(list);
}

void test_delete_at_head() {
    printf("\nTesting Delete at Head\n");
    DoublyLinkedList *list = createList();

    int result = deleteAtHead(list);
    test_assert(result == -1, "Delete from empty list returns -1");

    insertAtTail(list, 10);
    insertAtTail(list, 20);
    insertAtTail(list, 30);

    result = deleteAtHead(list);
    test_assert(result == 10, "Delete at head returns correct value");
    test_assert(list->head->data == 20, "New head is correct");
    test_assert(list->size == 2, "Size decreases after delete");

    result = deleteAtHead(list);
    test_assert(result == 20, "Second delete returns correct value");

    result = deleteAtHead(list);
    test_assert(result == 30, "Third delete returns correct value");
    test_assert(isEmpty(list), "List is empty after all deletes");

    freeList(list);
}

void test_delete_at_tail() {
    printf("\nTesting Delete at Tail\n");
    DoublyLinkedList *list = createList();

    int result = deleteAtTail(list);
    test_assert(result == -1, "Delete from empty list returns -1");

    insertAtTail(list, 10);
    insertAtTail(list, 20);
    insertAtTail(list, 30);

    result = deleteAtTail(list);
    test_assert(result == 30, "Delete at tail returns correct value");
    test_assert(list->tail->data == 20, "New tail is correct");
    test_assert(list->size == 2, "Size decreases after delete");

    result = deleteAtTail(list);
    test_assert(result == 20, "Second delete returns correct value");

    result = deleteAtTail(list);
    test_assert(result == 10, "Third delete returns correct value");
    test_assert(isEmpty(list), "List is empty after all deletes");

    freeList(list);
}

void test_delete_at_index() {
    printf("\nTesting Delete at Index\n");
    DoublyLinkedList *list = createList();

    insertAtTail(list, 10);
    insertAtTail(list, 20);
    insertAtTail(list, 30);
    insertAtTail(list, 40);

    int result = deleteAt(list, 2);
    test_assert(result == 30, "Delete at index 2 returns correct value");
    test_assert(list->size == 3, "Size is 3 after delete");

    result = deleteAt(list, 0);
    test_assert(result == 10, "Delete at index 0 works");

    result = deleteAt(list, -1);
    test_assert(result == -1, "Delete at invalid negative index returns -1");

    result = deleteAt(list, 100);
    test_assert(result == -1, "Delete at invalid large index returns -1");

    freeList(list);
}

void test_search() {
    printf("\nTesting Search\n");
    DoublyLinkedList *list = createList();

    test_assert(!search(list, 10), "Search in empty list returns false");

    insertAtTail(list, 10);
    insertAtTail(list, 20);
    insertAtTail(list, 30);

    test_assert(search(list, 10), "Search finds first element");
    test_assert(search(list, 20), "Search finds middle element");
    test_assert(search(list, 30), "Search finds last element");
    test_assert(!search(list, 99),
                "Search returns false for non-existent element");

    freeList(list);
}

void test_size_and_empty() {
    printf("\nTesting Size and isEmpty\n");
    DoublyLinkedList *list = createList();

    test_assert(getSize(list) == 0, "Size is 0 for new list");
    test_assert(isEmpty(list), "isEmpty is true for new list");

    insertAtHead(list, 10);
    test_assert(getSize(list) == 1, "Size is 1 after one insert");
    test_assert(!isEmpty(list), "isEmpty is false after insert");

    insertAtHead(list, 20);
    insertAtHead(list, 30);
    test_assert(getSize(list) == 3, "Size is 3 after three inserts");

    deleteAtHead(list);
    test_assert(getSize(list) == 2, "Size is 2 after one delete");

    deleteAtHead(list);
    deleteAtHead(list);
    test_assert(getSize(list) == 0, "Size is 0 after all deletes");
    test_assert(isEmpty(list), "isEmpty is true after clearing list");

    freeList(list);
}

void test_bidirectional_links() {
    printf("\nTesting Bidirectional Links\n");
    DoublyLinkedList *list = createList();

    insertAtTail(list, 10);
    insertAtTail(list, 20);
    insertAtTail(list, 30);

    test_assert(list->head->data == 10, "Head is correct");
    test_assert(list->head->next->data == 20, "Forward link 1->2 correct");
    test_assert(list->head->next->next->data == 30,
                "Forward link 2->3 correct");

    test_assert(list->tail->data == 30, "Tail is correct");
    test_assert(list->tail->prev->data == 20, "Backward link 3->2 correct");
    test_assert(list->tail->prev->prev->data == 10,
                "Backward link 2->1 correct");

    test_assert(list->head->prev == NULL, "Head prev is NULL");
    test_assert(list->tail->next == NULL, "Tail next is NULL");

    freeList(list);
}

void test_mixed_operations() {
    printf("\nTesting Mixed Operations\n");
    DoublyLinkedList *list = createList();

    insertAtHead(list, 5);
    insertAtTail(list, 10);
    insertAt(list, 7, 1);

    test_assert(list->head->data == 5, "After mixed inserts head is 5");
    test_assert(list->head->next->data == 7, "After mixed inserts middle is 7");
    test_assert(list->tail->data == 10, "After mixed inserts tail is 10");
    test_assert(list->size == 3, "Size is 3 after mixed inserts");

    deleteAt(list, 1);
    test_assert(list->head->next->data == 10, "After delete at index 1");
    test_assert(list->size == 2, "Size is 2 after delete");

    freeList(list);
}

int main() {
    printf("========================================\n");
    printf("  Dounly Linked List Test\n");
    printf("========================================\n");

    test_create_list();
    test_insert_at_head();
    test_insert_at_tail();
    test_insert_at_index();
    test_delete_at_head();
    test_delete_at_tail();
    test_delete_at_index();
    test_search();
    test_size_and_empty();
    test_bidirectional_links();
    test_mixed_operations();

    printf("\n========================================\n");
    printf("  TEST RESULTS\n");
    printf("========================================\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);
    printf("Total:  %d\n", tests_passed + tests_failed);

    if (tests_failed == 0) {
        printf("\nAll tests passed.\n");
    } else {
        printf("\nSome tests failed.\n");
    }

    return tests_failed == 0 ? 0 : 1;
}
