#pragma once

#include <algorithm>
#include <tuple>

namespace stdx::details {

// Шаблонный класс, хранящий C-style строку фиксированной длины

template<typename CharT = char, std::size_t N = 0>
struct fixed_string {
    CharT data[N]{};
    constexpr fixed_string() = default;
    constexpr fixed_string(const CharT (&str)[N]) {
        std::copy_n(str, N, data);
    }
    template<std::size_t Size, typename = typename std::enable_if<(Size < N)>::type>
    constexpr fixed_string(const CharT (&str)[Size]) {
        std::copy_n(str, Size, data);
        std::fill(data + Size, data + N, '\0');
    }
    constexpr fixed_string(CharT *begin, CharT *end) {
        for (std::size_t i = 0; i < sizeof(data)/sizeof(CharT); ++i) {
            if (begin != end) {
                data[i] = *begin++;
            } else {
                data[i] = '\0';
            }
        }
    }
    constexpr std::size_t size() const { return sizeof(data)/sizeof(CharT); }
};

// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга

// ваш код здесь
struct parse_error : fixed_string<char, 64> {};

// Шаблонный класс для хранения результатов парсинга

template <typename... Ts>
struct scan_result {
    std::tuple<Ts...> data;
};

} // namespace stdx::details
