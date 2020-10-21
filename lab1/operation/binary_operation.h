//
// Created by Citrus on 18.10.2020.
//

#ifndef SPOS_LAB_1_BINARY_OPERATION_H
#define SPOS_LAB_1_BINARY_OPERATION_H

#include <algorithm>

namespace bin_operation {

    template <typename T>
    struct binary_operation {
        virtual T execute(T first, T second) = 0;
    };

    struct conjunction : public binary_operation<bool> {
        bool execute(bool first, bool second) override {
            return first && second;
        }
    };

    struct disjunction : public binary_operation<bool> {
        bool execute(bool first, bool second) override {
            return first || second;
        }
    };

    struct minimum : public binary_operation<int> {
        int execute(int first, int second) override {
            return std::min(first, second);
        }
    };

    struct multiply : public binary_operation<double> {
        double execute(double first, double second) override {
            return first * second;
        }
    };

}

#endif //SPOS_LAB_1_BINARY_OPERATION_H
