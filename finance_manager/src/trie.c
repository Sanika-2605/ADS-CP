#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"

static int charToIndex(char ch)
{
    return tolower((unsigned char)ch) - 'a';
}

TrieNode *createTrieNode(void)
{
    TrieNode *node = (TrieNode *)calloc(1, sizeof(TrieNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed for trie node\n");
        exit(EXIT_FAILURE);
    }
    return node;
}

void insertVendor(TrieNode *root, const char *vendor, const char *category)
{
    int i;
    TrieNode *current = root;

    if (root == NULL || vendor == NULL || category == NULL) {
        return;
    }

    for (i = 0; vendor[i] != '\0'; i++) {
        if (!isalpha((unsigned char)vendor[i])) {
            continue;
        }

        int index = charToIndex(vendor[i]);
        if (index < 0 || index >= ALPHABET_SIZE) {
            continue;
        }

        if (current->children[index] == NULL) {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }

    current->isEndOfWord = 1;
    strncpy(current->category, category, MAX_CATEGORY_LEN - 1);
    current->category[MAX_CATEGORY_LEN - 1] = '\0';
}

const char *searchVendor(TrieNode *root, const char *vendor)
{
    int i;
    TrieNode *current = root;

    if (root == NULL || vendor == NULL) {
        return NULL;
    }

    for (i = 0; vendor[i] != '\0'; i++) {
        if (!isalpha((unsigned char)vendor[i])) {
            continue;
        }

        int index = charToIndex(vendor[i]);
        if (index < 0 || index >= ALPHABET_SIZE) {
            return NULL;
        }

        if (current->children[index] == NULL) {
            return NULL;
        }
        current = current->children[index];
    }

    if (current->isEndOfWord) {
        return current->category;
    }
    return NULL;
}

void freeTrie(TrieNode *root)
{
    int i;
    if (root == NULL) {
        return;
    }

    for (i = 0; i < ALPHABET_SIZE; i++) {
        freeTrie(root->children[i]);
    }
    free(root);
}