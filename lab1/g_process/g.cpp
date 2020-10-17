#include "../demofuncs/demofuncs.h"
#include <iostream>

int main() {
    int test_case;
    std::cin >> test_case;
	std::cout << spos::lab1::demo::g_func< spos::lab1::demo::op_group::INT >(test_case);
	return 0;
}