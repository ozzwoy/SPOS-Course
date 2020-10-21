#include "boost/process.hpp"
#include "demofuncs/demofuncs.h"
#include "manager/manager.h"
#include "operation/group_operation.h"
#include "operation/binary_operation.h"
#include "operation/variadic_operation.h"
#include <iostream>
#include <string>
namespace bp = boost::process;

int main() {
    /*bp::opstream fin, gin;
    bp::ipstream fout, gout;

    spos::lab1::demo::op_group group = spos::lab1::demo::op_group::INT;
    int test_case = 0;*/

    auto exe_path = boost::filesystem::current_path();
    auto proj_path = exe_path.parent_path();
    std::string f_exe_path = proj_path.string() + "\\f_process\\cmake-build-release\\f";
    std::string g_exe_path = proj_path.string() + "\\g_process\\cmake-build-release\\g";

    manager<spos::lab1::demo::op_group::INT> manager(f_exe_path, g_exe_path);
    manager.run(0);

    /*bp::child f_proc(f_exe_path, bp::std_out > fout, bp::std_in < fin);
    bp::child g_proc(g_exe_path, bp::std_out > gout, bp::std_in < gin);

    fin << spos::lab1::demo::to_string(group) << std::endl << test_case << std::endl;
    gin << spos::lab1::demo::to_string(group) << std::endl << test_case << std::endl;

    while (f_proc.running() || g_proc.running());

    int f_value, g_value;
    fout >> f_value;
    gout >> g_value;

    f_proc.wait();
    g_proc.wait();

    std::cout << "Value of f() is " << f_value << std::endl;
    std::cout << "Value of g() is " << g_value << std::endl;

    group_operation::group_operation<spos::lab1::demo::op_group::INT> operation;
    std::cout << "Result: " << operation.execute(f_value, g_value);*/

    return 0;
}
