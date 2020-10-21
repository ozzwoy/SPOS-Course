//
// Created by Citrus on 22.10.2020.
//

#ifndef SPOS_LAB_1_PERIODIC_PROMPT_H
#define SPOS_LAB_1_PERIODIC_PROMPT_H

#include "cancellator.h"
#include <chrono>
#include <iostream>
#include <process.h>
#include <string>
#include <thread>
#include <WinUser.h>
#include <xutility>

class periodic_cancellator : public cancellator {
private:
    typedef std::chrono::time_point<std::chrono::system_clock> time_point;
    const std::chrono::seconds period;
    bool _is_prompt_disabled;
    bool _is_prompt_on;
    bool _is_cancelled;
    bool _is_off;
    std::string message;

    bool show_prompt() {
        int value;

        system("cls");
        while (true) {
            std::cout << "Please, choose one of the following options:" << std::endl <<
                      "- enter 0 to stop computation" << std::endl <<
                      "- enter 1 to continue computation" << std::endl <<
                      "- enter 2 to continue computation without prompt" << std::endl <<
                      "Enter here: ";
            if (std::cin >> value) {
                system("cls");
                switch (value) {
                    case 0:
                        std::cout << message;
                        return true;
                    case 1:
                        std::cout << message;
                        return false;
                    case 2:
                        _is_prompt_disabled = true;
                        std::cout << message;
                        return false;
                    default:
                        std::cout << "Wrong input! Try again." << std::endl << std::endl;
                }
            } else {
                system("cls");
                std::cout << "Wrong input! Try again." << std::endl << std::endl;
            }
        }
    }

    void run() {
        time_point prev_time, cur_time;
        std::chrono::seconds time_passed{0};

        while (true) {
            if (!_is_prompt_disabled) prev_time = std::chrono::system_clock::now();
            while (!_is_off && !(GetKeyState(VK_ESCAPE) & 0x8000)) {
                if (!_is_prompt_disabled) {
                    if (time_passed < period) {
                        cur_time = std::chrono::system_clock::now();
                        time_passed = std::chrono::duration_cast<std::chrono::seconds>(cur_time - prev_time);
                    } else break;
                }
            }

            if (!_is_off || (!_is_prompt_disabled && show_prompt())) {
                _is_cancelled = true;
                return;
            }
        }
    }

public:
    periodic_cancellator(std::chrono::seconds period, std::string post_prompt_message)
        : period(period), message(std::move(post_prompt_message)), _is_prompt_disabled(false), _is_prompt_on(false),
        _is_cancelled(false), _is_off(false) {}

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
