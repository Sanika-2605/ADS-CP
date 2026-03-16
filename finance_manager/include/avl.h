#ifndef AVL_H
#define AVL_H

#define SYMBOL_LEN 20

typedef struct AVLNode {
    char symbol[SYMBOL_LEN];
    int quantity;
    double price;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

AVLNode *insertHolding(AVLNode *root, const char *symbol, int quantity, double price);
void printPortfolio(AVLNode *root);
double getTotalPortfolioValue(AVLNode *root);
void freePortfolio(AVLNode *root);

#endif
