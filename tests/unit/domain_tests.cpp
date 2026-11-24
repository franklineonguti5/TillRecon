#include "tillrecon/domain/currency.hpp"
#include "tillrecon/domain/domain_error.hpp"
#include "tillrecon/domain/identifier.hpp"
#include "tillrecon/domain/money.hpp"
#include "tillrecon/domain/transaction.hpp"

#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>
#include <type_traits>

using namespace tillrecon::domain;

namespace {

Currency kes()
{
    return Currency{"KES"};
}

Money money(std::int64_t amount)
{
    return Money{amount, kes()};
}

Transaction create_sale(
    Money amount,
    TransactionStatus status = TransactionStatus::Posted
)
{
    return Transaction{
        TransactionId{"TXN-10001"},
        BranchId{"BR-001"},
        ExternalReference{"MPESA-88291"},
        std::move(amount),
        Transaction::Timestamp{},
        TransactionKind::Sale,
        PaymentMethod::Mpesa,
        status,
        std::nullopt
    };
}

}

// CURRENCY TESTS

TEST(CurrencyTests, AcceptsValidCurrencyCode)
{
    const Currency currency{"KES"};

    EXPECT_EQ(currency.code(), "KES");
}

TEST(CurrencyTests, RejectsInvalidLength)
{
    EXPECT_THROW(
        Currency{"KE"},
        std::invalid_argument
    );

    EXPECT_THROW(
        Currency{"KESH"},
        std::invalid_argument
    );
}

TEST(CurrencyTests, RejectsLowercaseCurrency)
{
    EXPECT_THROW(
        Currency{"kes"},
        std::invalid_argument
    );
}

TEST(CurrencyTests, RejectsNumericCurrency)
{
    EXPECT_THROW(
        Currency{"123"},
        std::invalid_argument
    );
}

// MONEY TESTS

TEST(MoneyTests, StoresMinorUnits)
{
    const Money amount{125075, kes()};

    EXPECT_EQ(amount.minor_units(), 125075);
}

TEST(MoneyTests, AddsMoneyCorrectly)
{
    const Money first{10000, kes()};
    const Money second{25000, kes()};

    const Money result = first + second;

    EXPECT_EQ(result.minor_units(), 35000);
}

TEST(MoneyTests, SubtractsMoneyCorrectly)
{
    const Money first{50000, kes()};
    const Money second{15000, kes()};

    const Money result = first - second;

    EXPECT_EQ(result.minor_units(), 35000);
}

TEST(MoneyTests, RejectsDifferentCurrencies)
{
    const Money kes_amount{10000, Currency{"KES"}};
    const Money usd_amount{10000, Currency{"USD"}};

    EXPECT_THROW(
        (static_cast<void>(kes_amount + usd_amount)),
        std::invalid_argument
    );
}

TEST(MoneyTests, DetectsAdditionOverflow)
{
    const Money first{
        std::numeric_limits<std::int64_t>::max(),
        kes()
    };

    const Money second{1, kes()};

    EXPECT_THROW(
        (static_cast<void>(first + second)),
        std::overflow_error
    );
}

TEST(MoneyTests, DetectsSubtractionOverflow)
{
    const Money first{
        std::numeric_limits<std::int64_t>::min(),
        kes()
    };

    const Money second{1, kes()};

    EXPECT_THROW(
        (static_cast<void>(first - second)),
        std::overflow_error
    );
}

TEST(MoneyTests, IdentifiesPositiveAmounts)
{
    EXPECT_TRUE(money(100).is_positive());
    EXPECT_FALSE(money(0).is_positive());
    EXPECT_FALSE(money(-100).is_positive());
}

TEST(MoneyTests, IdentifiesZeroAmounts)
{
    EXPECT_TRUE(money(0).is_zero());
    EXPECT_FALSE(money(100).is_zero());
}

// IDENTIFIER TESTS

TEST(IdentifierTests, AcceptsValidTransactionId)
{
    const TransactionId id{"TXN-10001"};

    EXPECT_EQ(id.value(), "TXN-10001");
}

TEST(IdentifierTests, RejectsEmptyIdentifier)
{
    EXPECT_THROW(
        TransactionId{""},
        std::invalid_argument
    );
}

TEST(IdentifierTests, RejectsUnsupportedCharacters)
{
    EXPECT_THROW(
        TransactionId{"TXM 10001"},
        std::invalid_argument
    );
}

TEST(IdentifierTests, RejectsExcessivelyLongIdentifier)
{
    EXPECT_THROW(
        TransactionId{std::string(129, 'A')},
        std::invalid_argument
    );
}

TEST(IdentifierTests, DistinguishesIdentifierTypes)
{
    static_assert(
        !std::is_convertible_v<TransactionId, BranchId>
    );

    static_assert(
        !std::is_convertible_v<BranchId, TransactionId>
    );
}

TEST(IdentifierTests, AcceptsMaximumIdentifierLength)
{
    const TransactionId id{std::string(128, 'A')};

    EXPECT_EQ(id.value().size(), 128);
}

TEST(IdentifierTests, AcceptsValidBranchId)
{
    const BranchId id{"BR-001"};

    EXPECT_EQ(id.value(), "BR-001");
}

TEST(IdentifierTests, AcceptsValidExternalReference)
{
    const ExternalReference reference{"MPESA-88291"};

    EXPECT_EQ(reference.value(), "MPESA-88291");
}

TEST(IdentifierTests, RejectsEmptyExternalReference)
{
    EXPECT_THROW(
        ExternalReference{""},
        std::invalid_argument
    );
}

// TRANSACTION TESTS

