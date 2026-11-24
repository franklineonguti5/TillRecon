#pragma once

#include <stdexcept>

namespace tillrecon::domain {

template <typename Tag>
BasicIdentifier<Tag>::BasicIdentifier(std::string_view value)
    : value_(value)
{
    if (value_.empty()) {
        throw std::invalid_argument(
            "Identifier cannot be empty."
        );
    }

    if (value_.size() > 128) {
        throw std::invalid_argument(
            "Identifier exceeds the maximum length."
        );
    }

    for (const char character : value_) {

        const bool valid =
            (character >= 'A' && character <= 'Z') ||
            (character >= 'a' && character <= 'z') ||
            (character >= '0' && character <= '9') ||
            character == '-' ||
            character == '_';

        if (!valid) {
            throw std::invalid_argument(
                "Identifier contains unsupported characters."
            );
        }
    }
}

template <typename Tag>
const std::string& BasicIdentifier<Tag>::value() const noexcept
{
    return value_;
}

}