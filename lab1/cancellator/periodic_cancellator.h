//
// Created by Citrus on 22.10.2020.
//

#ifndef SPOS_LAB_1_PERIODIC_PROMPT_H
#define SPOS_LAB_1_PERIODIC_PROMPT_H

#include "cancellator.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <WinUser.h>
#include <xutility>
using namespace std::chrono_literals;

class periodic_cancellator : public cancellator {
private:
    typedef std::chrono::time_point<std::chrono::system_clock> time_point;
    const std::chrono::seconds period;
    bool _is_prompt_disabled;
    bool _is_prompt_on;
    bool _is_cancelled;
    bool _is_off;

    bool show_prompt(std::chrono::seconds const &time_passed) {
        int value;

        while (true) {
            std::cout << std::endl << "[Time passed before prompt: " << time_passed.count() << "s]" << std::endl <<
                      "Please, choose one of the following options:" << std::endl <<
                      "- enter 0 to stop computation" << std::endl <<
                      "- enter 1 to continue computation" << std::endl <<
                      "- enter 2 to continue computation without prompt" << std::endl <<
                      "Enter here: ";
            if (std::cin >> value) {
                switch (value) {
                    case 0:
                        return true;
                    case 1:
                        std::cout << std::endl << "Computation continued. Wait for result..." << std::endl;
                        return false;
                    case 2:
                        _is_prompt_disabled = true;
                        std::cout << std::endl << "Computation continued without prompt. Wait for result..." <<
                                     std::endl;
                        return false;
                    default:
                        std::cout << "Wrong input! Try again." << std::endl << std::endl;
                }
            } else {
                std::cin.clear();
                std::cin.ignore(100, '\n');
                std::cout << "Wrong input! Try again." << std::endl << std::endl;
            }
        }
    }

    void run() {
        time_point prev_time, cur_time;
        std::chrono::seconds time_passed;

        while (true) {
            if (!_is_prompt_disabled) prev_time = std::chrono::system_clock::now();
            time_passed = 0s;

            while (!_is_off && !(GetKeyState(VK_ESCAPE) & 0x8000)) {
                if (!_is_prompt_disabled) {
                    if (time_passed < period) {
                        cur_time = std::chrono::system_clock::now();
                        time_passed = std::chrono::duration_cast<std::chrono::seconds>(cur_time - prev_time);
                    } else break;
                }
            }

            if (_is_off) return;
            else if (!_is_prompt_disabled) {
                _is_prompt_on = true;
                if (show_prompt(time_passed)) {
                    std::cout << std::endl << "Computation stopped." << std::endl;
                    _is_prompt_on = false;
                    _is_cancelled = true;
                    return;
                }
                _is_prompt_on = false;
            } else {
                _is_cancelled = true;
                std::cout << std::endl << "Computation stopped." << std::endl;
                return;
            }
        }
    }

public:
    explicit periodic_cancellator(unsigned long long period)
        : period(period), _is_prompt_disabled(false), _is_prompt_on(false), _is_cancelled(false), _is_off(false) {}

    void start() override {
        auto th = std::thread(&periodic_cancellator::run, this);
        th.detach();
    }

    bool has_prompt() override { return !_is_prompt_disabled; }

    bool is_prompt_on() override { return _is_prompt_on; }

    bool is_cancelled() override { return _is_cancelled; }

    void finish() override { _is_off = true; }
};

#endif //SPOS_LAB_1_PERIODIC_PROMPT_H
