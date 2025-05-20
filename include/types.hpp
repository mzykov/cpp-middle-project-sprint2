#pragma once

#include <algorithm>
#include <tuple>

namespace stdx::details {

// Шаблонный класс, хранящий C-style строку фиксированной длины

template <std::size_t N>
struct fixed_string {
    char data[N]{};
    constexpr fixed_string() = default;
    constexpr fixed_string(const char (&str)[N]) {
        std::copy_n(str, N, data);
    }
    template<std::size_t Size, typename = typename std::enable_if<(Size < N)>::type>
    constexpr fixed_string(const char (&str)[Size]) {
        std::copy_n(str, Size, data);
        std::fill(data + Size, data + N, '\0');
    }
    constexpr fixed_string(const char *b, const char *e) {
        for (std::size_t i = 0; i < N; ++i) {
            if (b + i != e) {
                data[i] = *(b + i);
            } else {
                data[i] = '\0';
            }
        }
    }
    constexpr std::size_t size() const { return sizeof(data)/sizeof(char); }
};

// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга

// ваш код здесь
struct parse_error : fixed_string<64> {};

// Шаблонный класс для хранения результатов парсинга

template <typename... Ts>
struct scan_result {
    std::tuple<Ts...> data;

    template <std::size_t I>
    consteval auto values() const {
        return std::get<I>(data);
    }

    template <typename T>
    consteval auto values() const {
        return std::get<T>(data);
    }
};

} // namespace stdx::details
