#ifndef BILLS_H
#define BILLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BILL_NAME_LEN 50
#define BILL_DATE_LEN 11

typedef struct {
    char name[BILL_NAME_LEN];
    double amount;
    char dueDate[BILL_DATE_LEN];
} Bill;

#define MAX_BILLS 100
typedef struct {
    Bill heap[MAX_BILLS];
    int size;
} BillManager;

void initBillManager(BillManager *bm);
void addBill(BillManager *bm, const char *name, double amount, const char *dueDate);
Bill* peekNextBill(BillManager *bm);
Bill popNextBill(BillManager *bm);
void printAllBills(BillManager *bm);

#endif
