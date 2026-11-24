#include "tillrecon/ingestion/source_record_identity.hpp"

#include <stdexcept>
#include<type_traits>

#include <gtest/gtest.h>

using namespace tillrecon::ingestion;

// SOURCE RECORD IDENTITY TESTS

TEST(SourceRecordIdentityTests, CreateValidIdentity)
{
    const SourceRecordIdentity identity{
        SourceSystemId{"MPESA-TILL-001"},
        SourceRecordId{"QWE123XYZ"}
    };

    EXPECT_EQ(
        identity.source_system_id().value(),
        "MPESA-TILL-001"
    );

    EXPECT_EQ(
        identity.source_record_id().value(),
        "QWE123XYZ"
    );
}

TEST(SourceRecordIdentityTests, IdenticalIdentitiesCompareEqual)
{
    const SourceRecordIdentity first{
        SourceSystemId{"MPESA-TILL-001"},
        SourceRecordId{"QWE123XYZ"}
    };

    const SourceRecordIdentity second{
        SourceSystemId{"MPESA-TILL-001"},
        SourceRecordId{"QWE123XYZ"}
    };

    EXPECT_EQ(first, second);
}

TEST(SourceRecordIdentityTests, DifferentSourceSystemsAreNotEqual)
{
    const SourceRecordIdentity first{
        SourceSystemId{"MPESA-TILL-001"},
        SourceRecordId{"QWE123XYZ"}
    };

    const SourceRecordIdentity second{
        SourceSystemId{"MPESA-TILL-002"},
        SourceRecordId{"QWE123XYZ"}
    };

    EXPECT_NE(first, second);
}

TEST(SourceRecordIdentityTests, DifferentSourceRecordsAreNotEqual)
{
    const SourceRecordIdentity first{
        SourceSystemId{"MPESA-TILL-001"},
        SourceRecordId{"QWE123XYZ"}
    };

    const SourceRecordIdentity second{
        SourceSystemId{"MPESA-TILL-001"},
        SourceRecordId{"QWE123ABC"}
    };

    EXPECT_NE(first, second);
}

TEST(SourceRecordIdentityTests, RejectsEmptySourceSystemId)
{
    EXPECT_THROW(
        SourceSystemId{""},
        std::invalid_argument
    );
}

TEST (SourceRecordIdentityTests, RejectsEmptySourceRecordId)
{
    EXPECT_THROW(
        SourceRecordId{""},
        std::invalid_argument
    );
}

TEST(SourceRecordIdentityTests, RejectsUnsupportedSourceRecordCharacters)
{
    EXPECT_THROW(
        SourceRecordId{"QWE 123 XYZ"},
        std::invalid_argument
    );
}

TEST(SourceRecordIdentityTests, DistinguishesSourceIdentifierTypes)
{
    static_assert(
        !std::is_convertible_v<SourceSystemId, SourceRecordId>
    );

    static_assert(
        !std::is_convertible_v<SourceRecordId, SourceSystemId>
    );
}