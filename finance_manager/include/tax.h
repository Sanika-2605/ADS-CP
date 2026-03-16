#ifndef TAX_H
#define TAX_H

#define DEDUCTION_NAME_LEN 50
#define SCENARIO_NAME_LEN 50
#define MAX_TAX_SCENARIOS 20

typedef struct Deduction {
    char name[DEDUCTION_NAME_LEN];
    double amount;
    struct Deduction *next;
} Deduction;

typedef struct TaxScenario {
    int id;
    char name[SCENARIO_NAME_LEN];
    double income;
    Deduction *deductions;
} TaxScenario;

typedef struct {
    TaxScenario *scenarios[MAX_TAX_SCENARIOS];
    int count;
    int nextId;
} TaxManager;

void initTaxManager(TaxManager *manager);
TaxScenario *createScenario(TaxManager *manager, const char *name, double income);
TaxScenario *cloneScenario(TaxManager *manager, int sourceScenarioId, const char *newName);
int addDeduction(TaxManager *manager, int scenarioId, const char *deductionName, double amount);
double totalDeductions(const TaxScenario *scenario);
double calculateTax(const TaxScenario *scenario);
void compareScenarios(TaxManager *manager, int scenarioAId, int scenarioBId);
void listScenarios(const TaxManager *manager);
void freeTaxManager(TaxManager *manager);

#endif
