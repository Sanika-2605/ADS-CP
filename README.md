# Personal Finance & Portfolio Manager

A modular CLI-based Personal Finance & Investment Portfolio Manager written in pure **C (C99)**, built entirely with custom-implemented data structures — no external libraries beyond `stdio.h`, `stdlib.h`, and `string.h`.

---

## Overview

This is a menu-driven terminal application that helps you manage personal finances, track investments, schedule bills, and plan taxes — all powered by core data structures implemented from scratch.

---

## Data Structures Used

| Module | Data Structure | Purpose |
|---|---|---|
| `transaction` | **Trie** | Vendor-to-category auto-matching |
| `transaction` | **Segment Tree** | Range sum queries on daily expenses |
| `portfolio` | **Red-Black Tree** | Balanced storage of stock holdings |
| `portfolio` | **Max Heap** | Tracking top gainers by % gain |
| `bills` | **Min Heap** | Priority queue for upcoming bills |
| `tax` | **DP + Memoization** | Efficient multi-slab tax calculation |

---

## Project Structure

```
ADS-CP/
└── finance_manager/
    ├── Makefile
    ├── include/
    │   ├── transaction.h   ← Trie + Segment Tree definitions
    │   ├── portfolio.h     ← Red-Black Tree + Max Heap definitions
    │   ├── bills.h         ← Min Heap (Bill Priority Queue)
    │   └── tax.h           ← DP Tax Calculator
    └── src/
        ├── main.c          ← Menu-driven integration
        ├── transaction.c   ← Trie + Segment Tree implementation
        ├── portfolio.c     ← RBT + Heap implementation
        ├── bills.c         ← Min Heap implementation
        └── tax.c           ← DP with memoization
```

---

## Build & Run

### Option 1: Direct GCC (Recommended)

Navigate to the `finance_manager` folder:

```bash
gcc -Wall -Iinclude src/main.c src/transaction.c src/portfolio.c src/bills.c src/tax.c -o finance_manager
./finance_manager
```

On **Windows (PowerShell)**:

```powershell
gcc -Wall -Iinclude src/main.c src/transaction.c src/portfolio.c src/bills.c src/tax.c -o finance_manager.exe
.\finance_manager.exe
```

### Option 2: Using Make

```bash
make
./finance_manager
```

Windows:

```powershell
make
.\finance_manager.exe
```

### Clean Build Artifacts

```bash
make clean
```

---

## Menu Structure

```
========================================
   PERSONAL FINANCE & PORTFOLIO MGR
========================================
1. Transactions (Add / View / Range Sum)
2. Portfolio    (Add / View / Top Gainers)
3. Bills        (Add / View / Pay Next)
4. Tax          (Calculate / Scenarios)
0. Exit
```

---

## Module Details

### `transaction.c` — Trie + Segment Tree

- **Trie**: Stores vendor→category mappings. When you add a transaction for "Amazon", it auto-tags it as "Shopping".  
  Pre-seeded vendors: `Amazon`, `Zomato`, `Swiggy`, `Uber`, `Netflix`
- **Segment Tree**: Tracks daily expenses (indexed by day of year, 1–366) and supports range sum queries in O(log N).

### `portfolio.c` — Red-Black Tree + Max Heap

- **Red-Black Tree**: Stores stock holdings sorted by symbol. Uses standard rotations and recoloring to stay balanced, guaranteeing O(log N) insert/search.
- **Max Heap**: Built dynamically from the RBT to quickly surface the top-performing stock by gain percentage.

### `bills.c` — Min Heap

- Implements a **Priority Queue** where the bill with the earliest due date is always at heap root.
- Supports: add bill, peek next bill, pay (extract) next bill.
- Date comparison is done lexicographically on YYYY-MM-DD strings.

### `tax.c` — Dynamic Programming with Memoization

- Calculates tax using **bracketed slabs** (e.g., Indian income tax structure).
- Results are **memoized** by income level (in 1000-unit buckets) to avoid recomputation.
- Default slabs seeded at startup:

  | Income Range | Rate |
  |---|---|
  | Up to ₹2,50,000 | 0% |
  | ₹2,50,001 – ₹5,00,000 | 5% |
  | ₹5,00,001 – ₹10,00,000 | 20% |
  | Above ₹10,00,000 | 30% |

---

## Sample Run

```
Choice: 1             ← Transactions
Choice: 1             ← Add Transaction
Vendor: Amazon
Category: (empty)     ← auto-categorized as "Shopping"
Amount: 500
Day: 42

Choice: 3             ← Range Sum Query
Start Day: 1
End Day: 60
Total Expense: 500.00

Choice: 2             ← Portfolio
Choice: 1             ← Add Holding
Symbol: AAPL
Buy Price: 150  Current: 160
→ Gain: 6.67%

Choice: 4             ← Tax
Choice: 2             ← Calculate
Income: 600000
→ Estimated Tax: 32500.00
```

---

## Requirements

- `gcc` (GCC 9+ recommended)
- `make` (optional)
- Terminal or PowerShell

---

## Suggested Work Split (5 Members)

| Member | Owns |
|---|---|
| Member 1 | `transaction.c/.h` — Trie |
| Member 2 | `transaction.c/.h` — Segment Tree |
| Member 3 | `portfolio.c/.h` — Red-Black Tree + Max Heap |
| Member 4 | `bills.c/.h` — Min Heap Priority Queue |
| Member 5 | `tax.c/.h` — DP Tax |

> Each member works in a feature branch. Merge after peer review. Run a full menu smoke test before the final merge.
