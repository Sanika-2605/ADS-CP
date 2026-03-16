#include <stdio.h>
#include <string.h>
#include "heap.h"

static void swapBill(Bill *a, Bill *b)
{
    Bill temp = *a;
    *a = *b;
    *b = temp;
}

static void heapifyUp(MinHeap *heap, int index)
{
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (strcmp(heap->items[index].dueDate, heap->items[parent].dueDate) < 0) {
            swapBill(&heap->items[index], &heap->items[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

static void heapifyDown(MinHeap *heap, int index)
{
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < heap->size &&
            strcmp(heap->items[left].dueDate, heap->items[smallest].dueDate) < 0) {
            smallest = left;
        }

        if (right < heap->size &&
            strcmp(heap->items[right].dueDate, heap->items[smallest].dueDate) < 0) {
            smallest = right;
        }

        if (smallest != index) {
            swapBill(&heap->items[index], &heap->items[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

void initHeap(MinHeap *heap)
{
    heap->size = 0;
}

int insertBill(MinHeap *heap, const char *name, double amount, const char *dueDate)
{
    if (heap->size >= MAX_BILLS) {
        return 0;
    }

    strncpy(heap->items[heap->size].name, name, BILL_NAME_LEN - 1);
    heap->items[heap->size].name[BILL_NAME_LEN - 1] = '\0';

    heap->items[heap->size].amount = amount;

    strncpy(heap->items[heap->size].dueDate, dueDate, BILL_DATE_LEN - 1);
    heap->items[heap->size].dueDate[BILL_DATE_LEN - 1] = '\0';

    heapifyUp(heap, heap->size);
    heap->size++;
    return 1;
}

const Bill *peekNextBill(const MinHeap *heap)
{
    if (heap->size == 0) {
        return NULL;
    }
    return &heap->items[0];
}

int extractMin(MinHeap *heap, Bill *outBill)
{
    if (heap->size == 0) {
        return 0;
    }

    if (outBill != NULL) {
        *outBill = heap->items[0];
    }

    heap->items[0] = heap->items[heap->size - 1];
    heap->size--;

    if (heap->size > 0) {
        heapifyDown(heap, 0);
    }
    return 1;
}

int isHeapEmpty(const MinHeap *heap)
{
    return heap->size == 0;
}