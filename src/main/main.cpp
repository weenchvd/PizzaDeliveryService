
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"graphicUI.hpp"
#include<iostream>
#include<memory>


int main(int argc, char* argv[])
{
    using namespace std;

    try {
        graphicUI();
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
