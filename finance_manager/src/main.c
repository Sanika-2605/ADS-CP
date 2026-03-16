#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"
#include "avl.h"
#include "heap.h"
#include "segment_tree.h"
#include "tax.h"
#include "transaction.h"

static void clearNewline(char *text)
{
    text[strcspn(text, "\n")] = '\0';
}

static void readString(const char *prompt, char *buffer, int size)
{
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL) {
        clearNewline(buffer);
    }
}

static int readInt(const char *prompt)
{
    char buffer[64];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    return atoi(buffer);
}

static double readDouble(const char *prompt)
{
    char buffer[64];
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0.0;
    }
    return atof(buffer);
}

static void seedTrie(TrieNode *trie)
{
    insertVendor(trie, "amazon", "Shopping");
    insertVendor(trie, "zomato", "Food");
    insertVendor(trie, "uber", "Travel");
    insertVendor(trie, "netflix", "Entertainment");
}

static void printMenu(void)
{
    printf("\n=== Personal Finance Manager ===\n");
    printf("1. Add Transaction\n");
    printf("2. View Transactions\n");
    printf("3. Expense Range Query\n");
    printf("4. Add Portfolio Holding\n");
    printf("5. View Portfolio\n");
    printf("6. Add Bill\n");
    printf("7. View Next Bill\n");
    printf("8. Pay Next Bill\n");
    printf("9. Create Tax Scenario\n");
    printf("10. Clone Tax Scenario and Add Deduction\n");
    printf("11. List Tax Scenarios\n");
    printf("12. Compare Tax Scenarios\n");
    printf("0. Exit\n");
}

int main(void)
{
    int running = 1;
    TrieNode *vendorTrie = createTrieNode();
    AVLNode *portfolioRoot = NULL;
    MinHeap billHeap;
    SegmentTree expenseTree;
    TransactionManager transactionManager;
    TaxManager taxManager;

    initHeap(&billHeap);
    initSegmentTree(&expenseTree, 365);
    initTransactionManager(&transactionManager);
    initTaxManager(&taxManager);
    seedTrie(vendorTrie);

    while (running) {
        int choice;
        printMenu();
        choice = readInt("Enter your choice: ");

        if (choice == 1) {
            char vendor[VENDOR_LEN];
            char category[CATEGORY_LEN];
            char date[DATE_LEN_TXN];
            double amount;

            readString("Enter vendor: ", vendor, sizeof(vendor));
            readString("Enter category (leave empty for auto): ", category, sizeof(category));
            amount = readDouble("Enter amount: ");
            readString("Enter date (YYYY-MM-DD): ", date, sizeof(date));

            if (strlen(category) == 0) {
                strcpy(category, "Misc");
            }

            if (addTransaction(&transactionManager, vendorTrie, &expenseTree, vendor, category, amount, date)) {
                printf("Transaction added successfully.\n");clearerr
            } else {
                printf("Failed to add transaction.\n");
            }

        } else if (choice == 2) {
            printTransactions(&transactionManager);

        } else if (choice == 3) {
            int startDay = readInt("Enter start day (1-365): ");
            int endDay = readInt("Enter end day (1-365): ");
            double total = queryRange(&expenseTree, startDay, endDay);
            printf("Total expenses from day %d to day %d = %.2f\n", startDay, endDay, total);

        } else if (choice == 4) {
            char symbol[SYMBOL_LEN];
            int quantity;
            double price;

            readString("Enter stock symbol: ", symbol, sizeof(symbol));
            quantity = readInt("Enter quantity: ");
            price = readDouble("Enter current price: ");

            portfolioRoot = insertHolding(portfolioRoot, symbol, quantity, price);
            printf("Holding inserted/updated.\n");

        } else if (choice == 5) {
            double total = getTotalPortfolioValue(portfolioRoot);
            if (portfolioRoot == NULL) {
                printf("Portfolio is empty.\n");
            } else {
                printPortfolio(portfolioRoot);
                printf("Total Portfolio Value: %.2f\n", total);
            }

        } else if (choice == 6) {
            char name[BILL_NAME_LEN];
            char dueDate[BILL_DATE_LEN];
            double amount;

            readString("Enter bill name: ", name, sizeof(name));
            amount = readDouble("Enter bill amount: ");
            readString("Enter due date (YYYY-MM-DD): ", dueDate, sizeof(dueDate));

            if (insertBill(&billHeap, name, amount, dueDate)) {
                printf("Bill added.\n");
            } else {
                printf("Bill heap is full.\n");
            }

        } else if (choice == 7) {
            const Bill *nextBill = peekNextBill(&billHeap);
            if (nextBill == NULL) {
                printf("No bills available.\n");
            } else {
                printf("Next Bill: %s | Amount: %.2f | Due Date: %s\n",
                       nextBill->name, nextBill->amount, nextBill->dueDate);
            }

        } else if (choice == 8) {
            Bill paidBill;
            if (extractMin(&billHeap, &paidBill)) {
                printf("Paid Bill: %s | Amount: %.2f | Due Date: %s\n",
                       paidBill.name, paidBill.amount, paidBill.dueDate);
            } else {
                printf("No bills to pay.\n");
            }

        } else if (choice == 9) {
            char name[SCENARIO_NAME_LEN];
            double income;
            char deductionName[DEDUCTION_NAME_LEN];
            double deductionAmount;
            TaxScenario *scenario;

            readString("Enter scenario name: ", name, sizeof(name));
            income = readDouble("Enter income: ");
            scenario = createScenario(&taxManager, name, income);

            if (scenario == NULL) {
                printf("Cannot create more scenarios.\n");
            } else {
                readString("Enter first deduction name: ", deductionName, sizeof(deductionName));
                deductionAmount = readDouble("Enter deduction amount: ");
                addDeduction(&taxManager, scenario->id, deductionName, deductionAmount);
                printf("Scenario created with ID %d.\n", scenario->id);
            }

        } else if (choice == 10) {
            int sourceId;
            char cloneName[SCENARIO_NAME_LEN];
            char deductionName[DEDUCTION_NAME_LEN];
            double deductionAmount;
            TaxScenario *clone;

            sourceId = readInt("Enter source scenario ID: ");
            readString("Enter cloned scenario name: ", cloneName, sizeof(cloneName));
            clone = cloneScenario(&taxManager, sourceId, cloneName);

            if (clone == NULL) {
                printf("Clone failed. Check source ID or scenario limit.\n");
            } else {
                readString("Enter extra deduction name: ", deductionName, sizeof(deductionName));
                deductionAmount = readDouble("Enter extra deduction amount: ");
                addDeduction(&taxManager, clone->id, deductionName, deductionAmount);
                printf("Scenario cloned with new ID %d.\n", clone->id);
            }

        } else if (choice == 11) {
            listScenarios(&taxManager);

        } else if (choice == 12) {
            int a = readInt("Enter first scenario ID: ");
            int b = readInt("Enter second scenario ID: ");
            compareScenarios(&taxManager, a, b);

        } else if (choice == 0) {
            running = 0;

        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    freeTaxManager(&taxManager);
    freePortfolio(portfolioRoot);
    freeTrie(vendorTrie);
    return 0;
}