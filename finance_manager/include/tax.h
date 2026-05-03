#ifndef TAX_H
#define TAX_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_SLABS 10
#define MEMO_SIZE 1000 // Cache for income levels (in units of thousands)

typedef struct {
    double limit;
    double rate;
} TaxSlab;

typedef struct {
    TaxSlab slabs[MAX_SLABS];
    int slabCount;
    double memo[MEMO_SIZE];
} TaxCalculator;

void initTaxCalculator(TaxCalculator *tc);
void addTaxSlab(TaxCalculator *tc, double limit, double rate);
double calculateTaxDP(TaxCalculator *tc, double income);
void clearMemo(TaxCalculator *tc);

#endif
