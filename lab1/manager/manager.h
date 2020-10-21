//
// Created by Citrus on 21.10.2020.
//

#ifndef SPOS_LAB_1_MANAGER_H
#define SPOS_LAB_1_MANAGER_H

#include "boost/process.hpp"
#include "../cancellator/cancellator.h"
#include "../cancellator/simple_cancellator.h"
#include "../demofuncs/demofuncs.h"
#include "../operation/group_operation.h"
#include <chrono>
#include <iostream>
#include <string>
namespace bp = boost::process;

template <spos::lab1::demo::op_group Group>
class manager {
private:
    typedef typename spos::lab1::demo::op_group_info_holder<Group>::data_type data_type;
    typedef std::chrono::time_point<std::chrono::system_clock> time_point;
    static const data_type ZERO = spos::lab1::demo::op_group_info_holder<Group>::ZERO;
    std::string f_path;
    std::string g_path;
    cancellator* _cancellator;

    void on_hangs(std::string const &hang_func, std::string const &non_hang_func, data_type value,
                  time_point start, time_point end) {
        std::cout << ((value == ZERO) ? ("0. " + non_hang_func + "() returns 0, short-circuit operation.") :
                      ("undefined. " + hang_func + "() hangs."));
        std::cout << "\nElapsed time: " <<
                  std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms.";
    }

    void on_cancelled(bp::child &f_proc, bp::ipstream &fout, bp::child &g_proc, bp::ipstream &gout,
                      time_point start, time_point end) {
        data_type f_value, g_value;

        if (f_proc.running() && g_proc.running()) {
            std::cout << "undefined. Both f() and g() hang.";
        } else if (f_proc.running()) {
            gout >> g_value;
            on_hangs("f", "g", g_value, start, end);
        } else if (g_proc.running()) {
            fout >> f_value;
            on_hangs("g", "f", f_value, start, end);
        } else {
            fout >> f_value;
            gout >> g_value;
            group_operation::group_operation<Group> operation;
            std::cout << operation.execute(f_value, g_value);
            std::cout << "\nElapsed time: " <<
                    std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms.";
        }

        f_proc.terminate();
        g_proc.terminate();
    }

    void on_returns_zero(std::string const &func, time_point start, time_point end) {
        while (_cancellator->is_shutdown_process_on());
        _cancellator->finish();
        system("cls");
        std::cout << "0. " + func + "() returns 0, short-circuit operation.";
        std::cout << "\nElapsed time: " <<
                  std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms.";
    }

public:
    manager(std::string f_path, std::string g_path, cancellator* _cancellator = nullptr) :
            _cancellator(_cancellator), f_path(std::move(f_path)), g_path(std::move(g_path)) {
        if (!_cancellator) this->_cancellator = new simple_cancellator('q');
    }

    void run(int x) {
        bp::opstream fin, gin;
        bp::ipstream fout, gout;

        data_type f_value, g_value;
        bool f_read = false;

        std::chrono::time_point<std::chrono::system_clock> start, end;

        fin << spos::lab1::demo::to_string(Group) << std::endl << x << std::endl;
        gin << spos::lab1::demo::to_string(Group) << std::endl << x << std::endl;

        bp::child f_proc(f_path, bp::std_out > fout, bp::std_in < fin);
        bp::child g_proc(g_path, bp::std_out > gout, bp::std_in < gin);

        _cancellator->start();
        std::cout << "Computation runs. Wait for result.\nResult: ";
        start = std::chrono::system_clock::now();

        while (f_proc.running() && g_proc.running() && !_cancellator->is_cancelled());

        end = std::chrono::system_clock::now();

        if (_cancellator->is_cancelled()) {
            system("cls");
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
                f_read = true;
            }
        }

        while ((f_proc.running() || g_proc.running()) && !_cancellator->is_cancelled());
        end = std::chrono::system_clock::now();

        if (_cancellator->is_cancelled()) {
            system("cls");
            on_cancelled(f_proc, fout, g_proc, gout, start, end);
            return;
        } else {
            while (_cancellator->is_shutdown_process_on());
            _cancellator->finish();
            system("cls");

            if (f_read) gout >> g_value;
            else fout >> f_value;

            group_operation::group_operation<Group> operation;
            std::cout << operation.execute(f_value, g_value);
            std::cout << "\nElapsed time: " <<
                      std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << "s.";
        }
    }
};

#endif //SPOS_LAB_1_MANAGER_H
