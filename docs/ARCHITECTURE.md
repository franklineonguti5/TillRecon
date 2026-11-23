# TilRecon System Architecture

## 1. Architectural Approach

TillRecon follows a modular layered architecture.

Business rules must remain independent of
database implementations, file formats
and user interfaces.

## 2. System Components

### Core Domain

Contains fundamental financial entities
and business rules.

Responsibilities:

- Money representation
- Transaction identity
- Currency handling
- Transaction status
- Validation rules

### Ingestion Layer

Responsible for receiving financial records.

Responsibilities:

- CSV parcing
- Data normalization
- Input validation
- Import error reporting
- Duplicate import prevention

### Reconciliation Engine

Responsible for comparing POS records
against payment settlement records.

Responsibilities:

- Exactmatching
- Reference matching
- Tolerance matching 
- Duplicate detection
- Partial settlement analysis
- Exception classification

### Storage Layer

Responsible for persistence.

Initial implementation:

SQLite.

Production implementation:

PostgreSQL

Database access must be isolated behind
repository interfaces.

### Reporting Layers

Responsible for producing financial
and operational reports.

Supported initial formats:

- CSV
- JSON

Excel-compatible reports will be developed
as part of the reporting milestone.

### Application Layer

Coordinates system operations.

Responsibilities:

- Import commands
- Reconciliation execution
- Report Generation
- Exception inspection

## 3. Dependency Direction

    Application Layer
            |
            V
Domain and Application Services
            |
            V
    Repositoru Interfaces
            |
            V
Infrastructure Implementations

The domain must not depend directly
on SQLite or PostgreSQL.

## 4. Financial Representation

All monetary values ise signed 64-bit
integer minor units.

Floating-point financial calculations
are prohibited.

## 5. Reconciliation Integrity

Every reconciliation run must have
a unique identifier.

Source records must remain identifiable

Reconciliation results must preserve
their original transaction references.

## Error Handling

Error must be classified.

Categories:

- Input errors
- Validation errors
- Business rule violations
- Storage errors
- Imfrastructure errors

Error must never be silently discarded.

## 7. Testing Strategy

Unit tests:

Test domain entities and business rules.

Intergration tests:

Test ingestion and persistence.

Reconciliation tests:

Test matching and exception classification.

Performance tests:

Measure throughput and memory consumption.

End-to-end tests:

Test complete reconciliation workflows.