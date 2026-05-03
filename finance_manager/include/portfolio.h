#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYMBOL_LEN 10

typedef enum { RED, BLACK } Color;

// Holding Data
typedef struct {
    char symbol[SYMBOL_LEN];
    int quantity;
    double buyPrice;
    double currentPrice;
    double gainPct;
} Holding;

// Red-Black Tree Node
typedef struct RBNode {
    Holding data;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

// Heap Structure for Top Gainers
#define MAX_HEAP_SIZE 100
typedef struct {
    Holding *elements[MAX_HEAP_SIZE];
    int size;
} MaxHeap;

// Portfolio Manager
typedef struct {
    RBNode *root;
    RBNode *T_Nil; // Sentinel node for RBT
    MaxHeap gainerHeap;
} PortfolioManager;

// Function Prototypes - RBT
void initPortfolioManager(PortfolioManager *pm);
void insertHolding(PortfolioManager *pm, const char *symbol, int qty, double buyPrice, double currentPrice);
RBNode* searchHolding(PortfolioManager *pm, const char *symbol);
void printPortfolioInOrder(PortfolioManager *pm, RBNode *node);
void freePortfolio(PortfolioManager *pm, RBNode *node);

// Function Prototypes - Heap
void initHeap(MaxHeap *h);
void pushHeap(MaxHeap *h, Holding *hld);
Holding* popHeap(MaxHeap *h);
void buildGainerHeap(PortfolioManager *pm, RBNode *node);

#endif
