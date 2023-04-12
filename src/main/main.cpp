
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include"courier.hpp"
#include"delivery.hpp"
#include"graphicUI.hpp"
#include"map.hpp"
#include"msystem.hpp"
#include"options.hpp"
#include"scheduler.hpp"
#include<boost/circular_buffer.hpp>
#include<chrono>
#include<d3d11.h>
#include<iomanip>
#include<iostream>
#include<memory>
#include<numeric>
#include<thread>
#include<vector>

bool createOrder(std::chrono::nanoseconds elapsedTime)
{
    using namespace std::chrono;
    constexpr unsigned int chance{ 5 };
    constexpr unsigned int step{ 2 };
    constexpr unsigned int stepBig{ step * 5 };
    constexpr unsigned int from{ 1 };
    static unsigned int to{ 50 };
    constexpr unsigned int toMin{ 10 };
    constexpr unsigned int toMax{ 100 };
    static nanoseconds time{ 0 };
    time += elapsedTime;
    if (time >= seconds{ 60 }) {
        time = nanoseconds{ 0 };
        const auto random{ cmn::getRandomNumber(from, to) };
        if (random >= from && random <= chance) {
            if (to <= toMax - stepBig) {
                to += stepBig;
            }
            return true;
        }
        else {
            if (to >= toMin + step) {
                to -= step;
            }
        }
    }
    return false;
}

int main(int argc, char* argv[])
{
    using namespace std;

    try {
        WNDCLASSEXW wc;
        HWND hWnd;
        if (!initializeGUI(wc, hWnd)) return -1;

        bool showGuiMenuMain{ true };
        ds::Graph::vertex_descriptor office{ 0 };
        ds::Map map{};
        ds::Scheduler scheduler{ office };
        ds::Kitchen kitchen{};
        ds::Delivery delivery{};
        ds::ManagmentSystem ms{ map, scheduler, kitchen, delivery };
        scheduler.setManagmentSystem(&ms);
        kitchen.setManagmentSystem(&ms);
        delivery.setManagmentSystem(&ms);

        ms.activateKitchener(ds::WorkerID{ 11 }, ds::KitchenerType::DOUGH);
        ms.activateKitchener(ds::WorkerID{ 12 }, ds::KitchenerType::FILLING);
        ms.activateKitchener(ds::WorkerID{ 13 }, ds::KitchenerType::FILLING);
        ms.activateKitchener(ds::WorkerID{ 14 }, ds::KitchenerType::PICKER);
        ms.activateKitchener(ds::WorkerID{ 15 }, ds::KitchenerType::PICKER);
        ms.activateKitchener(ds::WorkerID{ 16 }, ds::KitchenerType::PICKER);
        ms.activateCourier(ds::WorkerID{ 0 });
        ms.activateCourier(ds::WorkerID{ 1 });
        ms.activateCourier(ds::WorkerID{ 2 });
        ms.createOrder();
        ms.createOrder();


        const unsigned int sleepForTimeCapacity{ 20 };
        boost::circular_buffer<chrono::nanoseconds> sleepForTime{
            sleepForTimeCapacity, chrono::nanoseconds{ nano::den / ds::Options::defFPS_ } };

        chrono::nanoseconds npf{ 0 }; // nanoseconds per frame
        auto prevFS{ chrono::steady_clock::now() }; // previous frameStart
        ms.setCurrentTime();
        while (showGuiMenuMain) {
            const auto frameStart{ chrono::steady_clock::now() };
            auto elapsedTime{ frameStart - prevFS };
            prevFS = frameStart;

            if (createOrder(elapsedTime * ds::Options::instance().timeSpeed_)) {
                ms.createOrder();
            }

            ms.update(elapsedTime * ds::Options::instance().timeSpeed_);
            renderGUI(&showGuiMenuMain, ms);

            npf = chrono::nanoseconds{ nano::den / ds::Options::instance().fps_ };
            const chrono::nanoseconds avgSleepForTime{
                accumulate(sleepForTime.cbegin(), sleepForTime.cend(), chrono::nanoseconds{ 0 })
                / sleepForTime.size()
            };
            npf -= avgSleepForTime;

            const auto frameEnd{ chrono::steady_clock::now() };
            const auto sleepTime{ npf - (frameEnd - frameStart) };
            const auto t1{ chrono::steady_clock::now() };
            this_thread::sleep_for(sleepTime);
            const auto t2{ chrono::steady_clock::now() };
            sleepForTime.push_back(t2 - t1 - sleepTime);
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
