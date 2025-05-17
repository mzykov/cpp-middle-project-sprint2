#include "scan.hpp"

int main() { 
    static_assert(
        std::get<int>(stdx::scan<stdx::details::format_string<"">{}, stdx::details::fixed_string{}, int>().data) == 42
    );
}
