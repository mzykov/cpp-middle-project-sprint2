#pragma once

#include <array>
#include <expected>
#include <utility>

#include "types.hpp"

namespace stdx::details {

// Шаблонный класс для хранения форматирующей строчки и ее особенностей
template<fixed_string str>
class format_string {
public:
    static constexpr fixed_string fmt = str;

    static consteval std::expected<std::size_t, parse_error> get_number_placeholders();
    static constexpr std::size_t number_placeholders = *get_number_placeholders();

    static consteval std::array<std::pair<std::size_t, std::size_t>, number_placeholders> get_placeholder_positions();
    static constexpr std::array<std::pair<std::size_t, std::size_t>, number_placeholders> placeholder_positions = get_placeholder_positions();
};

// Функция для получения количества плейсхолдеров и проверки корректности формирующей строки
template<fixed_string str>
consteval std::expected<std::size_t, parse_error> format_string<str>::get_number_placeholders() {
    constexpr std::size_t N = str.size();

    if (!N)
        return 0;

    std::size_t placeholder_count = 0;
    std::size_t pos = 0;
    const std::size_t size = N - 1; // -1 для игнорирования нуль-терминатора

    while (pos < size) {
        // Пропускаем все символы до '{'
        if (str.data[pos] != '{') {
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
        if (str.data[pos] == '%') {
            ++pos;
            if (pos >= size) {
                return std::unexpected(parse_error{"Unclosed last placeholder"});
            }

            // Проверяем допустимые спецификаторы
            const char spec = str.data[pos];
            constexpr char valid_specs[] = {'d', 'u', 's'};
            bool valid = false;

            for (const char s : valid_specs) {
                if (spec == s) {
                    valid = true;
                    break;
                }
            }

            if (!valid) {
                return std::unexpected(parse_error{"Invalid specifier"});
            }
            ++pos;
        }

        // Проверяем закрывающую скобку
        if (pos >= size || str.data[pos] != '}') {
            return std::unexpected(parse_error{"\'}\' hasn't been found in appropriate place"});
        }
        ++pos;
    }

    return placeholder_count;
}

// Пользовательский литерал
template <fixed_string str>
constexpr format_string<str> operator""_fs() {
    return format_string<str>{};
}

// Функция для получения позиций плейсхолдеров
template<fixed_string str>
consteval std::array<std::pair<std::size_t, std::size_t>, format_string<str>::number_placeholders> format_string<str>::get_placeholder_positions() {
    std::array<std::pair<std::size_t, std::size_t>, format_string<str>::number_placeholders> result;
    constexpr std::size_t size = str.size() - 1; // -1 для игнорирования нуль-терминатора

    std::size_t placeholder_num = 0;
    std::size_t pos = 0;

    while (pos < size) {
        // Пропускаем все символы до '{'
        if (str.data[pos] != '{') {
            ++pos;
            continue;
        }

        // Начало плейсхолдера
        result[placeholder_num] = { 0, 0 };
        std::get<0>(result[placeholder_num]) = pos;
        ++pos;

        // Проверка спецификатора формата
        if (str.data[pos] == '%') {
            pos += 2; // length("%d") = 2
        }

        std::get<1>(result[placeholder_num]) = pos;
        ++placeholder_num;
        ++pos;
    }

    return result;
}

} // namespace stdx::details
