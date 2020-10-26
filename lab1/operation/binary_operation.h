//
// Created by Citrus on 19.10.2020.
//

#ifndef SPOS_LAB_1_BINARY_OPERATION_H
#define SPOS_LAB_1_BINARY_OPERATION_H

#include "../function/demofuncs.h"
#include <algorithm>
namespace df = spos::lab1::demo;

namespace binary_operation {

    template <df::op_group group>
    struct binary_operation;

    template <>
    struct binary_operation<df::op_group::AND> {
        bool execute(bool first, bool second) {
            return first && second;
        }
    };

    template <>
    struct binary_operation<df::op_group::OR> {
        bool execute(bool first, bool second) {
            return first || second;
        }
    };

    template <>
    struct binary_operation<df::op_group::INT> {
        virtual int execute(int first, int second) = 0;
    };

    struct min : public binary_operation<df::op_group::INT> {
        int execute(int first, int second) override {
            return std::min(first, second);
        }
    };

    template <>
    struct binary_operation<df::op_group::DOUBLE> {
        virtual double execute(double first, double second) = 0;
    };

    struct multiplication : public binary_operation<df::op_group::DOUBLE> {
        double execute(double first, double second) override {
            return first * second;
        }
    };

    struct sum : public binary_operation<df::op_group::INT>, public binary_operation<df::op_group::DOUBLE> {
        int execute(int first, int second) override {
            return first + second;
        }

        double execute(double first, double second) override {
            return first + second;
        }
    };

}

#endif //SPOS_LAB_1_BINARY_OPERATION_H
