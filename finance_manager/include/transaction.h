#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 26
#define VENDOR_LEN 100
#define CATEGORY_LEN 50
#define DATE_LEN 11 // YYYY-MM-DD

// Trie Node for vendor/category matching
typedef struct TrieNode {
    struct TrieNode *children[MAX_CHAR];
    char category[CATEGORY_LEN];
    int isEndOfWord;
} TrieNode;

// Segment Tree for range sum queries
// We'll use a fixed size for simple demonstration (e.g., 366 days for a year)
#define DAYS_IN_YEAR 366
typedef struct {
    double tree[4 * DAYS_IN_YEAR];
} SegmentTree;

// Transaction Structure
typedef struct {
    char vendor[VENDOR_LEN];
    char category[CATEGORY_LEN];
    double amount;
    char date[DATE_LEN];
    int dayOfYear; // Simplified for Segment Tree (1-366)
} Transaction;

// Function Prototypes - Trie
TrieNode* createTrieNode();
void insertVendor(TrieNode *root, const char *vendor, const char *category);
const char* searchCategory(TrieNode *root, const char *vendor);
void freeTrie(TrieNode *root);

// Function Prototypes - Segment Tree
void initSegmentTree(SegmentTree *st);
void updateExpense(SegmentTree *st, int day, double amount, int node, int start, int end);
double queryExpense(SegmentTree *st, int L, int R, int node, int start, int end);

// Transaction Manager
typedef struct {
    Transaction *transactions;
    int count;
    int capacity;
    TrieNode *vendorTrie;
    SegmentTree expenseTree;
} TransactionManager;

void initTransactionManager(TransactionManager *tm);
void addTransaction(TransactionManager *tm, const char *vendor, const char *category, double amount, int day);
void printTransactions(TransactionManager *tm);
void freeTransactionManager(TransactionManager *tm);

#endif
