//
// Created by Citrus on 19.10.2020.
//

#ifndef SPOS_LAB_1_VARIADIC_OPERATION_H
#define SPOS_LAB_1_VARIADIC_OPERATION_H

#include "../function/demofuncs.h"
#include <algorithm>
#include <numeric>
#include <vector>

namespace variadic_operation {

    template <spos::lab1::demo::op_group group>
    struct variadic_operation;

    template<>
    struct variadic_operation<spos::lab1::demo::op_group::AND> {
        bool execute(std::vector<bool> const &args) {
            return std::ranges::all_of(args.begin(), args.end(), [](bool v){ return v; });
        }
    };

    template<>
    struct variadic_operation<spos::lab1::demo::op_group::OR> {
        bool execute(std::vector<bool> args) {
            return std::ranges::any_of(args.begin(), args.end(), [](bool v){ return v; });
        }
    };

    template<>
    struct variadic_operation<spos::lab1::demo::op_group::INT> {
        virtual int execute(std::vector<int> args) = 0;
    };

    struct min : public variadic_operation<spos::lab1::demo::op_group::INT> {
        int execute(std::vector<int> args) override {
            return *std::min_element(args.begin(), args.end());
        }
    };

    template<>
    struct variadic_operation<spos::lab1::demo::op_group::DOUBLE> {
        virtual double execute(std::vector<double> args) = 0;
    };

    struct multiply : public variadic_operation<spos::lab1::demo::op_group::DOUBLE> {
        double execute(std::vector<double> args) override {
            return std::accumulate(args.begin(), args.end(), 1,
                                   [](double left, double right) { return left * right; });
        }
    };

}

#endif //SPOS_LAB_1_VARIADIC_OPERATION_H
