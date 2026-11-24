#pragma once

#include "tillrecon/domain/currency.hpp"

#include <cstdint>

namespace tillrecon::domain {

class Money final {
public:
    using MinorUnits = std::int64_t;

    Money(MinorUnits amount, Currency currency);

    [[nodiscard]]
    MinorUnits minor_units() const noexcept;

    [[nodiscard]]
    const Currency& currency() const noexcept;

    [[nodiscard]]
    Money operator+(const Money& other) const;

    [[nodiscard]]
    Money operator-(const Money& other) const;

    [[nodiscard]]
    bool operator==(const Money& other) const noexcept = default;

    [[nodiscard]]
    bool is_positive() const noexcept;

    [[nodiscard]]
    bool is_zero() const noexcept;

private:
    void ensure_same_currency(const Money& other) const;

    MinorUnits amount_;
    Currency currency_;
};

}