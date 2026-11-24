#pragma once

#include <stdexcept>
#include <string>

namespace tillrecon::domain {

class DomainError : public std::runtime_error {
public:
    explicit DomainError(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

class InvalidTransactionError final : public DomainError {
public:
    explicit InvalidTransactionError(
        const std::string& message
    )
        : DomainError(message)
    {
    }
};

}