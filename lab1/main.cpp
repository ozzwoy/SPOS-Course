#include "boost/process.hpp"
#include "boost/filesystem.hpp"
#include "demofuncs/demofuncs.h"
#include <iostream>
namespace bp = boost::process;

int main() {
    bp::opstream fin, gin;
    bp::ipstream fout, gout;

    auto exe_path = boost::filesystem::current_path();
    auto proj_path = exe_path.parent_path();
    std::string f_exe_path = (std::string)proj_path.string() + "\\f_process\\cmake-build-release\\f";
    std::string g_exe_path = (std::string)proj_path.string() + "\\g_process\\cmake-build-release\\g";

    bp::child f_proc(f_exe_path, bp::std_out > fout, bp::std_in < fin);
    bp::child g_proc(g_exe_path, bp::std_out > gout, bp::std_in < gin);

    fin << "0" << std::endl;
    gin << "0" << std::endl;
    int f_value, g_value;
    while (f_proc.running() || g_proc.running());

    fout >> f_value;
    gout >> g_value;

    f_proc.wait();
    g_proc.wait();

    std::cout << "Value of f() is " << f_value << std::endl;
    std::cout << "Value of g() is " << g_value << std::endl;

    return 0;
}
