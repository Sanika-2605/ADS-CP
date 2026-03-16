# ADS-CP

Personal Finance & Investment Portfolio Manager built in C.

## Overview

This project is a menu-driven CLI application that helps you track personal finances and simulate tax outcomes. It uses multiple data structures to support different modules efficiently.

## Features

- **Transaction tracking** with vendor and category support
- **Auto-category hints** for known vendors using a Trie
- **Expense range queries** (day-based) using a Segment Tree
- **Portfolio management** (insert/update holdings, total value) using AVL Tree
- **Bill scheduling** and payment priority using Min Heap
- **Tax scenario planning** with cloning and deduction comparison

## Data Structures Used

- **Trie**: vendor/category lookup
- **AVL Tree**: balanced stock holdings portfolio
- **Min Heap**: next due bill retrieval
- **Segment Tree**: expense total over date ranges
- **Array-backed manager structures**: transactions and tax scenarios

## Build & Run

### Option 1: Using Make

From the `finance_manager` folder:

```bash
make
./finance_manager
```

On Windows (PowerShell):

```powershell
make
.\finance_manager.exe
```

### Option 2: Direct GCC command

From the `finance_manager` folder:

```bash
gcc -Wall -Wextra -Iinclude src/*.c -o finance_manager
./finance_manager
```

Windows (PowerShell):

```powershell
gcc -Wall -Wextra -Iinclude src/*.c -o finance_manager.exe
.\finance_manager.exe
```

## Clean Build Artifacts

From `finance_manager`:

```bash
make clean
```

## Project Structure

```text
finance_manager/
	include/
		avl.h
		heap.h
		segment_tree.h
		tax.h
		transaction.h
		trie.h
	src/
		avl.c
		heap.c
		main.c
		segment_tree.c
		tax.c
		transaction.c
		trie.c
	Makefile
```

## Main Menu Modules

The application currently supports:

1. Add Transaction
2. View Transactions
3. Expense Range Query
4. Add Portfolio Holding
5. View Portfolio
6. Add Bill
7. View Next Bill
8. Pay Next Bill
9. Create Tax Scenario
10. Clone Tax Scenario and Add Deduction
11. List Tax Scenarios
12. Compare Tax Scenarios

## Requirements

- C compiler (`gcc` recommended)
- `make` (optional but recommended)
- Terminal/PowerShell

## Project Workflow

1. **Initialize core managers** in `main.c`:
	 - Trie for vendor/category suggestions
	 - AVL for portfolio holdings
	 - Min Heap for bills
	 - Segment Tree for expense range queries
	 - Transaction and Tax managers
2. **Read user choice** from menu (1 to 12).
3. **Route to module logic** based on selected operation.
4. **Update corresponding data structure** (insert/query/clone/extract).
5. **Print result to CLI** and return to menu loop.
6. **Cleanup memory** before exiting.

## File-wise Algorithms / Data Structures

### `src/main.c`
- Menu-driven controller logic
- Input parsing and operation dispatch

### `src/trie.c` + `include/trie.h`
- **Trie** operations:
	- vendor insertion
	- vendor search for auto-category support
	- memory cleanup (recursive free)

### `src/avl.c` + `include/avl.h`
- **AVL Tree** algorithms:
	- height/balance tracking
	- left/right rotations
	- balanced insert/update for holdings
	- in-order traversal for portfolio display

### `src/heap.c` + `include/heap.h`
- **Min Heap** algorithms:
	- heapify up/down
	- insert bill
	- extract minimum due bill
	- peek next bill

### `src/segment_tree.c` + `include/segment_tree.h`
- **Segment Tree** algorithms:
	- point update for daily expense
	- range sum query for day intervals

### `src/transaction.c` + `include/transaction.h`
- Transaction manager logic:
	- add transaction records
	- integrate trie category lookup
	- update segment tree totals
	- transaction listing

### `src/tax.c` + `include/tax.h`
- Tax scenario management:
	- create scenario
	- add deductions
	- clone scenario (copy + modify)
	- compare scenario outputs

## Suggested Work Split for 5 Members

Use this as a direct team allocation plan:

### Member 1 — Core CLI & Integration
- Own `src/main.c`
- Integrate all modules and final flow testing
- Handle input validation and menu improvements

### Member 2 — Transaction + Trie Module
- Own `src/transaction.c`, `src/trie.c`, `include/transaction.h`, `include/trie.h`
- Improve vendor/category intelligence and transaction UX

### Member 3 — Portfolio (AVL) Module
- Own `src/avl.c`, `include/avl.h`
- Add/update portfolio operations and valuation correctness tests

### Member 4 — Bills + Expense Queries
- Own `src/heap.c`, `src/segment_tree.c`, `include/heap.h`, `include/segment_tree.h`
- Validate due-date priority and expense range-query correctness

### Member 5 — Tax + Build/Docs/QA
- Own `src/tax.c`, `include/tax.h`, `Makefile`, `README.md`
- Maintain build reliability, documentation quality, and final test checklist

### Team Delivery Workflow (Recommended)
- Each member works in a feature branch.
- Merge module-level PRs after peer review.
- Run `make` and a full menu smoke test before final merge.
