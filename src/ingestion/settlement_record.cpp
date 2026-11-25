#include "tillrecon/ingestion/settlement_record.hpp"

#include <stdexcept>
#include <utility>

namespace tillrecon::ingestion {

SettlementRecord::SettlementRecord(
    SourceRecordIdentity identity,
    std::optional<domain::ExternalReference> external_reference,
    std::optional<domain::ExternalReference> related_external_reference,
    domain::Money amount,
    Timestamp occurred_at,
    SettlementKind kind,
    SettlementDirection direction,
    SettlementStatus status
)
    : identity_(std::move(identity)),
      external_reference_(std::move(external_reference)),
      related_external_reference_(std::move(related_external_reference)),
      amount_(std::move(amount)),
      occurred_at_(occurred_at),
      kind_(kind),
      direction_(direction),
      status_(status)
{
    validate();
}

void SettlementRecord::validate() const
{
    if (!amount_.is_positive()) {
        throw std::invalid_argument(
            "Settlement amount must be positive."
        );
    }

    switch (kind_) {
        case SettlementKind::Payment:
        case SettlementKind::Refund:
        case SettlementKind::Reversal:
        case SettlementKind::Fee:
        case SettlementKind::Adjustment:
            break;

        default:
            throw std::invalid_argument(
                "Unsupported settlement kind."
            );
    }

    switch (direction_) {
        case SettlementDirection::Credit:
        case SettlementDirection::Debit:
            break;

        default:
            throw std::invalid_argument(
                "Unsupported settlement direction."
            );
    }

    switch (status_) {
        case SettlementStatus::Pending:
        case SettlementStatus::Settled:
        case SettlementStatus::Failed:
            break;

        default:
            throw std::invalid_argument(
                "Unsupported settlement status."
            );
    }
}

const SourceRecordIdentity&
SettlementRecord::identity() const noexcept
{
    return identity_;
}

const std::optional<domain::ExternalReference>&
SettlementRecord::external_reference() const noexcept
{
    return external_reference_;
}

const std::optional<domain::ExternalReference>&
SettlementRecord::related_external_reference() const noexcept
{
    return related_external_reference_;
}

const domain::Money&
SettlementRecord::amount() const noexcept
{
    return amount_;
}

SettlementRecord::Timestamp
SettlementRecord::occurred_at() const noexcept
{
    return occurred_at_;
}

SettlementKind SettlementRecord::kind() const noexcept
{
    return kind_;
}

SettlementDirection SettlementRecord::direction() const noexcept
{
    return direction_;
}

SettlementStatus SettlementRecord::status() const noexcept
{
    return status_;
}

}
