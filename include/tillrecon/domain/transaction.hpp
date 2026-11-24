#pragma once

#include "tillrecon/domain/identifier.hpp"
#include "tillrecon/domain/money.hpp"

#include <chrono>
#include <optional>

namespace tillrecon::domain {

enum class TransactionKind {
    Sale,
    Refund,
    Reversal
};

enum class PaymentMethod {
    Cash,
    Mpesa,
    BankTransfer,
    DebitCard,
    CreditCard
};

// Represent the recorded status of an imported transaction.
// Both states are valid during transaction construction.
// State-transition rules are enforced separately.
enum class TransactionStatus {
    Posted,
    Voided
};

class Transaction final {
public:
    using Timestamp = std::chrono::sys_seconds;

    Transaction(
        TransactionId id,
        BranchId branch_id,
        std::optional<ExternalReference> external_reference,
        Money amount,
        Timestamp occurred_at,
        TransactionKind kind,
        PaymentMethod payment_method,
        TransactionStatus status,
        std::optional<TransactionId> related_transaction_id
    );

    [[nodiscard]]
    const TransactionId& id() const noexcept;

    [[nodiscard]]
    const BranchId& branch_id() const noexcept;

    [[nodiscard]]
    const std::optional<ExternalReference>&
    external_reference() const noexcept;

    [[nodiscard]]
    const Money& amount() const noexcept;

    [[nodiscard]]
    Timestamp occurred_at() const noexcept;

    [[nodiscard]]
    TransactionKind kind() const noexcept;

    [[nodiscard]]
    PaymentMethod payment_method() const noexcept;

    [[nodiscard]]
    TransactionStatus status() const noexcept;

    [[nodiscard]]
    const std::optional<TransactionId>&
    related_transaction_id() const noexcept;

private:
    TransactionId id_;
    BranchId branch_id_;
    std::optional<ExternalReference> external_reference_;
    Money amount_;
    Timestamp occurred_at_;
    TransactionKind kind_;
    PaymentMethod payment_method_;
    TransactionStatus status_;
    std::optional<TransactionId> related_transaction_id_;

    void validate() const;
};

}