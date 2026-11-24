#include "tillrecon/ingestion/source_record_identity.hpp"

#include <utility>

namespace tillrecon::ingestion {

SourceRecordIdentity::SourceRecordIdentity(
    SourceSystemId source_system_id,
    SourceRecordId source_record_id
)
    :   source_system_id_(std::move(source_system_id)),
        source_record_id_(std::move(source_record_id))
{
}

const SourceSystemId&
SourceRecordIdentity::source_system_id() const noexcept
{
    return source_system_id_;
}

const SourceRecordId&
SourceRecordIdentity::source_record_id() const noexcept
{
    return source_record_id_;
}

}