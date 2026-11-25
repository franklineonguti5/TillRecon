#include "tillrecon/ingestion/raw_source_record.hpp"

#include <stdexcept>
#include <utility>

namespace tillrecon::ingestion {

RawSourceRecord::RawSourceRecord(
    SourceRecordIdentity identity,
    SourcePayloadFormat format,
    std::string payload,
    Timestamp received_at
)
    : identity_(std::move(identity)),
      format_(format),
      payload_(std::move(payload)),
      received_at_(received_at)
{
    validate();
}

void RawSourceRecord::validate() const
{
    if (payload_.empty()) {
        throw std::invalid_argument(
            "Source record payload cannot be empty."
        );
    }

    switch (format_) {
        case SourcePayloadFormat::Csv:
        case SourcePayloadFormat::Json:
            break;

        default:
            throw std::invalid_argument(
                "Unsupported source payload format."
            );
    }
}

const SourceRecordIdentity& RawSourceRecord::identity() const noexcept
{
    return identity_;
}

SourcePayloadFormat RawSourceRecord::format() const noexcept
{
    return format_;
}

std::string_view RawSourceRecord::payload() const noexcept
{
    return payload_;
}

RawSourceRecord::Timestamp RawSourceRecord::received_at() const noexcept
{
    return received_at_;
}

}