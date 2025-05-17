#pragma once

#include <array>
#include <expected>
#include <utility>

#include "types.hpp"

namespace stdx::details {

// Шаблонный класс для хранения форматирующей строчки и ее особенностей
template<fixed_string fmt>
class format_string {
public:
    constexpr static fixed_string format = fmt;

    consteval std::expected<std::size_t, parse_error> get_number_placeholders();
    constexpr static std::size_t number_placeholders = get_number_placeholders();

    template<std::size_t Size>
    consteval std::array<std::pair<std::size_t, std::size_t>, Size> get_placeholder_positions();

    template<std::size_t Size = number_placeholders>
    constexpr static std::array<std::pair<std::size_t, std::size_t>, Size> placeholder_positions = get_placeholder_positions<Size>();
};

// Пользовательский литерал
template<fixed_string str>
constexpr format_string<str> operator""_fs() {
    return format_string<str>{};
}

// Функция для получения количества плейсхолдеров и проверки корректности формирующей строки
// Функция закомментирована, так как еще не реализованы классы, которые она использует
template<fixed_string fmt>
consteval std::expected<std::size_t, parse_error> format_string<fmt>::get_number_placeholders() {
    constexpr std::size_t N = fmt.size();

    if (!N)
        return 0;

    std::size_t placeholder_count = 0;
    std::size_t pos = 0;
    const std::size_t size = N - 1; // -1 для игнорирования нуль-терминатора

    while (pos < size) {
        // Пропускаем все символы до '{'
        if (fmt.data[pos] != '{') {
            ++pos;
            continue;
        }

        // Проверяем незакрытый плейсхолдер
        if (pos + 1 >= size) {
            return std::unexpected(parse_error{"Unclosed last placeholder"});
        }

        // Начало плейсхолдера
        ++placeholder_count;
        ++pos;

        // Проверка спецификатора формата
        if (fmt.data[pos] == '%') {
            ++pos;
            if (pos >= size) {
                return std::unexpected(parse_error{"Unclosed last placeholder"});
            }

            // Проверяем допустимые спецификаторы
            const char spec = fmt.data[pos];
            constexpr char valid_specs[] = {'d', 'u', 'f', 's'};
            bool valid = false;

            for (const char s : valid_specs) {
                if (spec == s) {
                    valid = true;
                    break;
                }
            }

            if (!valid) {
                return std::unexpected(parse_error{"Invalid specifier."});
            }
            ++pos;
        }

        // Проверяем закрывающую скобку
        if (pos >= size || fmt.data[pos] != '}') {
            return std::unexpected(parse_error{"\'}\' hasn't been found in appropriate place"});
        }
        ++pos;
    }

    return placeholder_count;
}

// Функция для получения позиций плейсхолдеров
template<fixed_string fmt>
template<std::size_t Size>
consteval std::array<std::pair<std::size_t, std::size_t>, Size> format_string<fmt>::get_placeholder_positions() {
    std::array<std::pair<std::size_t, std::size_t>, Size> result;
    constexpr std::size_t size = fmt.size() - 1; // -1 для игнорирования нуль-терминатора

    std::size_t placeholder_num = 0;
    std::size_t pos = 0;

    while (pos < size) {
        // Пропускаем все символы до '{'
        if (fmt.data[pos] != '{') {
            ++pos;
            continue;
        }

        // Начало плейсхолдера
        result[placeholder_num] = { 0, 0 };
        std::get<0>(result[placeholder_num]) = pos;
        ++pos;

        // Проверка спецификатора формата
        if (fmt.data[pos] == '%') {
            pos += 2; // length("%d") = 2
        }

        std::get<1>(result[placeholder_num]) = pos;
        ++placeholder_num;
        ++pos;
    }

    return result;
}

} // namespace stdx::details
