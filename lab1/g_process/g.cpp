#include "../demofuncs/demofuncs.h"
#include <iostream>

int main() {
    std::string group_str;
    int test_case;

    std::cin >> group_str;
    std::cin >> test_case;

    if (group_str == to_string(spos::lab1::demo::op_group::AND)) {
        std::cout << spos::lab1::demo::g_func< spos::lab1::demo::op_group::AND >(test_case);
    } else if (group_str == to_string(spos::lab1::demo::op_group::OR)) {
        std::cout << spos::lab1::demo::g_func< spos::lab1::demo::op_group::OR >(test_case);
    } else if (group_str == to_string(spos::lab1::demo::op_group::INT)) {
        std::cout << spos::lab1::demo::g_func< spos::lab1::demo::op_group::INT >(test_case);
    } else if (group_str == to_string(spos::lab1::demo::op_group::DOUBLE)) {
        std::cout << spos::lab1::demo::g_func< spos::lab1::demo::op_group::DOUBLE >(test_case);
    }

    return 0;
}