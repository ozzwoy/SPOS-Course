//
// Created by Citrus on 20.10.2020.
//

#ifndef SPOS_LAB_1_SIMPLE_CANCELLATOR_H
#define SPOS_LAB_1_SIMPLE_CANCELLATOR_H

#include "cancellator.h"
#include <iostream>
#include <thread>
#include <WinUser.h>

class simple_cancellator : public cancellator {
private:
    bool _is_cancelled;
    bool _is_off;

    void run() {
        while (!_is_off && !(GetKeyState(VK_ESCAPE) & 0x8000));
        if (!_is_off) {
            _is_cancelled = true;
            std::cout << std::endl << "Computation stopped." << std::endl;
        }
    }

public:
    simple_cancellator() : _is_cancelled(false), _is_off(false) {}

    void start() override {
        auto th = std::thread(&simple_cancellator::run, this);
        th.detach();
    }

    bool has_prompt() override { return false; }

    bool is_prompt_on() override { return false; }

    bool is_cancelled() override { return _is_cancelled; }

    void finish() override { _is_off = true; }
};

#endif //SPOS_LAB_1_SIMPLE_CANCELLATOR_H
