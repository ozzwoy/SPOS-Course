//
// Created by Citrus on 23.10.2020.
//

#include "../demofuncs.h"
#include <iostream>
#include <string>

template<spos::lab1::demo::op_group group>
void display_result(std::string const &func, int test_case) {
    if (func == "f") {
        std::cout << spos::lab1::demo::f_func<group>(test_case);
    } else if (func == "g") {
        std::cout << spos::lab1::demo::g_func<group>(test_case);
    } else if (func == "m") {
        std::cout << spos::lab1::demo::m_func<group>(test_case);
    } else if (func == "n") {
        std::cout << spos::lab1::demo::n_func<group>(test_case);
    }
}

int main() {
    std::string func, group_str;
    int test_case;

    std::cin >> func >> group_str >> test_case;

    if (group_str == spos::lab1::demo::to_string(spos::lab1::demo::op_group::AND)) {
        display_result<spos::lab1::demo::op_group::AND>(func, test_case);
    } else if (group_str == spos::lab1::demo::to_string(spos::lab1::demo::op_group::OR)) {
        display_result<spos::lab1::demo::op_group::OR>(func, test_case);
    } else if (group_str == spos::lab1::demo::to_string(spos::lab1::demo::op_group::INT)) {
        display_result<spos::lab1::demo::op_group::INT>(func, test_case);
    } else if (group_str == spos::lab1::demo::to_string(spos::lab1::demo::op_group::DOUBLE)) {
        display_result<spos::lab1::demo::op_group::DOUBLE>(func, test_case);
    }

    return 0;
}