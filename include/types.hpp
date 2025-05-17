#pragma once

#include <algorithm>
#include <tuple>

namespace stdx::details {

// Шаблонный класс, хранящий C-style строку фиксированной длины

template<std::size_t N = 0>
struct fixed_string {
    char data[N + 1]; // 1 для \0 в конце
    constexpr fixed_string() = default;
    constexpr fixed_string(const char (&str)[N]) {
        std::copy(str, str + N, data);
        data[N] = '\0';
    }
    template<std::size_t Size, typename = typename std::enable_if<(Size < N)>::type>
    constexpr fixed_string(const char (&str)[Size]) {
        std::copy(str, str + Size, data);
        for (std::size_t i = Size; i < sizeof(data)/sizeof(char); ++i) {
            data[i] = '\0';
        }
    }
    constexpr fixed_string(char *begin, char *end) {
        for (std::size_t i = 0; i < sizeof(data)/sizeof(char); ++i) {
            if (begin != end) {
                data[i] = *begin++;
            } else {
                data[i] = '\0';
            }
        }
        data[N] = '\0';
    }
    constexpr std::size_t size() const { return sizeof(data); }
};

// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга

// ваш код здесь
struct parse_error : fixed_string<64> {};

// Шаблонный класс для хранения результатов парсинга

template <typename... Ts>
struct scan_result {
    std::tuple<Ts...> data;
};

} // namespace stdx::details
