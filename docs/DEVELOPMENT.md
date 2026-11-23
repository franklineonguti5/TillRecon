# Development Standards

## C++ Standards

- C++20 minimum.
- Prefer standard library facilities.
- Use RAII for resource management.
- Avoid unnecessary raw pointers.
- Use const correctness.
- Prefer explicit interfaces.
- Avoid global mutable state.
- Use meaningful names.

## Financial Standards

- Never use floating-point values for money.
- Validate monetary boundaries.
- Detect integer overflow.
- Preserve currwncy information.
- Never silently round financial amounts.

## Testing

- Test normal execution.
- Test invalid inputs.
- Test boundary conditions.
- Test duplicate records.
- Test negative scenarios.
- Test financial edge cases.

## Git Standards

Commit messages must describe actual changes.

Examples:

feat: add financial transaction domain model

fix: reject invalid settlement amounts

test: cover duplicate transaction detection

docs: document reconciliation architecture

refactor: isolate persistence implementation

perf: optimize transaction matching

## Code Review

No feature is considered complete until:

1. Implementation exists.
2. Tests cover its behaviour.
3. Edge cases are considered.
4. Documentation is updated.
5. No known critical defects remain.