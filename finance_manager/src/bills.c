#include "bills.h"

void initBillManager(BillManager *bm) {
    bm->size = 0;
}

void addBill(BillManager *bm, const char *name, double amount, const char *dueDate) {
    if (bm->size >= MAX_BILLS) {
        printf("Bill capacity full!\n");
        return;
    }

    Bill newBill;
    strncpy(newBill.name, name, BILL_NAME_LEN - 1);
    newBill.amount = amount;
    strncpy(newBill.dueDate, dueDate, BILL_DATE_LEN - 1);

    int i = bm->size++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (strcmp(newBill.dueDate, bm->heap[p].dueDate) >= 0) break;
        bm->heap[i] = bm->heap[p];
        i = p;
    }
    bm->heap[i] = newBill;
}

Bill* peekNextBill(BillManager *bm) {
    if (bm->size == 0) return NULL;
    return &bm->heap[0];
}

Bill popNextBill(BillManager *bm) {
    Bill root = bm->heap[0];
    Bill last = bm->heap[--bm->size];

    int i = 0;
    while (i * 2 + 1 < bm->size) {
        int child = i * 2 + 1;
        if (child + 1 < bm->size && strcmp(bm->heap[child + 1].dueDate, bm->heap[child].dueDate) < 0) {
            child++;
        }
        if (strcmp(last.dueDate, bm->heap[child].dueDate) <= 0) break;
        bm->heap[i] = bm->heap[child];
        i = child;
    }
    if (bm->size > 0) bm->heap[i] = last;
    return root;
}

void printAllBills(BillManager *bm) {
    if (bm->size == 0) {
        printf("No pending bills.\n");
        return;
    }
    printf("\n--- Upcoming Bills (Priority) ---\n");
    printf("%-20s %-10s %-12s\n", "Bill Name", "Amount", "Due Date");
    // Note: Printing a heap is not sorted, but the root is the next due.
    // For a real app, we might extract all to display sorted, but here we'll just show the priority.
    for (int i = 0; i < bm->size; i++) {
        printf("%-20s %-10.2f %-12s\n", bm->heap[i].name, bm->heap[i].amount, bm->heap[i].dueDate);
    }
}
