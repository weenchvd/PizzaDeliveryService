
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"

namespace cmn {

using namespace std;

int getRandomNumber(int from, int to)
{
    static random_device rd;
    static default_random_engine re{ rd() };
    uniform_int_distribution<int> unifDist{ from, to };
    return unifDist(re);
}

} // namespace cmn
