#include <stdio.h>
#include <string.h>
#include "transaction.h"

int dateToDay(const char *date)
{
    int year;
    int month;
    int day;
    int i;
    int total = 0;
    int monthDays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) {
        return 1;
    }

    if (month < 1 || month > 12 || day < 1 || day > 31) {
        return 1;
    }

    for (i = 1; i < month; i++) {
        total += monthDays[i];
    }
    total += day;

    if (total < 1) {
        total = 1;
    }
    if (total > MAX_DAYS) {
        total = MAX_DAYS;
    }
    return total;
}

void initTransactionManager(TransactionManager *manager)
{
    manager->count = 0;
}

int addTransaction(TransactionManager *manager,
                   TrieNode *vendorTrie,
                   SegmentTree *expenseTree,
                   const char *vendor,
                   const char *category,
                   double amount,
                   const char *date)
{
    Transaction *transaction;
    const char *suggestedCategory;

    if (manager->count >= MAX_TRANSACTIONS) {
        return 0;
    }

    transaction = &manager->items[manager->count];

    strncpy(transaction->vendor, vendor, VENDOR_LEN - 1);
    transaction->vendor[VENDOR_LEN - 1] = '\0';

    suggestedCategory = searchVendor(vendorTrie, vendor);
    if (suggestedCategory != NULL) {
        strncpy(transaction->category, suggestedCategory, CATEGORY_LEN - 1);
        transaction->category[CATEGORY_LEN - 1] = '\0';
    } else {
        strncpy(transaction->category, category, CATEGORY_LEN - 1);
        transaction->category[CATEGORY_LEN - 1] = '\0';
        insertVendor(vendorTrie, vendor, transaction->category);
    }

    transaction->amount = amount;

    strncpy(transaction->date, date, DATE_LEN_TXN - 1);
    transaction->date[DATE_LEN_TXN - 1] = '\0';

    transaction->day = dateToDay(date);

    updateExpense(expenseTree, transaction->day, amount);
    manager->count++;
    return 1;
}

void printTransactions(const TransactionManager *manager)
{
    int i;
    if (manager->count == 0) {
        printf("No transactions found.\n");
        return;
    }

    printf("\nTransactions:\n");
    printf("%-4s %-20s %-15s %-12s %-12s\n", "No", "Vendor", "Category", "Amount", "Date");

    for (i = 0; i < manager->count; i++) {
        const Transaction *t = &manager->items[i];
        printf("%-4d %-20s %-15s %-12.2f %-12s\n",
               i + 1,
               t->vendor,
               t->category,
               t->amount,
               t->date);
    }
}