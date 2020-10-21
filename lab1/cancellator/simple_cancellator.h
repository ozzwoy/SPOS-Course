//
// Created by Citrus on 20.10.2020.
//

#ifndef SPOS_LAB_1_SIMPLE_CANCELLATOR_H
#define SPOS_LAB_1_SIMPLE_CANCELLATOR_H

#include "cancellator.h"
#include <conio.h>
#include <thread>

class simple_cancellator : public cancellator {
private:
    char escape_key;
    bool _is_cancelled;
    bool _is_off;

    void run() {
        int ch;

        while (!_is_off && _getch() != escape_key);
        if (!_is_off) _is_cancelled = true;
    }

public:
    explicit simple_cancellator(char escape_key) : escape_key(escape_key), _is_cancelled(false), _is_off(false) {}

    void start() override {
        auto th = std::thread(&simple_cancellator::run, this);
        th.detach();
    }

    bool is_shutdown_process_on() override { return false; }

    bool is_cancelled() override { return _is_cancelled; }

    void finish() override { _is_off = true; }
};

#endif //SPOS_LAB_1_SIMPLE_CANCELLATOR_H
