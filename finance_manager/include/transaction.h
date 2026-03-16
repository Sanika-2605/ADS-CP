#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "trie.h"
#include "segment_tree.h"

#define MAX_TRANSACTIONS 500
#define VENDOR_LEN 50
#define CATEGORY_LEN 50
#define DATE_LEN_TXN 11

typedef struct {
    char vendor[VENDOR_LEN];
    char category[CATEGORY_LEN];
    double amount;
    char date[DATE_LEN_TXN];
    int day;
} Transaction;

typedef struct TransactionManager {
    Transaction items[MAX_TRANSACTIONS];
    int count;
} TransactionManager;

void initTransactionManager(TransactionManager *manager);
int dateToDay(const char *date);
int addTransaction(TransactionManager *manager,
                   TrieNode *vendorTrie,
                   SegmentTree *expenseTree,
                   const char *vendor,
                   const char *category,
                   double amount,
                   const char *date);
void printTransactions(const TransactionManager *manager);

#endif
