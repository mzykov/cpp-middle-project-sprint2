#include "scan.hpp"

int main() {
    {
        static_assert(
            std::get<int>(stdx::scan<stdx::details::format_string<"Integer: {%d}">{}, stdx::details::fixed_string{"Integer: 42"}, int>().data) == 42
        );
    }
    {
        static_assert(
            std::get<int>(stdx::scan<stdx::details::format_string<"Negative integer: {%d}">{}, stdx::details::fixed_string{"Negative integer: -42"}, int>().data) == -42
        );
    }
    {
        static_assert(
            std::get<unsigned int>(stdx::scan<stdx::details::format_string<"Unsigned integer: {%u}">{}, stdx::details::fixed_string{"Unsigned integer: 43"}, unsigned int>().data) == 43
        );
    }
    {
        static_assert(
            std::get<std::string_view>(stdx::scan<stdx::details::format_string<"String view of: {%s}!">{}, stdx::details::fixed_string{"String view of: Hello World!"}, std::string_view>().data) == "Hello World"
        );
    }
    {
        // Given
        constexpr auto fmt    = stdx::details::format_string<"Say {%s} {%d} times!">{};
        constexpr auto source = stdx::details::fixed_string{"Say Hello 3 times!"};

        // When
        constexpr auto result = stdx::scan<fmt, source, std::string_view, int>();

        // Then
        static_assert(
            std::get<std::string_view>(result.data) == "Hello"
        );
        static_assert(
            std::get<int>(result.data) == 3
        );
    }
    {
        // Given
        constexpr auto fmt    = stdx::details::format_string<"My name is {%s} {%s}!">{};
        constexpr auto source = stdx::details::fixed_string{"My name is Mikhail Zykov!"};

        // When
        constexpr auto result = stdx::scan<fmt, source, std::string_view, std::string_view>();

        // Then
        static_assert(
            std::get<0>(result.data) == "Mikhail"
        );
        static_assert(
            std::get<1>(result.data) == "Zykov"
        );
    }
    return 0;
}
