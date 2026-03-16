#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tax.h"

static Deduction *createDeductionNode(const char *name, double amount)
{
    Deduction *node = (Deduction *)malloc(sizeof(Deduction));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed for deduction\n");
        exit(EXIT_FAILURE);
    }

    strncpy(node->name, name, DEDUCTION_NAME_LEN - 1);
    node->name[DEDUCTION_NAME_LEN - 1] = '\0';
    node->amount = amount;
    node->next = NULL;
    return node;
}

static void freeDeductionList(Deduction *head)
{
    while (head != NULL) {
        Deduction *next = head->next;
        free(head);
        head = next;
    }
}

static Deduction *cloneDeductionList(const Deduction *head)
{
    Deduction *newHead = NULL;
    Deduction *tail = NULL;

    while (head != NULL) {
        Deduction *node = createDeductionNode(head->name, head->amount);
        if (newHead == NULL) {
            newHead = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        head = head->next;
    }

    return newHead;
}

static TaxScenario *findScenario(TaxManager *manager, int scenarioId)
{
    int i;
    for (i = 0; i < manager->count; i++) {
        if (manager->scenarios[i] != NULL && manager->scenarios[i]->id == scenarioId) {
            return manager->scenarios[i];
        }
    }
    return NULL;
}

static const TaxScenario *findScenarioConst(const TaxManager *manager, int scenarioId)
{
    int i;
    for (i = 0; i < manager->count; i++) {
        if (manager->scenarios[i] != NULL && manager->scenarios[i]->id == scenarioId) {
            return manager->scenarios[i];
        }
    }
    return NULL;
}

void initTaxManager(TaxManager *manager)
{
    int i;
    manager->count = 0;
    manager->nextId = 1;
    for (i = 0; i < MAX_TAX_SCENARIOS; i++) {
        manager->scenarios[i] = NULL;
    }
}

TaxScenario *createScenario(TaxManager *manager, const char *name, double income)
{
    TaxScenario *scenario;

    if (manager->count >= MAX_TAX_SCENARIOS) {
        return NULL;
    }

    scenario = (TaxScenario *)malloc(sizeof(TaxScenario));
    if (scenario == NULL) {
        fprintf(stderr, "Memory allocation failed for tax scenario\n");
        exit(EXIT_FAILURE);
    }

    scenario->id = manager->nextId;
    manager->nextId++;

    strncpy(scenario->name, name, SCENARIO_NAME_LEN - 1);
    scenario->name[SCENARIO_NAME_LEN - 1] = '\0';
    scenario->income = income;
    scenario->deductions = NULL;

    manager->scenarios[manager->count] = scenario;
    manager->count++;
    return scenario;
}

TaxScenario *cloneScenario(TaxManager *manager, int sourceScenarioId, const char *newName)
{
    TaxScenario *source = findScenario(manager, sourceScenarioId);
    TaxScenario *clone;

    if (source == NULL || manager->count >= MAX_TAX_SCENARIOS) {
        return NULL;
    }

    clone = (TaxScenario *)malloc(sizeof(TaxScenario));
    if (clone == NULL) {
        fprintf(stderr, "Memory allocation failed while cloning scenario\n");
        exit(EXIT_FAILURE);
    }

    clone->id = manager->nextId;
    manager->nextId++;
    strncpy(clone->name, newName, SCENARIO_NAME_LEN - 1);
    clone->name[SCENARIO_NAME_LEN - 1] = '\0';
    clone->income = source->income;
    clone->deductions = cloneDeductionList(source->deductions);

    manager->scenarios[manager->count] = clone;
    manager->count++;
    return clone;
}

int addDeduction(TaxManager *manager, int scenarioId, const char *deductionName, double amount)
{
    TaxScenario *scenario = findScenario(manager, scenarioId);
    Deduction *node;

    if (scenario == NULL) {
        return 0;
    }

    node = createDeductionNode(deductionName, amount);
    node->next = scenario->deductions;
    scenario->deductions = node;
    return 1;
}

double totalDeductions(const TaxScenario *scenario)
{
    double total = 0.0;
    const Deduction *current = scenario->deductions;

    while (current != NULL) {
        total += current->amount;
        current = current->next;
    }
    return total;
}

double calculateTax(const TaxScenario *scenario)
{
    double taxable = scenario->income - totalDeductions(scenario);
    double tax = 0.0;

    if (taxable <= 0.0) {
        return 0.0;
    }

    if (taxable <= 250000.0) {
        tax = 0.0;
    } else if (taxable <= 500000.0) {
        tax = (taxable - 250000.0) * 0.05;
    } else if (taxable <= 1000000.0) {
        tax = 12500.0 + (taxable - 500000.0) * 0.20;
    } else {
        tax = 112500.0 + (taxable - 1000000.0) * 0.30;
    }

    return tax;
}

void compareScenarios(TaxManager *manager, int scenarioAId, int scenarioBId)
{
    const TaxScenario *a = findScenarioConst(manager, scenarioAId);
    const TaxScenario *b = findScenarioConst(manager, scenarioBId);

    if (a == NULL || b == NULL) {
        printf("One or both scenario IDs are invalid.\n");
        return;
    }

    printf("\nComparison:\n");
    printf("Scenario %d (%s): Income=%.2f Deductions=%.2f Tax=%.2f\n",
           a->id, a->name, a->income, totalDeductions(a), calculateTax(a));
    printf("Scenario %d (%s): Income=%.2f Deductions=%.2f Tax=%.2f\n",
           b->id, b->name, b->income, totalDeductions(b), calculateTax(b));

    if (calculateTax(a) < calculateTax(b)) {
        printf("Scenario %d saves more tax.\n", a->id);
    } else if (calculateTax(a) > calculateTax(b)) {
        printf("Scenario %d saves more tax.\n", b->id);
    } else {
        printf("Both scenarios result in same tax.\n");
    }
}

void listScenarios(const TaxManager *manager)
{
    int i;
    if (manager->count == 0) {
        printf("No tax scenarios available.\n");
        return;
    }

    printf("\nTax Scenarios:\n");
    for (i = 0; i < manager->count; i++) {
        const TaxScenario *scenario = manager->scenarios[i];
        printf("ID: %d | Name: %s | Income: %.2f | Total Deductions: %.2f | Tax: %.2f\n",
               scenario->id,
               scenario->name,
               scenario->income,
               totalDeductions(scenario),
               calculateTax(scenario));
    }
}

void freeTaxManager(TaxManager *manager)
{
    int i;
    for (i = 0; i < manager->count; i++) {
        if (manager->scenarios[i] != NULL) {
            freeDeductionList(manager->scenarios[i]->deductions);
            free(manager->scenarios[i]);
            manager->scenarios[i] = NULL;
        }
    }
    manager->count = 0;
}