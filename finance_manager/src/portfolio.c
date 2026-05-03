#include "portfolio.h"

// --- Red-Black Tree Implementation ---

static RBNode* createNode(PortfolioManager *pm, Holding h) {
    RBNode *node = (RBNode*)malloc(sizeof(RBNode));
    node->data = h;
    node->color = RED;
    node->left = pm->T_Nil;
    node->right = pm->T_Nil;
    node->parent = pm->T_Nil;
    return node;
}

void initPortfolioManager(PortfolioManager *pm) {
    pm->T_Nil = (RBNode*)malloc(sizeof(RBNode));
    pm->T_Nil->color = BLACK;
    pm->T_Nil->left = pm->T_Nil->right = pm->T_Nil->parent = NULL;
    pm->root = pm->T_Nil;
    initHeap(&pm->gainerHeap);
}

static void leftRotate(PortfolioManager *pm, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != pm->T_Nil) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == pm->T_Nil) pm->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

static void rightRotate(PortfolioManager *pm, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != pm->T_Nil) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == pm->T_Nil) pm->root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

static void insertFixup(PortfolioManager *pm, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(pm, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(pm, z->parent->parent);
            }
        } else {
            RBNode *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(pm, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(pm, z->parent->parent);
            }
        }
    }
    pm->root->color = BLACK;
}

void insertHolding(PortfolioManager *pm, const char *symbol, int qty, double buyPrice, double currentPrice) {
    RBNode *existing = searchHolding(pm, symbol);
    if (existing != pm->T_Nil) {
        existing->data.quantity = qty;
        existing->data.buyPrice = buyPrice;
        existing->data.currentPrice = currentPrice;
        existing->data.gainPct = ((currentPrice - buyPrice) / buyPrice) * 100.0;
        return;
    }

    Holding h;
    strncpy(h.symbol, symbol, SYMBOL_LEN - 1);
    h.quantity = qty;
    h.buyPrice = buyPrice;
    h.currentPrice = currentPrice;
    h.gainPct = ((currentPrice - buyPrice) / buyPrice) * 100.0;

    RBNode *z = createNode(pm, h);
    RBNode *y = pm->T_Nil;
    RBNode *x = pm->root;

    while (x != pm->T_Nil) {
        y = x;
        if (strcmp(z->data.symbol, x->data.symbol) < 0) x = x->left;
        else x = x->right;
    }

    z->parent = y;
    if (y == pm->T_Nil) pm->root = z;
    else if (strcmp(z->data.symbol, y->data.symbol) < 0) y->left = z;
    else y->right = z;

    insertFixup(pm, z);
}

RBNode* searchHolding(PortfolioManager *pm, const char *symbol) {
    RBNode *curr = pm->root;
    while (curr != pm->T_Nil && strcmp(symbol, curr->data.symbol) != 0) {
        if (strcmp(symbol, curr->data.symbol) < 0) curr = curr->left;
        else curr = curr->right;
    }
    return curr;
}

void printPortfolioInOrder(PortfolioManager *pm, RBNode *node) {
    if (node == pm->T_Nil) return;
    printPortfolioInOrder(pm, node->left);
    printf("%-10s %-10d %-10.2f %-10.2f %-10.2f%%\n",
           node->data.symbol, node->data.quantity, node->data.buyPrice,
           node->data.currentPrice, node->data.gainPct);
    printPortfolioInOrder(pm, node->right);
}

void freePortfolio(PortfolioManager *pm, RBNode *node) {
    if (node == pm->T_Nil) return;
    freePortfolio(pm, node->left);
    freePortfolio(pm, node->right);
    free(node);
}

// --- Heap Implementation ---

void initHeap(MaxHeap *h) {
    h->size = 0;
}

void pushHeap(MaxHeap *h, Holding *hld) {
    if (h->size >= MAX_HEAP_SIZE) return;
    int i = h->size++;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (hld->gainPct <= h->elements[p]->gainPct) break;
        h->elements[i] = h->elements[p];
        i = p;
    }
    h->elements[i] = hld;
}

Holding* popHeap(MaxHeap *h) {
    if (h->size == 0) return NULL;
    Holding *root = h->elements[0];
    Holding *last = h->elements[--h->size];
    int i = 0;
    while (i * 2 + 1 < h->size) {
        int child = i * 2 + 1;
        if (child + 1 < h->size && h->elements[child + 1]->gainPct > h->elements[child]->gainPct) {
            child++;
        }
        if (last->gainPct >= h->elements[child]->gainPct) break;
        h->elements[i] = h->elements[child];
        i = child;
    }
    h->elements[i] = last;
    return root;
}

void buildGainerHeap(PortfolioManager *pm, RBNode *node) {
    if (node == pm->T_Nil) return;
    pushHeap(&pm->gainerHeap, &node->data);
    buildGainerHeap(pm, node->left);
    buildGainerHeap(pm, node->right);
}
