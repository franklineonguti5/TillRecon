#include "tillrecon/domain/money.hpp"

#include <limits>
#include <stdexcept>
#include <utility>

namespace tillrecon::domain {

Money::Money(MinorUnits amount, Currency currency)
    : amount_(amount),
    currency_(std::move(currency))
{
}

Money::MinorUnits Money::minor_units() const noexcept
{
    return amount_;
}

const Currency& Money::currency() const noexcept
{
    return currency_;
}

void Money::ensure_same_currency(const Money& other) const
{
    if (currency_ != other.currency_) {
        throw std::invalid_argument(
            "Cannot perform arithmetic on different currencies."
        );
    }
}

Money Money::operator+(const Money& other) const
{
    ensure_same_currency(other);

    constexpr auto maximum =
        std::numeric_limits<MinorUnits>::max();

    constexpr auto minimum =
        std::numeric_limits<MinorUnits>::min();

    if ((other.amount_ > 0 &&
        amount_ > maximum - other.amount_) ||
        (other.amount_ < 0 &&
        amount_ < minimum - other.amount_)) {

        throw std::overflow_error(
            "Monetary addition exceeds the supported range."
        );
    }

    return Money(
        amount_ + other.amount_,
        currency_
    );
}

Money Money::operator-(const Money& other) const
{
    ensure_same_currency(other);

    constexpr auto maximum =
        std::numeric_limits<MinorUnits>::max();

    constexpr auto minimum =
        std::numeric_limits<MinorUnits>::min();

    if ((other.amount_ < 0 &&
        amount_ > maximum + other.amount_) ||
        (other.amount_ > 0 &&
        amount_ < minimum + other.amount_)) {

        throw std::overflow_error(
            "Monetary subtraction exceeds the supported range."
        );
    }

    return Money(
        amount_ - other.amount_,
        currency_
    );
}

bool Money::is_positive() const noexcept
{
    return amount_ > 0;
}

bool Money::is_zero() const noexcept
{
    return amount_ == 0;
}

}