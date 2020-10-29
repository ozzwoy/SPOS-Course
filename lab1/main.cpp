#include "operation/binary_operation.h"
#include "operation/variadic_operation.h"
#include "boost/process.hpp"
#include "manager/basic_manager.h"
#include "manager/extended_manager.h"
#include "cancellator/periodic_cancellator.h"
#include <iostream>
#include <string>
namespace bp = boost::process;

int read_x() {
    int x;
    std::cout << "Enter x: ";
    while (!(std::cin >> x)) {
        std::cout << std::endl << "Enter x: ";
    }
    std::cout << std::endl << std::endl;
    return x;
}

void basic_test() {
    int x = read_x();
    std::string exe_path = boost::filesystem::current_path().string() + R"(\function\function_manager\function_manager)";

    basic_manager<spos::lab1::demo::op_group::INT> manager("f", "g", exe_path,
                                                           new binary_operation::min(),
                                                           new periodic_cancellator(15));
    manager.run(x);
}

void extended_test() {
    int x = read_x();
    std::string exe_path = boost::filesystem::current_path().string() + R"(\function\function_manager\function_manager)";

    extended_manager<spos::lab1::demo::op_group::INT> manager({"f", "g", "m", "n"}, exe_path,
                                                              new variadic_operation::min(),
                                                              new periodic_cancellator(5));
    manager.run(x);
}

int main() {
    basic_test();
    //extended_test();
    return 0;
}
