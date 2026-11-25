#pragma once

#include "tillrecon/ingestion/source_record_identity.hpp"

#include <chrono>
#include <string>
#include <string_view>

namespace tillrecon::ingestion {

enum class SourcePayloadFormat {
    Csv,
    Json
};

class RawSourceRecord final {
public:
    using Timestamp = std::chrono::sys_seconds;

    RawSourceRecord(
        SourceRecordIdentity identity,
        SourcePayloadFormat format,
        std::string payload,
        Timestamp received_at
    );

    [[nodiscard]]
    const SourceRecordIdentity& identity() const noexcept;

    [[nodiscard]]
    SourcePayloadFormat format() const noexcept;

    [[nodiscard]]
    std::string_view payload() const noexcept;

    [[nodiscard]]
    Timestamp received_at() const noexcept;

    [[nodiscard]]
    bool operator==(
        const RawSourceRecord& other
    ) const noexcept = default;

private:
    SourceRecordIdentity identity_;
    SourcePayloadFormat format_;
    std::string payload_;
    Timestamp received_at_;

    void validate() const;
};

}