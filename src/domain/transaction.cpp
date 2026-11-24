#include "tillrecon/domain/transaction.hpp"
#include "tillrecon/domain/domain_error.hpp"

#include <stdexcept>
#include <utility>

namespace tillrecon::domain {

Transaction::Transaction(
    TransactionId id,
    BranchId branch_id,
    std::optional<ExternalReference> external_reference,
    Money amount,
    Timestamp occurred_at,
    TransactionKind kind,
    PaymentMethod payment_method,
    TransactionStatus status,
    std::optional<TransactionId> related_transaction_id
)
    :   id_(std::move(id)),
        branch_id_(std::move(branch_id)),
        external_reference_(std::move(external_reference)),
        amount_(std::move(amount)),
        occurred_at_(occurred_at),
        kind_(kind),
        payment_method_(payment_method),
        status_(status),
        related_transaction_id_(std::move(related_transaction_id))
{
    validate();
}

void Transaction::validate() const
{
    if (!amount_.is_positive()) {
        throw InvalidTransactionError(
            "Transaction amount must be greater than zero."
        );
    }

    switch (kind_) {
        case TransactionKind::Sale:
            if (related_transaction_id_.has_value()) {
                throw InvalidTransactionError(
                    "A sale cannot reference a related transaction."
                );
            }
            break;

        case TransactionKind::Refund:
        case TransactionKind::Reversal:
            if (!related_transaction_id_.has_value()) {
                throw InvalidTransactionError(
                    "Refund and reversals must reference"
                    " their original transaction."
                );
            }

            if (related_transaction_id_.value() == id_) {
                throw InvalidTransactionError(
                    "A transaction cannot reference itself."
                );
            }
            break;

        default:
            throw InvalidTransactionError(
                "Unsupported transaction kind."
            );
    }

    switch (payment_method_) {
        case PaymentMethod::Cash:
        case PaymentMethod::Mpesa:
        case PaymentMethod::BankTransfer:
        case PaymentMethod::DebitCard:
        case PaymentMethod::CreditCard:
            break;

        default:
            throw InvalidTransactionError(
                "Unsupported payment method."
            );
    }

    switch (status_) {
        case TransactionStatus::Posted:
        case TransactionStatus::Voided:
            break;

        default:
            throw InvalidTransactionError(
                "Unsupported transaction status."
            );
    }
}

const TransactionId& Transaction::id() const noexcept
{
    return id_;
}

const BranchId& Transaction::branch_id() const noexcept
{
    return branch_id_;
}

const std::optional<ExternalReference>&
Transaction::external_reference() const noexcept
{
    return external_reference_;
}

const Money& Transaction::amount() const noexcept
{
    return amount_;
}

Transaction::Timestamp Transaction::occurred_at() const noexcept
{
    return occurred_at_;
}

TransactionKind Transaction::kind() const noexcept
{
    return kind_;
}

PaymentMethod Transaction::payment_method() const noexcept
{
    return payment_method_;
}

TransactionStatus Transaction::status() const noexcept
{
    return status_;
}

const std::optional<TransactionId>&
Transaction::related_transaction_id() const noexcept
{
    return related_transaction_id_;
}

}