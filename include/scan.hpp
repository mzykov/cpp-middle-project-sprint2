#pragma once

#include <tuple>
#include <utility>

#include "parse.hpp"
#include "format_string.hpp"
#include "types.hpp"

namespace stdx {

template <details::format_string fmt, details::fixed_string source, typename... Ts, std::size_t... I>
consteval details::scan_result<Ts...> scan_impl(std::index_sequence<I...>) {
    return details::scan_result<Ts...>{
        std::make_tuple(details::parse_input<I, fmt, source, Ts>()...)
    };
}

// Главная функция
template <details::format_string fmt, details::fixed_string source, typename... Ts>
consteval details::scan_result<Ts...> scan() {
    return scan_impl<fmt, source, Ts...>(std::index_sequence_for<Ts...>{});
}

} // namespace stdx
