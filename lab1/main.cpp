#include "operation/binary_operation.h"
#include "boost/process.hpp"
#include "function/demofuncs.h"
#include "manager/basic_manager.h"
#include "cancellator/periodic_cancellator.h"
#include <string>
namespace bp = boost::process;

void basic_test() {
    std::string exe_path = boost::filesystem::current_path().string() + R"(\function\function_manager\function_manager)";

    basic_manager<spos::lab1::demo::op_group::INT> manager("f", "g", exe_path,
                                                           new binary_operation::min(),
                                                           new periodic_cancellator(15));
    manager.run(0);
}

int main() {
    basic_test();
    return 0;
}
