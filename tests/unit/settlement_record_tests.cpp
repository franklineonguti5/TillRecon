#include "tillrecon/ingestion/settlement_record.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <optional>
#include <stdexcept>

namespace tillrecon::ingestion {
namespace {

using namespace std::chrono;

using domain::Currency;
using domain::ExternalReference;
using domain::Money;

SourceRecordIdentity create_identity()
{
    return SourceRecordIdentity{
        SourceSystemId{"MPESA-TILL-001"},
        SourceRecordId{"SETTLEMENT-10001"}
    };
}

Money create_amount()
{
    return Money{
        150000,
        Currency{"KES"}
    };
}

ExternalReference create_external_reference()
{
    return ExternalReference{"MPESA-REF-9001"};
}

SettlementRecord create_payment()
{
    return SettlementRecord{
        create_identity(),
        create_external_reference(),
        std::nullopt,
        create_amount(),
        sys_seconds{seconds{1'800'000'000}},
        SettlementKind::Payment,
        SettlementDirection::Credit,
        SettlementStatus::Settled
    };
}

SettlementRecord create_refund()
{
    return SettlementRecord{
        create_identity(),
        create_external_reference(),
        ExternalReference{"MPESA-ORIGINAL-9001"},
        create_amount(),
        sys_seconds{seconds{1'800'000'000}},
        SettlementKind::Refund,
        SettlementDirection::Debit,
        SettlementStatus::Settled
    };
}

TEST(SettlementRecordTests, CreatesValidPayment)
{
    const auto record = create_payment();

    EXPECT_EQ(record.kind(), SettlementKind::Payment);
    EXPECT_EQ(record.direction(), SettlementDirection::Credit);
    EXPECT_EQ(record.status(), SettlementStatus::Settled);
    EXPECT_EQ(record.amount(), create_amount());
}

TEST(SettlementRecordTests, CreatesValidRefund)
{
    const auto record = create_refund();

    EXPECT_EQ(record.kind(), SettlementKind::Refund);
    EXPECT_EQ(record.direction(), SettlementDirection::Debit);

    ASSERT_TRUE(record.related_external_reference().has_value());

    EXPECT_EQ(
        record.related_external_reference()->value(),
        "MPESA-ORIGINAL-9001"
    );
}

TEST(SettlementRecordTests, CreatesValidReversal)
{
    const SettlementRecord record{
        create_identity(),
        create_external_reference(),
        ExternalReference{"MPESA-ORIGINAL-9001"},
        create_amount(),
        sys_seconds{},
        SettlementKind::Reversal,
        SettlementDirection::Debit,
        SettlementStatus::Settled
    };

    EXPECT_EQ(record.kind(), SettlementKind::Reversal);
}

TEST(SettlementRecordTests, CreatesValidFee)
{
    const SettlementRecord record{
        create_identity(),
        std::nullopt,
        std::nullopt,
        create_amount(),
        sys_seconds{},
        SettlementKind::Fee,
        SettlementDirection::Debit,
        SettlementStatus::Settled
    };

    EXPECT_EQ(record.kind(), SettlementKind::Fee);
}

TEST(SettlementRecordTests, CreatesValidCreditAdjustment)
{
    const SettlementRecord record{
        create_identity(),
        std::nullopt,
        std::nullopt,
        create_amount(),
        sys_seconds{},
        SettlementKind::Adjustment,
        SettlementDirection::Credit,
        SettlementStatus::Settled
    };

    EXPECT_EQ(record.direction(), SettlementDirection::Credit);
}

TEST(SettlementRecordTests, CreatesValidDebitAdjustment)
{
    const SettlementRecord record{
        create_identity(),
        std::nullopt,
        std::nullopt,
        create_amount(),
        sys_seconds{},
        SettlementKind::Adjustment,
        SettlementDirection::Debit,
        SettlementStatus::Settled
    };

    EXPECT_EQ(record.direction(), SettlementDirection::Debit);
}

TEST(SettlementRecordTests, AcceptsPendingStatus)
{
    const SettlementRecord record{
        create_identity(),
        create_external_reference(),
        std::nullopt,
        create_amount(),
        sys_seconds{},
        SettlementKind::Payment,
        SettlementDirection::Credit,
        SettlementStatus::Pending
    };

    EXPECT_EQ(record.status(), SettlementStatus::Pending);
}

TEST(SettlementRecordTests, AcceptsFailedStatus)
{
    const SettlementRecord record{
        create_identity(),
        create_external_reference(),
        std::nullopt,
        create_amount(),
        sys_seconds{},
        SettlementKind::Payment,
        SettlementDirection::Credit,
        SettlementStatus::Failed
    };

    EXPECT_EQ(record.status(), SettlementStatus::Failed);
}

TEST(SettlementRecordTests, RejectsZeroAmount)
{
    const auto construct_record = [] {
        SettlementRecord record{
            create_identity(),
            create_external_reference(),
            std::nullopt,
            Money{0, Currency{"KES"}},
            sys_seconds{},
            SettlementKind::Payment,
            SettlementDirection::Credit,
            SettlementStatus::Settled
        };

        (void)record;
    };

    EXPECT_THROW(construct_record(), std::invalid_argument);
}

TEST(SettlementRecordTests, RejectsNegativeAmount)
{
    const auto construct_record = [] {
        SettlementRecord record{
            create_identity(),
            create_external_reference(),
            std::nullopt,
            Money{-100, Currency{"KES"}},
            sys_seconds{},
            SettlementKind::Payment,
            SettlementDirection::Credit,
            SettlementStatus::Settled
        };

        (void)record;
    };

    EXPECT_THROW(construct_record(), std::invalid_argument);
}

TEST(SettlementRecordTests, PreservesPaymentDebitDirection)
{
    const SettlementRecord record{
        create_identity(),
        create_external_reference(),
        std::nullopt,
        create_amount(),
        sys_seconds{},
        SettlementKind::Payment,
        SettlementDirection::Debit,
        SettlementStatus::Settled
    };

    EXPECT_EQ(record.kind(), SettlementKind::Payment);
    EXPECT_EQ(record.direction(), SettlementDirection::Debit);
}

TEST(SettlementRecordTests, PreservesRefundCreditDirection)
{
    const SettlementRecord record{
        create_identity(),
        create_external_reference(),
        ExternalReference{"ORIGINAL-100"},
        create_amount(),
        sys_seconds{},
        SettlementKind::Refund,
        SettlementDirection::Credit,
        SettlementStatus::Settled
    };

    EXPECT_EQ(record.kind(), SettlementKind::Refund);
    EXPECT_EQ(record.direction(), SettlementDirection::Credit);
}

TEST(SettlementRecordTests, PreservesReversalCreditDirection)
{
    const SettlementRecord record{
        create_identity(),
        create_external_reference(),
        ExternalReference{"ORIGINAL-100"},
        create_amount(),
        sys_seconds{},
        SettlementKind::Reversal,
        SettlementDirection::Credit,
        SettlementStatus::Settled
    };

    EXPECT_EQ(record.kind(), SettlementKind::Reversal);
    EXPECT_EQ(record.direction(), SettlementDirection::Credit);
}

TEST(SettlementRecordTests, AcceptsReversalWithoutRelatedReference)
{
    const SettlementRecord record{
        create_identity(),
        create_external_reference(),
        std::nullopt,
        create_amount(),
        sys_seconds{},
        SettlementKind::Reversal,
        SettlementDirection::Debit,
        SettlementStatus::Settled
    };

    EXPECT_EQ(record.kind(), SettlementKind::Reversal);
    EXPECT_FALSE(record.related_external_reference().has_value());
}

TEST(SettlementRecordTests, AcceptsRefundWithoutRelatedReference)
{
    const SettlementRecord record{
        create_identity(),
        create_external_reference(),
        std::nullopt,
        create_amount(),
        sys_seconds{},
        SettlementKind::Refund,
        SettlementDirection::Debit,
        SettlementStatus::Settled
    };

    EXPECT_EQ(record.kind(), SettlementKind::Refund);
    EXPECT_FALSE(record.related_external_reference().has_value());
}

TEST(SettlementRecordTests, AcceptsPaymentWithRelatedReference)
{
    const SettlementRecord record{
        create_identity(),
        create_external_reference(),
        ExternalReference{"RELATED-100"},
        create_amount(),
        sys_seconds{},
        SettlementKind::Payment,
        SettlementDirection::Credit,
        SettlementStatus::Settled
    };

    ASSERT_TRUE(record.related_external_reference().has_value());

    EXPECT_EQ(
        record.related_external_reference()->value(),
        "RELATED-100"
    );
}

TEST(SettlementRecordTests, PreservesFeeCreditDirection)
{
    const SettlementRecord record{
        create_identity(),
        std::nullopt,
        std::nullopt,
        create_amount(),
        sys_seconds{},
        SettlementKind::Fee,
        SettlementDirection::Credit,
        SettlementStatus::Settled
    };

    EXPECT_EQ(record.kind(), SettlementKind::Fee);
    EXPECT_EQ(record.direction(), SettlementDirection::Credit);
}

TEST(SettlementRecordTests, AcceptsAdjustmentWithRelatedReference)
{
    const SettlementRecord record{
        create_identity(),
        create_external_reference(),
        ExternalReference{"RELATED-ADJUSTMENT-100"},
        create_amount(),
        sys_seconds{},
        SettlementKind::Adjustment,
        SettlementDirection::Debit,
        SettlementStatus::Settled
    };

    ASSERT_TRUE(record.related_external_reference().has_value());

    EXPECT_EQ(
        record.related_external_reference()->value(),
        "RELATED-ADJUSTMENT-100"
    );
}

TEST(SettlementRecordTests, RejectsUnsupportedSettlementKind)
{
    const auto construct_record = [] {
        SettlementRecord record{
            create_identity(),
            std::nullopt,
            std::nullopt,
            create_amount(),
            sys_seconds{},
            static_cast<SettlementKind>(999),
            SettlementDirection::Credit,
            SettlementStatus::Settled
        };

        (void)record;
    };

    EXPECT_THROW(construct_record(), std::invalid_argument);
}

TEST(SettlementRecordTests, RejectsUnsupportedSettlementDirection)
{
    const auto construct_record = [] {
        SettlementRecord record{
            create_identity(),
            create_external_reference(),
            std::nullopt,
            create_amount(),
            sys_seconds{},
            SettlementKind::Payment,
            static_cast<SettlementDirection>(999),
            SettlementStatus::Settled
        };

        (void)record;
    };

    EXPECT_THROW(construct_record(), std::invalid_argument);
}

TEST(SettlementRecordTests, RejectsUnsupportedSettlementStatus)
{
    const auto construct_record = [] {
        SettlementRecord record{
            create_identity(),
            create_external_reference(),
            std::nullopt,
            create_amount(),
            sys_seconds{},
            SettlementKind::Payment,
            SettlementDirection::Credit,
            static_cast<SettlementStatus>(999)
        };

        (void)record;
    };

    EXPECT_THROW(construct_record(), std::invalid_argument);
}

TEST(SettlementRecordTests, IdenticalRecordsCompareEqual)
{
    const auto first = create_payment();
    const auto second = create_payment();

    EXPECT_EQ(first, second);
}

}
}
