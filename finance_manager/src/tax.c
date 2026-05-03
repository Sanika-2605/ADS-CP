#include "tax.h"

void initTaxCalculator(TaxCalculator *tc) {
    tc->slabCount = 0;
    clearMemo(tc);
}

void addTaxSlab(TaxCalculator *tc, double limit, double rate) {
    if (tc->slabCount < MAX_SLABS) {
        tc->slabs[tc->slabCount].limit = limit;
        tc->slabs[tc->slabCount].rate = rate;
        tc->slabCount++;
    }
}

void clearMemo(TaxCalculator *tc) {
    for (int i = 0; i < MEMO_SIZE; i++) {
        tc->memo[i] = -1.0;
    }
}

static double calculateTaxRecursive(TaxCalculator *tc, double income) {
    double totalTax = 0;
    double prevLimit = 0;

    for (int i = 0; i < tc->slabCount; i++) {
        if (income > tc->slabs[i].limit) {
            totalTax += (tc->slabs[i].limit - prevLimit) * tc->slabs[i].rate;
            prevLimit = tc->slabs[i].limit;
        } else {
            totalTax += (income - prevLimit) * tc->slabs[i].rate;
            prevLimit = income;
            break;
        }
    }

    // If there's remaining income above the last limit, handle it (if last slab is an open limit)
    // In our case, we'll assume the last slab rate applies to everything above.
    if (income > prevLimit && tc->slabCount > 0) {
        totalTax += (income - prevLimit) * tc->slabs[tc->slabCount-1].rate;
    }

    return totalTax;
}

double calculateTaxDP(TaxCalculator *tc, double income) {
    int memoIdx = (int)(income / 1000.0);
    
    // Check bounds for memoization
    if (memoIdx < 0 || memoIdx >= MEMO_SIZE) {
        return calculateTaxRecursive(tc, income);
    }

    // Return cached value if exists
    if (tc->memo[memoIdx] != -1.0) {
        printf("[DP] Cache Hit for income level %dk\n", memoIdx);
        return tc->memo[memoIdx];
    }

    // Calculate and store in memo
    double result = calculateTaxRecursive(tc, income);
    tc->memo[memoIdx] = result;
    return result;
}