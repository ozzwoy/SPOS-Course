//
// Created by Citrus on 19.10.2020.
//

#ifndef SPOS_LAB_1_VARIADIC_OPERATION_H
#define SPOS_LAB_1_VARIADIC_OPERATION_H

#include <algorithm>
#include <initializer_list>
#include <numeric>

namespace var_operation {

    template <typename T>
    struct variadic_operation {
        virtual T execute(std::initializer_list<T> arg_list) = 0;
    };

    struct conjunction : public variadic_operation<bool> {
        bool execute(std::initializer_list<bool> arg_list) override {
            for (bool arg : arg_list) {
                if (!arg) return false;
            }
            return true;
        }
    };

    struct disjunction : public variadic_operation<bool> {
        bool execute(std::initializer_list<bool> arg_list) override {
            for (bool arg : arg_list) {
                if (arg) return true;
            }
            return false;
        }
    };

    struct minimum : public variadic_operation<int> {
        int execute(std::initializer_list<int> arg_list) override {
            return std::min(arg_list);
        }
    };

    struct multiply : public variadic_operation<double> {
        double execute(std::initializer_list<double> arg_list) override {
            return std::accumulate(arg_list.begin(), arg_list.end(), 1,
                                   [](double left, double right) { return left * right; });
        }
    };

}

#endif //SPOS_LAB_1_VARIADIC_OPERATION_H
