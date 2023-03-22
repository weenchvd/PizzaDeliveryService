
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef COMMON_HPP
#define COMMON_HPP

#include<assert.h>
#include<random>
#include<type_traits>
#include<utility>
#include<vector>

namespace cmn {

template <class Enum>
constexpr auto toUnderlying(Enum e) noexcept
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}

///************************************************************************************************

int getRandomNumber(int from, int to);

template <class T>
T extractWithShift(std::vector<T>& v, size_t index)
{
    assert(v.size() > 0 && index < v.size());
    for (size_t i{ index }; i < v.size() - 1; ++i) {
        std::swap(v[i], v[i + 1]);
    }
    T ret{ std::move(v.back()) };
    v.pop_back();
    return std::move(ret);
}

} // namespace cmn

#endif // !COMMON_HPP
