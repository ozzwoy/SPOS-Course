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
    std::string first_func_name;
    std::string second_func_name;
    std::string exe_path;
    binop::binary_operation<Group>* operation;
    cancellator* _cancellator;

    void on_hangs(std::string const &hang_func, std::string const &non_hang_func, data_type value,
                  time_point start, time_point end) {
        std::cout << "Result: ";
        std::cout << ((value == ZERO) ? ("zero. " + non_hang_func + "() returns zero value, short-circuit operation.") :
                                        ("undefined. " + hang_func + "() hangs.")) << std::endl <<
                     "Elapsed time: " <<
                     std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s.";
    }

    void on_cancelled(bp::child &f_proc, bp::ipstream &fout, bp::child &g_proc, bp::ipstream &gout,
                      time_point start, time_point end) {
        data_type f_value, g_value;

        if (f_proc.running() && g_proc.running()) {
            std::cout << "Result: undefined. Both f() and g() hang." << std::endl <<
                         "Elapsed time: " <<
                         std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s.";
        } else if (f_proc.running()) {
            gout >> g_value;
            on_hangs("f", "g", g_value, start, end);
        } else if (g_proc.running()) {
            fout >> f_value;
            on_hangs("g", "f", f_value, start, end);
        } else {
            fout >> f_value;
            gout >> g_value;

            std::cout << "Result: " << operation->execute(f_value, g_value) << std::endl <<
                         "Elapsed time: " <<
                         std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s.";
        }
    }

    void on_returns_zero(std::string const &func, time_point start, time_point end) {
        if (_cancellator->has_prompt()) while (_cancellator->is_prompt_on());
        _cancellator->finish();
        std::cout << "Result: " << "zero. " + func + "() returns zero value, short-circuit operation." << std::endl <<
                     "Elapsed time: " <<
                     std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s.";
    }

public:
    basic_manager(std::string first_func_name, std::string second_func_name, std::string exe_path,
                  binop::binary_operation<Group>* operation, cancellator* _cancellator = nullptr) :
            operation(operation), _cancellator(_cancellator), first_func_name(std::move(first_func_name)),
            second_func_name(std::move(second_func_name)), exe_path(std::move(exe_path)) {
        if (!_cancellator) this->_cancellator = new simple_cancellator();
    }

    virtual ~basic_manager() {
        delete operation;
        delete _cancellator;
    }

    void run(int x) {
        bp::opstream fin, gin;
        bp::ipstream fout, gout;

        data_type f_value, g_value;
        bool f_ready = false;

        time_point start, end;

        fin << first_func_name << std::endl << spos::lab1::demo::to_string(Group) << std::endl << x << std::endl;
        gin << second_func_name << std::endl << spos::lab1::demo::to_string(Group) << std::endl << x << std::endl;

        bp::child f_proc(exe_path, bp::std_out > fout, bp::std_in < fin);
        bp::child g_proc(exe_path, bp::std_out > gout, bp::std_in < gin);

        _cancellator->start();
        std::cout << "Computation runs. Wait for result..." << std::endl;
        start = std::chrono::system_clock::now();
        while (f_proc.running() && g_proc.running() && !_cancellator->is_cancelled());
        end = std::chrono::system_clock::now();

        if (_cancellator->is_cancelled()) {
            on_cancelled(f_proc, fout, g_proc, gout, start, end);
            return;
        } else {
            if (f_proc.running()) {
                gout >> g_value;
                if (g_value == ZERO) {
                    on_returns_zero("g", start, end);
                    return;
                }
            } else {
                fout >> f_value;
                if (f_value == ZERO) {
                    on_returns_zero("f", start, end);
                    return;
                }
                f_ready = true;
            }
        }

        while ((f_proc.running() || g_proc.running()) && !_cancellator->is_cancelled());
        end = std::chrono::system_clock::now();

        if (_cancellator->is_cancelled()) {
            on_cancelled(f_proc, fout, g_proc, gout, start, end);
            return;
        } else {
            if (_cancellator->has_prompt()) {
                while (_cancellator->is_prompt_on());
            }
            _cancellator->finish();

            if (f_ready) gout >> g_value;
            else fout >> f_value;

            std::cout << "Result: " << operation->execute(f_value, g_value) << std::endl <<
                         "Elapsed time: " <<
                         std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s.";
        }
    }
};

#endif //SPOS_LAB_1_BASIC_MANAGER_H
