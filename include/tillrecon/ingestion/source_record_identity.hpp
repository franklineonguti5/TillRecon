#pragma once

#include "tillrecon/domain/identifier.hpp"

namespace tillrecon::ingestion {

struct SourceSystemTag{};
struct SourceRecordTag {};

using SourceSystemId =
    domain::BasicIdentifier<SourceSystemTag>;

using SourceRecordId =
    domain::BasicIdentifier<SourceRecordTag>;

class SourceRecordIdentity final {
public:
    SourceRecordIdentity(
        SourceSystemId source_system_id,
        SourceRecordId source_record_id
    );

    [[nodiscard]]
    const SourceSystemId& source_system_id() const noexcept;

    [[nodiscard]]
    const SourceRecordId& source_record_id() const noexcept;

    [[nodiscard]]
    bool operator==(
        const SourceRecordIdentity& other
    ) const noexcept = default;

private:
    SourceSystemId source_system_id_;
    SourceRecordId source_record_id_;
};

}