TEST(TransactionTests, CreatesValidSales)
{
    const Transaction transaction =
        create_sale(money(150000));

    EXPECT_EQ(
        transaction.id().value(),
        "TXN-10001"
    );

    EXPECT_EQ(
        transaction.branch_id().value(),
        "BR-001"
    );

    EXPECT_EQ(
        transaction.amount().minor_units(),
        150000
    );

    EXPECT_EQ(
        transaction.kind(),
        TransactionKind::Sale
    );
}

TEST(TransactionTests, RejectsUnsupportedTransactionKind)
{
    EXPECT_THROW(
        (Transaction{
            TransactionId{"TXN-10002"},
            BranchId{"BR-001"},
            std::nullopt,
            money(10000),
            Transaction::Timestamp{},
            static_cast<TransactionKind>(999),
            PaymentMethod::Cash,
            TransactionStatus::Posted,
            std::nullopt
        }),
        InvalidTransactionError
    );
}

TEST(TransactionTests, RejectsUnsupportedPaymentMethod)
{
    EXPECT_THROW(
        (Transaction{
            TransactionId{"TXN-10003"},
            BranchId{"BR-001"},
            std::nullopt,
            money(10000),
            Transaction::Timestamp{},
            TransactionKind::Sale,
            static_cast<PaymentMethod>(999),
            TransactionStatus::Posted,
            std::nullopt
        }),
        InvalidTransactionError
    );
}

TEST(TransactionTests, RejectsUnsupportedTransactionStatus)
{
    EXPECT_THROW(
        (Transaction{
            TransactionId{"TXN-10004"},
            BranchId{"BR-001"},
            std::nullopt,
            money(10000),
            Transaction::Timestamp{},
            TransactionKind::Sale,
            PaymentMethod::Cash,
            static_cast<TransactionStatus>(999),
            std::nullopt
        }),
        InvalidTransactionError
    );
}

TEST(TransactionTests, RejectsZeroAmount)
{
    EXPECT_THROW(
        create_sale(money(0)),
        InvalidTransactionError
    );
}

TEST(TransactionTests, RejectsNegativeAmount)
{
    EXPECT_THROW(
        create_sale(money(-100)),
        InvalidTransactionError
    );
}

TEST(TransactionTests, RejectsSaleWithRelatedTransaction)
{
    EXPECT_THROW(
        (Transaction{
            TransactionId{"TXN-10002"},
            BranchId{"BR-001"},
            std::nullopt,
            money(10000),
            Transaction::Timestamp{},
            TransactionKind::Sale,
            PaymentMethod::Cash,
            TransactionStatus::Posted,
            TransactionId{"TXN-10001"}
        }),
        InvalidTransactionError
    );
}

TEST(TransactionTests, RejectsRefundWithoutOriginalTransaction)
{
    EXPECT_THROW(
        (Transaction{
            TransactionId{"REF-10001"},
            BranchId{"BR-001"},
            std::nullopt,
            money(5000),
            Transaction::Timestamp{},
            TransactionKind::Refund,
            PaymentMethod::Mpesa,
            TransactionStatus::Posted,
            std::nullopt
        }),
        InvalidTransactionError
    );
}

TEST(TransactionTests, AcceptsValidRefund)
{
    const Transaction refund{
        TransactionId{"REF-10001"},
        BranchId{"BR-001"},
        std::nullopt,
        money(5000),
        Transaction::Timestamp{},
        TransactionKind::Refund,
        PaymentMethod::Mpesa,
        TransactionStatus::Posted,
        TransactionId{"TXN-10001"}
    };

    EXPECT_EQ(
        refund.kind(),
        TransactionKind::Refund
    );

    EXPECT_EQ(
        refund.related_transaction_id()->value(),
        "TXN-10001"
    );
}

TEST(TransactionTests, AcceptsValidReversal)
{
    const Transaction reversal{
        TransactionId{"REV-10001"},
        BranchId{"BR-001"},
        std::nullopt,
        money(5000),
        Transaction::Timestamp{},
        TransactionKind::Reversal,
        PaymentMethod::Mpesa,
        TransactionStatus::Posted,
        TransactionId{"TXN-10001"}
    };

    EXPECT_EQ(
        reversal.kind(),
        TransactionKind::Reversal
    );


    EXPECT_EQ(
        reversal.related_transaction_id()->value(),
        "TXN-10001"
    );
}

TEST(TransactionTests, RejectsReversalWithoutOriginalTransaction)
{
    EXPECT_THROW(
        (Transaction{
            TransactionId{"REV-10002"},
            BranchId{"BR-001"},
            std::nullopt,
            money(5000),
            Transaction::Timestamp{},
            TransactionKind::Reversal,
            PaymentMethod::Mpesa,
            TransactionStatus::Posted,
            std::nullopt
        }),
        InvalidTransactionError
    );
}

TEST(TransactionTests, RejectsSelfReferencingRefund)
{
    EXPECT_THROW(
        (Transaction{
            TransactionId{"REF-10001"},
            BranchId{"BR-001"},
            std::nullopt,
            money(5000),
            Transaction::Timestamp{},
            TransactionKind::Refund,
            PaymentMethod::Mpesa,
            TransactionStatus::Posted,
            TransactionId{"REF-10001"}
        }),
        InvalidTransactionError
    );
}

TEST(TransactionTests, AcceptsVoidSale)
{
    const Transaction transaction =
        create_sale(
            money(15000),
            TransactionStatus::Voided
        );

    EXPECT_EQ(
        transaction.status(),
        TransactionStatus::Voided
    );
}