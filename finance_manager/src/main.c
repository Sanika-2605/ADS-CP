#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "transaction.h"
#include "portfolio.h"
#include "bills.h"
#include "tax.h"

// Helper to clear input buffer
void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void printMenu() {
    printf("\n========================================\n");
    printf("   PERSONAL FINANCE & PORTFOLIO MGR\n");
    printf("========================================\n");
    printf("1. Transactions (Add / View / Range Sum)\n");
    printf("2. Portfolio (Add / View / Top Gainers)\n");
    printf("3. Bills (Add / View / Pay Next)\n");
    printf("4. Tax (Calculate / Scenarios)\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

void handleTransactions(TransactionManager *tm) {
    int choice;
    printf("\n--- Transaction Menu ---\n");
    printf("1. Add Transaction\n");
    printf("2. View History\n");
    printf("3. Range Sum Query (Segment Tree)\n");
    printf("Choice: ");
    scanf("%d", &choice);
    clearBuffer();

    if (choice == 1) {
        char vendor[VENDOR_LEN], category[CATEGORY_LEN];
        double amount;
        int day;
        printf("Vendor: "); fgets(vendor, VENDOR_LEN, stdin); vendor[strcspn(vendor, "\n")] = 0;
        printf("Category (empty for auto): "); fgets(category, CATEGORY_LEN, stdin); category[strcspn(category, "\n")] = 0;
        printf("Amount: "); scanf("%lf", &amount);
        printf("Day of Year (1-366): "); scanf("%d", &day);
        addTransaction(tm, vendor, category, amount, day);
        printf("Added successfully!\n");
    } else if (choice == 2) {
        printTransactions(tm);
    } else if (choice == 3) {
        int L, R;
        printf("Start Day: "); scanf("%d", &L);
        printf("End Day: "); scanf("%d", &R);
        double sum = queryExpense(&tm->expenseTree, L, R, 1, 1, DAYS_IN_YEAR);
        printf("Total Expense from day %d to %d: %.2f\n", L, R, sum);
    }
}

void handlePortfolio(PortfolioManager *pm) {
    int choice;
    printf("\n--- Portfolio Menu ---\n");
    printf("1. Add/Update Holding (RB-Tree)\n");
    printf("2. View Portfolio (In-order)\n");
    printf("3. Show Top Gainer (Max Heap)\n");
    printf("Choice: ");
    scanf("%d", &choice);
    clearBuffer();

    if (choice == 1) {
        char symbol[SYMBOL_LEN];
        int qty;
        double buy, curr;
        printf("Symbol: "); fgets(symbol, SYMBOL_LEN, stdin); symbol[strcspn(symbol, "\n")] = 0;
        printf("Quantity: "); scanf("%d", &qty);
        printf("Buy Price: "); scanf("%lf", &buy);
        printf("Current Price: "); scanf("%lf", &curr);
        insertHolding(pm, symbol, qty, buy, curr);
        printf("Portfolio updated!\n");
    } else if (choice == 2) {
        printf("\n%-10s %-10s %-10s %-10s %-10s\n", "Symbol", "Qty", "Buy", "Current", "Gain%");
        printPortfolioInOrder(pm, pm->root);
    } else if (choice == 3) {
        initHeap(&pm->gainerHeap);
        buildGainerHeap(pm, pm->root);
        Holding *top = popHeap(&pm->gainerHeap);
        if (top) {
            printf("Top Gainer: %s with %.2f%% gain\n", top->symbol, top->gainPct);
        } else {
            printf("Portfolio empty.\n");
        }
    }
}

void handleBills(BillManager *bm) {
    int choice;
    printf("\n--- Bills Menu ---\n");
    printf("1. Add Bill (Min Heap)\n");
    printf("2. View All Bills\n");
    printf("3. Pay Next Due Bill\n");
    printf("Choice: ");
    scanf("%d", &choice);
    clearBuffer();

    if (choice == 1) {
        char name[BILL_NAME_LEN], date[BILL_DATE_LEN];
        double amount;
        printf("Name: "); fgets(name, BILL_NAME_LEN, stdin); name[strcspn(name, "\n")] = 0;
        printf("Amount: "); scanf("%lf", &amount);
        printf("Due Date (YYYY-MM-DD): "); scanf("%s", date);
        addBill(bm, name, amount, date);
    } else if (choice == 2) {
        printAllBills(bm);
    } else if (choice == 3) {
        if (bm->size > 0) {
            Bill b = popNextBill(bm);
            printf("Paid %s: %.2f (Due: %s)\n", b.name, b.amount, b.dueDate);
        } else {
            printf("No bills to pay.\n");
        }
    }
}

void handleTax(TaxCalculator *tc) {
    int choice;
    printf("\n--- Tax Menu ---\n");
    printf("1. Add Tax Slab\n");
    printf("2. Calculate Tax (Memoized DP)\n");
    printf("3. Clear Cache\n");
    printf("Choice: ");
    scanf("%d", &choice);
    clearBuffer();

    if (choice == 1) {
        double limit, rate;
        printf("Limit: "); scanf("%lf", &limit);
        printf("Rate (0.0 - 1.0): "); scanf("%lf", &rate);
        addTaxSlab(tc, limit, rate);
    } else if (choice == 2) {
        double income;
        printf("Income: "); scanf("%lf", &income);
        double tax = calculateTaxDP(tc, income);
        printf("Estimated Tax: %.2f\n", tax);
    } else if (choice == 3) {
        clearMemo(tc);
        printf("Cache cleared.\n");
    }
}

int main() {
    TransactionManager tm;
    PortfolioManager pm;
    BillManager bm;
    TaxCalculator tc;

    initTransactionManager(&tm);
    initPortfolioManager(&pm);
    initBillManager(&bm);
    initTaxCalculator(&tc);

    // Default Slabs
    addTaxSlab(&tc, 250000, 0.0);
    addTaxSlab(&tc, 500000, 0.05);
    addTaxSlab(&tc, 1000000, 0.20);
    addTaxSlab(&tc, 10000000, 0.30);

    int choice;
    while(1) {
        printMenu();
        if (scanf("%d", &choice) != 1) break;
        clearBuffer();

        if (choice == 0) break;
        switch(choice) {
            case 1: handleTransactions(&tm); break;
            case 2: handlePortfolio(&pm); break;
            case 3: handleBills(&bm); break;
            case 4: handleTax(&tc); break;
            default: printf("Invalid choice.\n");
        }
    }

    // Cleanup
    freeTransactionManager(&tm);
    freePortfolio(&pm, pm.root);
    free(pm.T_Nil);
    printf("Goodbye!\n");

    return 0;
}