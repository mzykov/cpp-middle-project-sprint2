#include "scan.hpp"

using namespace stdx::details::literals;

int main() {
    {	// 1. Just small signed positive integer
        // Given
        constexpr auto fmt = "Integer: {%d}"_fs;
        constexpr auto src = stdx::details::fixed_string{"Integer: 42"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int>();

        // Then
        static_assert(res.values<int>() == 42);
    }
    {	// 2. Just small negative integer
        // Given
        constexpr auto fmt = "Negative integer: {%d}"_fs;
        constexpr auto src = stdx::details::fixed_string{"Negative integer: -42"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int>();

        // Then
        static_assert(res.values<int>() == -42);
    }
    {	// 3. Just small unsigned integer
        // Given
        constexpr auto fmt = "Unsigned integer: {%u}"_fs;
        constexpr auto src = stdx::details::fixed_string{"Unsigned integer: 43"};

        // When
        constexpr auto res = stdx::scan<fmt, src, unsigned int>();

        // Then
        static_assert(res.values<unsigned int>() == 43);
    }
    {	// 4. Simple string view check
        // Given
        constexpr auto fmt = "String view of: {%s}!"_fs;
        constexpr auto src = stdx::details::fixed_string{"String view of: Hello World!"};

        // When
        constexpr auto res = stdx::scan<fmt, src, std::string_view>();

        // Then
        static_assert(res.values<std::string_view>() == "Hello World");
    }
    {	// 5. String view with unsigned integer combination
        // Given
        constexpr auto fmt = "Say {%s} {%u} times!"_fs;
        constexpr auto src = stdx::details::fixed_string{"Say Qu 3 times!"};

        // When
        constexpr auto res = stdx::scan<fmt, src, std::string_view, unsigned int>();

        // Then
        static_assert(res.values<std::string_view>() == "Qu");
        static_assert(res.values<unsigned int>() == 3);
    }
    {	// 6. A pair of string views
        // Given
        constexpr auto fmt = "My name is {%s} {%s}!"_fs;
        constexpr auto src = stdx::details::fixed_string{"My name is Mikhail Zykov!"};

        // When
        constexpr auto res = stdx::scan<fmt, src, std::string_view, std::string_view>();

        // Then
        static_assert(res.values<0>() == "Mikhail");
        static_assert(res.values<1>() == "Zykov");
    }
    {	// 7. Check if string of zeros equals to integer zero
        // Given
        constexpr auto fmt = "Integer: {%d}"_fs;
        constexpr auto src = stdx::details::fixed_string{"Integer: 00000000"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int>();

        // Then
        static_assert(res.values<int>() == 0);
    }
    {	// 8. Check if string of zeros with minus sign equals to zero
        // Given
        constexpr auto fmt = "Integer: {%d}"_fs;
        constexpr auto src = stdx::details::fixed_string{"Integer: -00"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int>();

        // Then
        static_assert(res.values<int>() == 0);
    }
    {	// 9. Negative integer with leading zeros check
        // Given
        constexpr auto fmt = "Integer: {%d}"_fs;
        constexpr auto src = stdx::details::fixed_string{"Integer: -00423"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int>();

        // Then
        static_assert(res.values<int>() == -423);
    }
    {	// 10. Max 32-bit signed integer check
        // Given
        constexpr auto fmt = "Max Integer: {%d}"_fs;
        constexpr auto src = stdx::details::fixed_string{"Max Integer: 2147483647"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int32_t>();

        // Then
        static_assert(res.values<int32_t>() == 2147483647);
    }
    {	// 11. Min 32-bit signed integer check
        // Given
        constexpr auto fmt = "Min Integer: {%d}"_fs;
        constexpr auto src = stdx::details::fixed_string{"Min Integer: -2147483648"};

        // When
        constexpr auto res = stdx::scan<fmt, src, int32_t>();

        // Then
        static_assert(res.values<int32_t>() == -2147483648);
    }
    {	// 12. Collection of various integer types, including max unsigned long value
        // Given
        constexpr auto fmt = "{} {} {} {} {}"_fs;
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
