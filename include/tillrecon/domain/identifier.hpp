#pragma once

#include <string>
#include <string_view>

namespace tillrecon::domain {

struct TransactionIdTag {};
struct BranchIdTag {};
struct ExternalReferenceTag {};

template <typename Tag>
class BasicIdentifier final {
public:
    explicit BasicIdentifier(std::string_view value);

    [[nodiscard]]
    const std::string& value() const noexcept;

    [[nodiscard]]
    bool operator==(
        const BasicIdentifier& other
    ) const noexcept = default;

private:
    std::string value_;
};

using TransactionId = BasicIdentifier<TransactionIdTag>;
using BranchId = BasicIdentifier<BranchIdTag>;
using ExternalReference = BasicIdentifier<ExternalReferenceTag>;

}

#include "tillrecon/domain/identifier.tpp"