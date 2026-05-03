#include "transaction.h"
#include <ctype.h>

// --- Trie Implementation ---

TrieNode* createTrieNode() {
    TrieNode *node = (TrieNode*)malloc(sizeof(TrieNode));
    if (node) {
        node->isEndOfWord = 0;
        node->category[0] = '\0';
        for (int i = 0; i < MAX_CHAR; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

static int charToIndex(char c) {
    return tolower(c) - 'a';
}

void insertVendor(TrieNode *root, const char *vendor, const char *category) {
    TrieNode *curr = root;
    for (int i = 0; vendor[i] != '\0'; i++) {
        if (!isalpha(vendor[i])) continue; // Simple rule: only alpha for mapping
        int index = charToIndex(vendor[i]);
        if (!curr->children[index]) {
            curr->children[index] = createTrieNode();
        }
        curr = curr->children[index];
    }
    curr->isEndOfWord = 1;
    strncpy(curr->category, category, CATEGORY_LEN - 1);
}

const char* searchCategory(TrieNode *root, const char *vendor) {
    TrieNode *curr = root;
    for (int i = 0; vendor[i] != '\0'; i++) {
        if (!isalpha(vendor[i])) continue;
        int index = charToIndex(vendor[i]);
        if (!curr->children[index]) {
            return NULL;
        }
        curr = curr->children[index];
    }
    if (curr != NULL && curr->isEndOfWord) {
        return curr->category;
    }
    return NULL;
}

void freeTrie(TrieNode *root) {
    if (!root) return;
    for (int i = 0; i < MAX_CHAR; i++) {
        freeTrie(root->children[i]);
    }
    free(root);
}

// --- Segment Tree Implementation ---

void initSegmentTree(SegmentTree *st) {
    for (int i = 0; i < 4 * DAYS_IN_YEAR; i++) {
        st->tree[i] = 0.0;
    }
}

void updateExpense(SegmentTree *st, int day, double amount, int node, int start, int end) {
    if (start == end) {
        st->tree[node] += amount;
        return;
    }
    int mid = (start + end) / 2;
    if (day <= mid)
        updateExpense(st, day, amount, 2 * node, start, mid);
    else
        updateExpense(st, day, amount, 2 * node + 1, mid + 1, end);
    st->tree[node] = st->tree[2 * node] + st->tree[2 * node + 1];
}

double queryExpense(SegmentTree *st, int L, int R, int node, int start, int end) {
    if (R < start || end < L) {
        return 0.0;
    }
    if (L <= start && end <= R) {
        return st->tree[node];
    }
    int mid = (start + end) / 2;
    return queryExpense(st, L, R, 2 * node, start, mid) +
           queryExpense(st, L, R, 2 * node + 1, mid + 1, end);
}

// --- Transaction Manager Implementation ---

void initTransactionManager(TransactionManager *tm) {
    tm->count = 0;
    tm->capacity = 10;
    tm->transactions = (Transaction*)malloc(sizeof(Transaction) * tm->capacity);
    tm->vendorTrie = createTrieNode();
    initSegmentTree(&tm->expenseTree);

    // Seed some categories
    insertVendor(tm->vendorTrie, "Amazon", "Shopping");
    insertVendor(tm->vendorTrie, "Zomato", "Food");
    insertVendor(tm->vendorTrie, "Swiggy", "Food");
    insertVendor(tm->vendorTrie, "Uber", "Transport");
    insertVendor(tm->vendorTrie, "Netflix", "Entertainment");
}

void addTransaction(TransactionManager *tm, const char *vendor, const char *category, double amount, int day) {
    if (tm->count == tm->capacity) {
        tm->capacity *= 2;
        tm->transactions = (Transaction*)realloc(tm->transactions, sizeof(Transaction) * tm->capacity);
    }

    Transaction *t = &tm->transactions[tm->count++];
    strncpy(t->vendor, vendor, VENDOR_LEN - 1);
    t->amount = amount;
    t->dayOfYear = day;

    // Use Trie for auto-categorization if category is empty
    if (category == NULL || strlen(category) == 0) {
        const char *matched = searchCategory(tm->vendorTrie, vendor);
        if (matched) {
            strncpy(t->category, matched, CATEGORY_LEN - 1);
        } else {
            strcpy(t->category, "Uncategorized");
        }
    } else {
        strncpy(t->category, category, CATEGORY_LEN - 1);
    }

    // Update Segment Tree
    updateExpense(&tm->expenseTree, day, amount, 1, 1, DAYS_IN_YEAR);
}

void printTransactions(TransactionManager *tm) {
    printf("\n--- Transactions History ---\n");
    printf("%-20s %-15s %-10s %-5s\n", "Vendor", "Category", "Amount", "Day");
    for (int i = 0; i < tm->count; i++) {
        Transaction *t = &tm->transactions[i];
        printf("%-20s %-15s %-10.2f %-5d\n", t->vendor, t->category, t->amount, t->dayOfYear);
    }
}

void freeTransactionManager(TransactionManager *tm) {
    free(tm->transactions);
    freeTrie(tm->vendorTrie);
}