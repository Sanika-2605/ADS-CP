#ifndef HEAP_H
#define HEAP_H

#define MAX_BILLS 100
#define BILL_NAME_LEN 50
#define BILL_DATE_LEN 11

typedef struct {
    char name[BILL_NAME_LEN];
    double amount;
    char dueDate[BILL_DATE_LEN];
} Bill;

typedef struct {
    Bill items[MAX_BILLS];
    int size;
} MinHeap;

void initHeap(MinHeap *heap);
int insertBill(MinHeap *heap, const char *name, double amount, const char *dueDate);
const Bill *peekNextBill(const MinHeap *heap);
int extractMin(MinHeap *heap, Bill *outBill);
int isHeapEmpty(const MinHeap *heap);

#endif
