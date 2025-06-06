#pragma once

#include <charconv>
#include <concepts>
#include <cstdint>
#include <expected>
#include <limits>
#include <optional>
#include <string>
#include <system_error>

#include "format_string.hpp"
#include "types.hpp"

namespace stdx::details {

// Шаблонная функция, возвращающая пару позиций в строке с исходными данными, соответствующих I-ому плейсхолдеру
template <std::size_t I, format_string fmt, fixed_string source>
consteval auto get_current_source_for_parsing() {
    static_assert(I >= 0 && I < fmt.number_placeholders, "Invalid placeholder index");

    constexpr auto to_sv = [](const auto &fs) {
        return std::string_view(fs.data, fs.size() - 1);
    };

    constexpr auto fmt_sv = to_sv(fmt.fmt);
    constexpr auto src_sv = to_sv(source);
    constexpr auto &positions = fmt.placeholder_positions;

    // Получаем границы текущего плейсхолдера в формате
    constexpr auto pos_i = positions[I];
    constexpr size_t fmt_start = pos_i.first, fmt_end = pos_i.second;

    // Находим начало в исходной строке
    constexpr auto src_start = [&]{
        if constexpr (I == 0) {
            return fmt_start;
        } else {
            // Находим конец предыдущего плейсхолдера в исходной строке
            constexpr auto prev_bounds = get_current_source_for_parsing<I-1, fmt, source>();
            const auto prev_end = prev_bounds.second;

            // Получаем разделитель между текущим и предыдущим плейсхолдерами
            constexpr auto prev_fmt_end = positions[I-1].second;
            constexpr auto sep = fmt_sv.substr(prev_fmt_end + 1, fmt_start - (prev_fmt_end + 1));

            // Ищем разделитель после предыдущего значения
            auto pos = src_sv.find(sep, prev_end);
            return pos != std::string_view::npos ? pos + sep.size() : src_sv.size();
        }
    }();

    // Находим конец в исходной строке
    constexpr auto src_end = [&]{
        // Получаем разделитель после текущего плейсхолдера
        if constexpr(fmt_end == (fmt_sv.size() - 1)) {
            return src_sv.size();
        }
        constexpr auto sep = fmt_sv.substr(fmt_end + 1,
            (I < fmt.number_placeholders - 1)
                ? positions[I+1].first - (fmt_end + 1)
                : fmt_sv.size() - (fmt_end + 1));
        // Ищем разделитель после текущего значения
        constexpr auto pos = src_sv.find(sep, src_start);
        return pos != std::string_view::npos ? pos : src_sv.size();
    }();

    return std::pair{src_start, src_end};
}

template <fixed_string sval, std::integral T>
consteval std::expected<T, parse_error> parse_value() {
    T res = 0, sign = 1, limit = std::numeric_limits<T>::max();
    std::size_t i = 0;

    if constexpr(std::is_signed<T>::value) {
        if (sval.size() > 0 && sval.data[0] == '-') {
            sign = -1;
            i = 1;
            limit = std::numeric_limits<T>::min();
        }
    }

    for (; i < sval.size(); ++i) {
        if (sval.data[i] == '\0')
            break;

        T digit = static_cast<T>(sval.data[i] - '0');

        if (sign > 0) {
            if (res > limit / 10 || res * 10 > limit - digit) {
                return std::unexpected(parse_error{"Positive integer overflow"});
            }
            res = res * 10 + digit;
        }
        else {
            if (res < limit / 10 || res * 10 < limit + digit) {
                return std::unexpected(parse_error{"Negative integer overflow"});
            }
            res = res * 10 - digit;
        }
    }

    return res;
}

template <fixed_string sval, typename = std::string_view>
consteval std::expected<std::string_view, parse_error> parse_value() {
    return std::string_view(sval.data, sval.size());
}

template <typename T>
concept fmt_types = std::same_as<T, std::string_view> || std::is_integral<T>::value;

// Шаблонная функция, выполняющая преобразования исходных данных в конкретный тип на основе I-го плейсхолдера
template <std::size_t I, format_string fmt, fixed_string source, fmt_types T>
consteval T parse_input() {
    constexpr std::size_t fmt_b = std::get<0>(fmt.placeholder_positions[I]);
    constexpr std::size_t fmt_e = std::get<1>(fmt.placeholder_positions[I]);
    constexpr auto spec = fixed_string<fmt_e - fmt_b>{ fmt.fmt.data + fmt_b, fmt.fmt.data + fmt_e };

    if constexpr(spec.size() > 2) {
        if constexpr(spec.data[2] == 'd') {
            static_assert(std::is_integral<T>::value && std::is_signed<T>::value, "Type meets the ‘%d’ specifier");
        }
        else if constexpr(spec.data[2] == 'u') {
            static_assert(std::is_integral<T>::value && !std::is_signed<T>::value, "Type meets the ‘%u’ specifier");
        }
        else if constexpr(spec.data[2] == 's') {
            static_assert(std::same_as<T, std::string_view>, "Type meets the ‘%s’ specifier");
        }
    }

    constexpr auto src = get_current_source_for_parsing<I, fmt, source>();
    constexpr std::size_t src_b = std::get<0>(src);
    constexpr std::size_t src_e = std::get<1>(src);
    constexpr auto source_data_b = source.data + src_b;
    constexpr auto source_data_e = source.data + src_e;

    return *parse_value<fixed_string<src_e - src_b>{ source_data_b, source_data_e }, T>();
}

} // namespace stdx::details
