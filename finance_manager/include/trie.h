#ifndef TRIE_H
#define TRIE_H

#define ALPHABET_SIZE 26
#define MAX_CATEGORY_LEN 50

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    char category[MAX_CATEGORY_LEN];
} TrieNode;

TrieNode *createTrieNode(void);
void insertVendor(TrieNode *root, const char *vendor, const char *category);
const char *searchVendor(TrieNode *root, const char *vendor);
void freeTrie(TrieNode *root);

#endif
