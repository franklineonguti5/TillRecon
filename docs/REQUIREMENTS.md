# TillRecon
## Software Requirements Specification

Version: 1.0
Status: Approved for Development

---

## 1. Project Overview

TillRecon is a financial reconcilliation and revenue
assurance system designed for multi-branch retail businesses.

The system compares financial transactions recorded
by point-of-sale (POS) systems against payment-provider
settlement records.

Its purpose is to identify financial discrepancies,
prevent duplicate reconciliations and provide
traceable financial reporting.

## 2. Business Problem

Retail businesses process transactions through
multiple payment Channels.

These include:

- Cash
- M-pesa
- Bank transfers
- Debit cards
- Credit cards

Differences between recorded sales and received
payments create financial risks.

Manual reconciliation is expensive, slow and
susceptible to human error.

TillRecon automates this process.

## 3. Primary Objectives

The system must:

1. Import financial transaction records.
2. Validate transaction data.
3. Detect duplicate records.
4. Match POS transactions against settlements.
5. Identify missing settlements.
6. Identify partial payments.
7. Identify overpayments and underpayments.
8. Handle refunds and reversals.
9. Support multiple retail branches.
10. Generate reconciliation reports.
11. Preserve financial audit history.
12. Maintain data integrity.

## 4. Transaction Sources

The initial system supports:

### POS Transactions

Records originating from retail point-of-sale systems.

### Payment Settlrments

Records originating from payment providers,
banks and financial institutions.

## 5. Matching Requirements

The reconciliation engine must support:

### Exact Matching

Match transactions using payment references
even when internal POS identifiers differ.

### Tolerance Matching

Support configurable date and amount tolerances.

### Duplication Detection

Identify multiple settlrmrnt records associated
with the same financial transaction.

### Partial Settlement Detection

Identify transactios where the received amount
is lower than the expected amount.

### Overpayment Detection

Identify settlements exceeding expected amounts.

## 6. Exception Categories

The initial system must recognize:

- MATCHED
- UNMATCHED_POS
- UNMATCHED_SETTLEMENT
- DUPLICATE
- UNDERPAID
- OVERPAID
- PARTIALLY_SETTLED
- INVALID_RECORD
- REVIEW_REQUIRED

## 7. Financial Integrity

All financial calculations must use integer
minor curency units.

Floasting-point arithmetic must not be used
for financial amounts.

For Kenyan Shillings, amounts will be represented
in cents.

Example:

KES 1,250.75 = 125075 cents.

Every reconciliation result must be traceable
to its original source records.

## 8. Data Validation

The system must validate:

- Transaction identifiers
- Branch identifiers
- Transaction dates
- Curency codes
- Transaction amounts
- Pyment Methods
- Reference numbers
- Transaction status

Invalid records must not silently enter
the reconciliation process.

## 9. Performance Requirements

The system must be designed to process
large transaction datasets efficiently.

Initial performance target:

100,000 transaction records in a controlled
benchmark environment.

This is a target to measure, not an assumed result.

## 10.  Security Requirements

The system must implement:

- Input validation
- Parameterized database queries
- Safe file handling
- Audit logging
- Database transaction management
- Protection aagainst duplicate imports
- Secure configuration management

## 11. Reporting Requirements

The system must produce:

- Daily reconciliation summaries
- Branch-level reconciliation reports
- Unmatched transaction reports
- Duplicate transaction reports
- Financial discepancy reports
- Settlement summaries

## 12. Technology Stack

Core language: C++20

Build system: CMakes

Local database: SQLite

Production database: PostgreSQL

Testing: GoogleTest

Version control: Git

## 13. Acceptance Criteria

The project is considered complete only when:

1. Transactions can be imported.
2. Invalid records are handled correctly.
3. Duplicate records are detected.
4. Reconciliation results are accurate.
5. Refunds and reversals are supported.
6. Database persistence works.
7. Financial calculations are verified.
8. Automated tests pass.
9. Performance targets are measured.
10. Documentation is complete.
11. Security Checks are completed.
12. Production deployment is documented.
13. Github repository is successfully released.