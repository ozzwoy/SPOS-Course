#include "boost/process.hpp"
#include "demofuncs/demofuncs.h"
#include "manager/manager.h"
#include <string>
namespace bp = boost::process;

int main() {
    auto exe_path = boost::filesystem::current_path();
    auto proj_path = exe_path.parent_path();
    std::string f_exe_path = proj_path.string() + "\\f_process\\cmake-build-release\\f";
    std::string g_exe_path = proj_path.string() + "\\g_process\\cmake-build-release\\g";

    manager<spos::lab1::demo::op_group::INT> manager(f_exe_path, g_exe_path);
    manager.run(2);

    return 0;
}
