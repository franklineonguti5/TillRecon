#pragma once

#include <string>
#include <string_view>

namespace tillrecon::domain {

class Currency final {
public:
    explicit Currency(std::string_view code);

    [[nodiscard]]
    const std::string& code() const noexcept;

    [[nodiscard]]
    bool operator==(const Currency& other) const noexcept = default;

private:
    std::string code_;

    static void validate(std::string_view code);
};

}