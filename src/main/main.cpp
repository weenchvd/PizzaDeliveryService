
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"graphicUI.hpp"
#include"map.hpp"
#include<chrono>
#include<d3d11.h>
#include<iostream>
#include<memory>
#include<thread>


int main(int argc, char* argv[])
{
    constexpr int fps{ 30 };
    constexpr int nanosecPerFrame{ 1'000'000'000 / fps };

    using namespace std;

    try {
        WNDCLASSEXW wc;
        HWND hWnd;
        if (!initializeGUI(wc, hWnd)) return -1;

        bool showGuiMenuMain{ true };
        ds::Map map;
        const chrono::nanoseconds npf{ nanosecPerFrame };
        auto prevTime{ chrono::steady_clock::now() };
        chrono::nanoseconds accumulatedLag{ 0 };
        while (showGuiMenuMain) {
            auto curTime{ chrono::steady_clock::now() };
            auto elapsedTime{ curTime - prevTime };
            prevTime = curTime;
            accumulatedLag += npf - elapsedTime;
            if (accumulatedLag > chrono::nanoseconds{ 0 }) {
                this_thread::sleep_for(accumulatedLag);
                accumulatedLag = chrono::nanoseconds{ 0 };
                //cout << "+ ";
            }
            else {
                //cout << "- ";
            }
            //cout << (double)elapsedTime.count() / 1'000'000 << " ms" << endl;


            renderGUI(&showGuiMenuMain, map);
        }

        shutdownGUI(wc, hWnd);
    }
    catch (const std::exception& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
    catch (...) {
        cerr << u8"[Unknown exception]" << endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
