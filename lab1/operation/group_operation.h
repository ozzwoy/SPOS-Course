//
// Created by Citrus on 19.10.2020.
//

#ifndef SPOS_LAB_1_GROUP_OPERATION_H
#define SPOS_LAB_1_GROUP_OPERATION_H

#include "../demofuncs/demofuncs.h"
#include <algorithm>
namespace df = spos::lab1::demo;

namespace group_operation {

    template <df::op_group group>
    struct group_operation;

    template <>
    struct group_operation<df::op_group::AND> {
        bool execute(bool first, bool second) {
            return first && second;
        }
    };

    template <>
    struct group_operation<df::op_group::OR> {
        bool execute(bool first, bool second) {
            return first || second;
        }
    };

    template <>
    struct group_operation<df::op_group::INT> {
        int execute(int first, int second) {
            return std::min(first, second);
        }
    };

    template <>
    struct group_operation<df::op_group::DOUBLE> {
        double execute(double first, double second) {
            return first * second;
        }
    };

}

#endif //SPOS_LAB_1_GROUP_OPERATION_H
