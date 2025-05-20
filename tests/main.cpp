#include "scan.hpp"

int main() {
    {	// 1.
        // Given
        constexpr auto fmt = stdx::details::format_string<"Integer: {%d}">{};
        constexpr auto src = stdx::details::fixed_string{"Integer: 42"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int>();

        // Then
        static_assert(res.values<int>() == 42);
    }
    {	// 2.
        // Given
        constexpr auto fmt = stdx::details::format_string<"Negative integer: {%d}">{};
        constexpr auto src = stdx::details::fixed_string{"Negative integer: -42"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int>();

        // Then
        static_assert(res.values<int>() == -42);
    }
    {	// 3.
        // Given
        constexpr auto fmt = stdx::details::format_string<"Unsigned integer: {%u}">{};
        constexpr auto src = stdx::details::fixed_string{"Unsigned integer: 43"};

        // When
        constexpr auto res = stdx::scan<fmt, src, unsigned int>();

        // Then
        static_assert(res.values<unsigned int>() == 43);
    }
    {	// 4.
        // Given
        constexpr auto fmt = stdx::details::format_string<"String view of: {%s}!">{};
        constexpr auto src = stdx::details::fixed_string{"String view of: Hello World!"};

        // When
        constexpr auto res = stdx::scan<fmt, src, std::string_view>();

        // Then
        static_assert(res.values<std::string_view>() == "Hello World");
    }
    {	// 5.
        // Given
        constexpr auto fmt = stdx::details::format_string<"Say {%s} {%u} times!">{};
        constexpr auto src = stdx::details::fixed_string{"Say Qu 3 times!"};

        // When
        constexpr auto res = stdx::scan<fmt, src, std::string_view, unsigned int>();

        // Then
        static_assert(res.values<std::string_view>() == "Qu");
        static_assert(res.values<unsigned int>() == 3);
    }
    {	// 6.
        // Given
        constexpr auto fmt = stdx::details::format_string<"My name is {%s} {%s}!">{};
        constexpr auto src = stdx::details::fixed_string{"My name is Mikhail Zykov!"};

        // When
        constexpr auto res = stdx::scan<fmt, src, std::string_view, std::string_view>();

        // Then
        static_assert(res.values<0>() == "Mikhail");
        static_assert(res.values<1>() == "Zykov");
    }
    {	// 7.
        // Given
        constexpr auto fmt = stdx::details::format_string<"Integer: {%d}">{};
        constexpr auto src = stdx::details::fixed_string{"Integer: 00000000"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int>();

        // Then
        static_assert(res.values<int>() == 0);
    }
    {	// 8.
        // Given
        constexpr auto fmt = stdx::details::format_string<"Integer: {%d}">{};
        constexpr auto src = stdx::details::fixed_string{"Integer: -00"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int>();

        // Then
        static_assert(res.values<int>() == 0);
    }
    {	// 9.
        // Given
        constexpr auto fmt = stdx::details::format_string<"Integer: {%d}">{};
        constexpr auto src = stdx::details::fixed_string{"Integer: -00423"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int>();

        // Then
        static_assert(res.values<int>() == -423);
    }
    {	// 10.
        // Given
        constexpr auto fmt = stdx::details::format_string<"Max Integer: {%d}">{};
        constexpr auto src = stdx::details::fixed_string{"Max Integer: 2147483647"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int32_t>();

        // Then
        static_assert(res.values<int32_t>() == 2147483647);
    }
    {	// 11.
        // Given
        constexpr auto fmt = stdx::details::format_string<"Min Integer: {%d}">{};
        constexpr auto src = stdx::details::fixed_string{"Min Integer: -2147483648"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int32_t>();

        // Then
        static_assert(res.values<int32_t>() == -2147483648);
    }
    {	// 12.
        // Given
        constexpr auto fmt = stdx::details::format_string<"{} {} {} {} {}">{};
        constexpr auto src = stdx::details::fixed_string{"-2147483648 2147483647 00 -01 18446744073709551615"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int32_t, int32_t, uint8_t, int8_t, uint64_t>();

        // Then
        static_assert(res.values<0>() == -2147483648);
        static_assert(res.values<1>() == 2147483647);
        static_assert(res.values<2>() == 0);
        static_assert(res.values<3>() == -1);
        static_assert(res.values<4>() == 18446744073709551615UL);
    }
    return 0;
}
