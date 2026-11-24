#include "tillrecon/domain/currency.hpp"

#include <stdexcept>

namespace tillrecon::domain {

Currency::Currency(std::string_view code)
    : code_(code)
{
    validate(code_);
}

const std::string& Currency::code() const noexcept
{
    return code_;
}

void Currency::validate(std::string_view code)
{
    if (code.size() != 3) {
        throw std::invalid_argument(
            "Currency code must contain exactly three characters."
        );
    }

    for (const char character : code) {
        if (character < 'A' || character > 'Z') {
            throw std::invalid_argument(
                "Currency code must contain uppercase ASCII letters."
            );
        }
    }
}

}