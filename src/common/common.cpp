
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include<ctime>
#include<iomanip>
#include<sstream>

namespace cmn {

using namespace std;

int getRandomNumber(int from, int to)
{
    static random_device rd;
    static default_random_engine re{ rd() };
    uniform_int_distribution<int> unifDist{ from, to };
    return unifDist(re);
}

string getDuration(chrono::system_clock::duration duration)
{
    const auto hours  { chrono::duration_cast<chrono::hours>(duration) };
    const auto minutes{ chrono::duration_cast<chrono::minutes>(duration) };
    const auto seconds{ chrono::duration_cast<chrono::seconds>(duration) };
    ostringstream oss;
    oss << hours.count() << ':'
        << setw(2) << setfill('0') << minutes.count() % 60 << ':'
        << setw(2) << setfill('0') << seconds.count() % 60;
    return oss.str();
}

string getTime(chrono::system_clock::time_point timePoint)
{
    const time_t t{ chrono::system_clock::to_time_t(timePoint) };
    ostringstream oss;
    oss << ctime(&t);
    return oss.str();
}

} // namespace cmn
