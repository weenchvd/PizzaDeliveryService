
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef COMMON_HPP
#define COMMON_HPP

#include<assert.h>
#include<chrono>
#include<random>
#include<string>
#include<type_traits>
#include<utility>
#include<vector>

namespace cmn {

template <class Enum>
constexpr auto toUnderlying(Enum e) noexcept
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}

template <class Enum>
constexpr void verifyEnum() noexcept
{
    assert(toUnderlying(Enum::__INVALID) == -1);
    assert(Enum::__INVALID < Enum::__NUMBER_OF);
    assert(Enum::__NUMBER_OF < Enum::__END);
}

template <class Enum>
constexpr bool isValidEnum(Enum e) noexcept
{
    verifyEnum<Enum>();
    return (e > Enum::__INVALID && e < Enum::__NUMBER_OF);
}

template <class Enum>
constexpr bool isValidEnumAux(Enum e) noexcept
{
    verifyEnum<Enum>();
    return (e > Enum::__NUMBER_OF && e < Enum::__END);
}

template <class Enum>
constexpr auto numberOf() noexcept
{
    verifyEnum<Enum>();
    return toUnderlying(Enum::__NUMBER_OF);
}

template <class Enum>
constexpr auto firstEnum() noexcept
{
    auto e{ static_cast<Enum>(toUnderlying(Enum::__INVALID) + 1) };
    assert(isValidEnum(e));
    return e;
}

template <class Enum>
constexpr auto lastEnum() noexcept
{
    auto e{ static_cast<Enum>(toUnderlying(Enum::__NUMBER_OF) - 1) };
    assert(isValidEnum(e));
    return e;
}

///************************************************************************************************

int getRandomNumber(int from, int to);

std::string getDuration(std::chrono::system_clock::duration duration);

std::string getTime(std::chrono::system_clock::time_point timePoint);

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
