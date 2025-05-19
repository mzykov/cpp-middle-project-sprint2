#include "scan.hpp"

int main() { 
    static_assert(
        std::get<int>(stdx::scan<stdx::details::format_string<"Integer: {%d}">{}, stdx::details::fixed_string{"Integer: 42"}, int>().data) == 42
    );
}
