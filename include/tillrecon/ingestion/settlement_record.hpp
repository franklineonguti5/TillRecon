#pragma once

#include "tillrecon/domain/identifier.hpp"
#include "tillrecon/domain/money.hpp"
#include "tillrecon/ingestion/source_record_identity.hpp"

#include <chrono>
#include <optional>

namespace tillrecon::ingestion {

enum class SettlementKind {
    Payment,
    Refund,
    Reversal,
    Fee,
    Adjustment
};

// Financial movement from the merchant account's perspective.
enum class SettlementDirection {
    Credit,
    Debit
};

enum class SettlementStatus {
    Pending,
    Settled,
    Failed
};

class SettlementRecord final {
public:
    using Timestamp = std::chrono::sys_seconds;

    SettlementRecord(
        SourceRecordIdentity identity,
        std::optional<domain::ExternalReference> external_reference,
        std::optional<domain::ExternalReference> related_external_reference,
        domain::Money amount,
        Timestamp occurred_at,
        SettlementKind kind,
        SettlementDirection direction,
        SettlementStatus status
    );

    [[nodiscard]]
    const SourceRecordIdentity& identity() const noexcept;

    [[nodiscard]]
    const std::optional<domain::ExternalReference>&
    external_reference() const noexcept;

    [[nodiscard]]
    const std::optional<domain::ExternalReference>&
    related_external_reference() const noexcept;

    [[nodiscard]]
    const domain::Money& amount() const noexcept;

    [[nodiscard]]
    Timestamp occurred_at() const noexcept;

    [[nodiscard]]
    SettlementKind kind() const noexcept;

    [[nodiscard]]
    SettlementDirection direction() const noexcept;

    [[nodiscard]]
    SettlementStatus status() const noexcept;

    [[nodiscard]]
    bool operator==(
        const SettlementRecord& other
    ) const noexcept = default;

private:
    SourceRecordIdentity identity_;
    std::optional<domain::ExternalReference> external_reference_;
    std::optional<domain::ExternalReference> related_external_reference_;
    domain::Money amount_;
    Timestamp occurred_at_;
    SettlementKind kind_;
    SettlementDirection direction_;
    SettlementStatus status_;

    void validate() const;
};

}
