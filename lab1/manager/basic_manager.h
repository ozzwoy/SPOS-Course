//
// Created by Citrus on 21.10.2020.
//

#ifndef SPOS_LAB_1_BASIC_MANAGER_H
#define SPOS_LAB_1_BASIC_MANAGER_H

#include "boost/process.hpp"
#include "../cancellator/cancellator.h"
#include "../cancellator/simple_cancellator.h"
#include "../function/demofuncs.h"
#include "../operation/binary_operation.h"
#include <chrono>
#include <iostream>
#include <string>
#include <xutility>
namespace bp = boost::process;
namespace binop = binary_operation;

template <spos::lab1::demo::op_group Group>
class basic_manager {
private:
    typedef typename spos::lab1::demo::op_group_info_holder<Group>::data_type data_type;
    typedef std::chrono::time_point<std::chrono::system_clock> time_point;
    static const data_type ZERO = spos::lab1::demo::op_group_info_holder<Group>::ZERO;
    std::string exe_path;
    std::string first_func_name;
    std::string second_func_name;
    data_type f_value;
    data_type g_value;
    bool f_read;
    bool g_read;
    binop::binary_operation<Group>* operation;
    cancellator* _cancellator;

    void on_returns(std::string const &func_name) {
        std::cout << "Function " + func_name + " returns ";
        if (func_name == first_func_name) std::cout << f_value << std::endl;
        else std::cout << g_value << std::endl;
    }

    void on_hangs(std::string const &func_name) {
        std::cout << "Function " + func_name + " hangs" << std::endl;
    }

    void show_result(long elapsed_time, bool short_circuit, bool is_cancelled) {
        if (!is_cancelled) {
            if (_cancellator->has_prompt()) {
                while (_cancellator->is_prompt_on());
            }
            _cancellator->finish();
        }

        std::cout << std::endl;

        if (f_read) on_returns(first_func_name);
        else on_hangs(first_func_name);

        if (g_read) on_returns(second_func_name);
        else on_hangs(second_func_name);

        if (short_circuit) {
            std::cout << "Short circuit operation." << std::endl;
            std::cout << "Result: " << ZERO << std::endl;
        } else if (is_cancelled) {
            std::cout << "Result: undefined" << std::endl;
        } else {
            std::cout << "Result: " << operation->execute(f_value, g_value) << std::endl;
        }

        std::cout << "Elapsed time: " << elapsed_time << "s" << std::endl;
    }

public:
    basic_manager(std::string first_func_name, std::string second_func_name, std::string exe_path,
                  binop::binary_operation<Group>* operation, cancellator* _cancellator = nullptr) :
            operation(operation), _cancellator(_cancellator), first_func_name(std::move(first_func_name)),
            second_func_name(std::move(second_func_name)), exe_path(std::move(exe_path)),
            f_read(false), g_read(false) {
        if (!_cancellator) this->_cancellator = new simple_cancellator();
    }

    virtual ~basic_manager() {
        delete operation;
        delete _cancellator;
    }

    void run(int x) {
        bp::opstream fin, gin;
        bp::ipstream fout, gout;

        time_point start, end;

        fin << first_func_name << std::endl << spos::lab1::demo::to_string(Group) << std::endl << x << std::endl;
        gin << second_func_name << std::endl << spos::lab1::demo::to_string(Group) << std::endl << x << std::endl;

        bp::child f_proc(exe_path, bp::std_out > fout, bp::std_in < fin);
        bp::child g_proc(exe_path, bp::std_out > gout, bp::std_in < gin);

        _cancellator->start();
        std::cout << "Computation runs. Wait for result..." << std::endl;
        start = std::chrono::system_clock::now();

        while (true) {
            if (!f_proc.running() && !f_read) {
                fout >> f_value;
                f_read = true;
                if (f_value == ZERO) {
                    end = std::chrono::system_clock::now();
                    show_result(std::chrono::duration_cast<std::chrono::seconds>(end - start).count(),
                                true, false);
                    return;
                }
            }

            if (!g_proc.running() && !g_read) {
                gout >> g_value;
                g_read = true;
                if (g_value == ZERO) {
                    end = std::chrono::system_clock::now();
                    show_result(std::chrono::duration_cast<std::chrono::seconds>(end - start).count(),
                                true, false);
                    return;
                }
            }

            if (f_read && g_read) {
                end = std::chrono::system_clock::now();
                show_result(std::chrono::duration_cast<std::chrono::seconds>(end - start).count(),
                            false, false);
                return;
            }

            if (_cancellator->is_cancelled()) {
                end = std::chrono::system_clock::now();
                show_result(std::chrono::duration_cast<std::chrono::seconds>(end - start).count(),
                            false, true);
                return;
            }
        }
    }
};

#endif //SPOS_LAB_1_BASIC_MANAGER_H
