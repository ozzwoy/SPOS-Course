//
// Created by Citrus on 22.10.2020.
//

#ifndef SPOS_LAB_1_EXTENDED_MANAGER_H
#define SPOS_LAB_1_EXTENDED_MANAGER_H

#include "boost/process.hpp"
#include "../cancellator/cancellator.h"
#include "../cancellator/simple_cancellator.h"
#include "../function/demofuncs.h"
#include "../operation/variadic_operation.h"
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <xutility>
namespace bp = boost::process;
namespace varop = variadic_operation;

template <spos::lab1::demo::op_group Group>
class extended_manager {
private:
    typedef typename spos::lab1::demo::op_group_info_holder<Group>::data_type data_type;
    typedef std::chrono::time_point<std::chrono::system_clock> time_point;
    static const data_type ZERO = spos::lab1::demo::op_group_info_holder<Group>::ZERO;
    std::vector<std::string> func_names;
    std::string exe_path;
    size_t num_of_funcs;
    std::vector<data_type> values;
    std::vector<bool> read;
    varop::variadic_operation<Group>* operation;
    cancellator* _cancellator;

    void on_returns(size_t i) {
        std::cout << "Function " + func_names[i] + " returns " << values[i] << std::endl;
    }

    void on_hangs(size_t i) {
        std::cout << "Function " + func_names[i] + " hangs" << std::endl;
    }

    void show_result(long elapsed_time, bool short_circuit, bool is_cancelled) {
        if (!is_cancelled) {
            if (_cancellator->has_prompt()) {
                while (_cancellator->is_prompt_on());
            }
            _cancellator->finish();
        }

        std::cout << std::endl;

        for (size_t i = 0; i < num_of_funcs; i++) {
            if (read[i]) on_returns(i);
            else on_hangs(i);
        }

        if (short_circuit) {
            std::cout << "Short circuit operation." << std::endl;
            std::cout << "Result: " << ZERO << std::endl;
        } else if (is_cancelled) {
            std::cout << "Result: undefined" << std::endl;
        } else {
            std::cout << "Result: " << operation->execute(values) << std::endl;
        }

        std::cout << "Elapsed time: " << elapsed_time << "s" << std::endl;
    }

public:
    extended_manager(std::vector<std::string> const &func_names, std::string exe_path,
                  varop::variadic_operation<Group>* operation, cancellator* _cancellator = nullptr) :
            operation(operation), _cancellator(_cancellator), func_names(func_names), exe_path(std::move(exe_path)),
            values(func_names.size()), read(func_names.size(), false){
        if (func_names.empty()) {
            throw std::invalid_argument("You passed no functions to manager.");
        }
        num_of_funcs = func_names.size();
        if (!_cancellator) this->_cancellator = new simple_cancellator();\
    }

    virtual ~extended_manager() {
        delete operation;
        delete _cancellator;
    }

    void run(int x) {
        std::vector<bp::opstream> fin(num_of_funcs);
        std::vector<bp::ipstream> fout(num_of_funcs);

        time_point start, end;

        for (size_t i = 0; i < fin.size(); i++) {
            fin[i] << func_names[i] << std::endl << spos::lab1::demo::to_string(Group) << std::endl << x << std::endl;
        }

        std::vector<bp::child> processes;
        processes.reserve(num_of_funcs);
        for (size_t i = 0; i < num_of_funcs; i++) {
            processes.emplace_back(exe_path, bp::std_out > fout[i], bp::std_in < fin[i]);
        }

        _cancellator->start();
        std::cout << "Computation runs. Wait for result..." << std::endl;
        start = std::chrono::system_clock::now();

        std::vector<size_t> ready_func_nums;
        size_t num_ready = 0;

        while (true) {
            ready_func_nums.clear();

            for (size_t i = 0; i < processes.size(); i++) {
                if (!processes[i].running() && !read[i]) {
                    data_type result;
                    fout[i] >> result;
                    values[i] = result;
                    read[i] = true;
                    ready_func_nums.push_back(i);
                    num_ready++;
                }
            }

            if (num_ready == processes.size()) {
                end = std::chrono::system_clock::now();
                show_result(std::chrono::duration_cast<std::chrono::seconds>(end - start).count(),
                            false, false);
                return;
            }

            if (!ready_func_nums.empty()) {
                for (size_t i : ready_func_nums) {
                    if (values[i] == ZERO) {
                        end = std::chrono::system_clock::now();
                        show_result(std::chrono::duration_cast<std::chrono::seconds>(end - start).count(),
                                    true, false);
                        return;
                    }
                }
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

#endif //SPOS_LAB_1_EXTENDED_MANAGER_H
