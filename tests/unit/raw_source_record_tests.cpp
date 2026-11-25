#include "tillrecon/ingestion/raw_source_record.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <stdexcept>
#include <string>
#include <string_view>

namespace tillrecon::ingestion {
namespace {

using namespace std::chrono;

SourceRecordIdentity create_identity()
{
    return SourceRecordIdentity{
        SourceSystemId{"MPESA-TILL-001"},
        SourceRecordId{"RECORD-10001"}
    };
}

RawSourceRecord create_csv_record()
{
    return RawSourceRecord{
        create_identity(),
        SourcePayloadFormat::Csv,
        "RECORD-10001,TXN-9001,150000,KES",
        sys_seconds{seconds{1'800'000'000}}
    };
}

TEST(RawSourceRecordTests, CreatesValidCsvRecord)
{
    const auto record = create_csv_record();

    EXPECT_EQ(record.format(), SourcePayloadFormat::Csv);

    EXPECT_EQ(
        record.payload(),
        "RECORD-10001,TXN-9001,150000,KES"
    );

    EXPECT_EQ(
        record.identity().source_system_id().value(),
        "MPESA-TILL-001"
    );

    EXPECT_EQ(
        record.identity().source_record_id().value(),
        "RECORD-10001"
    );
}

TEST(RawSourceRecordTests, CreatesValidJsonRecord)
{
    const RawSourceRecord record{
        create_identity(),
        SourcePayloadFormat::Json,
        R"({"transaction_id":"TXN-9001","amount":1500})",
        sys_seconds{seconds{1'800'000'000}}
    };

    EXPECT_EQ(record.format(), SourcePayloadFormat::Json);

    EXPECT_EQ(
        record.payload(),
        R"({"transaction_id":"TXN-9001","amount":1500})"
    );
}

TEST(RawSourceRecordTests, RejectsEmptyPayload)
{
    const auto construct_record = [] {
        RawSourceRecord record{
            create_identity(),
            SourcePayloadFormat::Csv,
            "",
            sys_seconds{}
        };

        (void)record;
    };

    EXPECT_THROW(
        construct_record(),
        std::invalid_argument
    );
}

TEST(RawSourceRecordTests, RejectsUnsupportedPayloadFormat)
{
    const auto construct_record = [] {
        RawSourceRecord record{
            create_identity(),
            static_cast<SourcePayloadFormat>(999),
            "some-payload",
            sys_seconds{}
        };

        (void)record;
    };

    EXPECT_THROW(
        construct_record(),
        std::invalid_argument
    );
}

TEST(RawSourceRecordTests, PreservesOriginalPayload)
{
    const std::string original =
        R"(  {"amount":1500,"reference":"ABC-123"}  )";

    const RawSourceRecord record{
        create_identity(),
        SourcePayloadFormat::Json,
        original,
        sys_seconds{}
    };

    EXPECT_EQ(record.payload(), original);
}

TEST(RawSourceRecordTests, PreservesReceivedTimestamp)
{
    const auto received_at =
        sys_seconds{seconds{1'800'000'000}};

    const RawSourceRecord record{
        create_identity(),
        SourcePayloadFormat::Csv,
        "RECORD-10001,TXN-9001,150000,KES",
        received_at
    };

    EXPECT_EQ(record.received_at(), received_at);
}

TEST(RawSourceRecordTests, IdenticalRecordsCompareEqual)
{
    const auto first = create_csv_record();
    const auto second = create_csv_record();

    EXPECT_EQ(first, second);
}

TEST(RawSourceRecordTests, DifferentPayloadsCompareUnequal)
{
    const RawSourceRecord first{
        create_identity(),
        SourcePayloadFormat::Csv,
        "TXN-100,1000",
        sys_seconds{}
    };

    const RawSourceRecord second{
        create_identity(),
        SourcePayloadFormat::Csv,
        "TXN-100,2000",
        sys_seconds{}
    };

    EXPECT_NE(first, second);
}

}
}