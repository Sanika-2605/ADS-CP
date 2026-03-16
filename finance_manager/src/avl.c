#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

static int maxInt(int a, int b)
{
    return (a > b) ? a : b;
}

static int nodeHeight(AVLNode *node)
{
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

static AVLNode *createNode(const char *symbol, int quantity, double price)
{
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed for AVL node\n");
        exit(EXIT_FAILURE);
    }

    strncpy(node->symbol, symbol, SYMBOL_LEN - 1);
    node->symbol[SYMBOL_LEN - 1] = '\0';
    node->quantity = quantity;
    node->price = price;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static int getBalance(AVLNode *node)
{
    if (node == NULL) {
        return 0;
    }
    return nodeHeight(node->left) - nodeHeight(node->right);
}

static AVLNode *rightRotate(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = maxInt(nodeHeight(y->left), nodeHeight(y->right)) + 1;
    x->height = maxInt(nodeHeight(x->left), nodeHeight(x->right)) + 1;

    return x;
}

static AVLNode *leftRotate(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = maxInt(nodeHeight(x->left), nodeHeight(x->right)) + 1;
    y->height = maxInt(nodeHeight(y->left), nodeHeight(y->right)) + 1;

    return y;
}

AVLNode *insertHolding(AVLNode *root, const char *symbol, int quantity, double price)
{
    int compare;

    if (root == NULL) {
        return createNode(symbol, quantity, price);
    }

    compare = strcmp(symbol, root->symbol);

    if (compare < 0) {
        root->left = insertHolding(root->left, symbol, quantity, price);
    } else if (compare > 0) {
        root->right = insertHolding(root->right, symbol, quantity, price);
    } else {
        root->quantity += quantity;
        root->price = price;
        return root;
    }

    root->height = 1 + maxInt(nodeHeight(root->left), nodeHeight(root->right));

    int balance = getBalance(root);

    if (balance > 1 && strcmp(symbol, root->left->symbol) < 0) {
        return rightRotate(root);
    }

    if (balance < -1 && strcmp(symbol, root->right->symbol) > 0) {
        return leftRotate(root);
    }

    if (balance > 1 && strcmp(symbol, root->left->symbol) > 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && strcmp(symbol, root->right->symbol) < 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void printPortfolio(AVLNode *root)
{
    if (root == NULL) {
        return;
    }

    printPortfolio(root->left);
    printf("Symbol: %-10s Quantity: %-5d Price: %-10.2f Value: %.2f\n",
           root->symbol,
           root->quantity,
           root->price,
           root->quantity * root->price);
    printPortfolio(root->right);
}

double getTotalPortfolioValue(AVLNode *root)
{
    if (root == NULL) {
        return 0.0;
    }

    return (root->quantity * root->price)
        + getTotalPortfolioValue(root->left)
        + getTotalPortfolioValue(root->right);
}

void freePortfolio(AVLNode *root)
{
    if (root == NULL) {
        return;
    }

    freePortfolio(root->left);
    freePortfolio(root->right);
    free(root);
}