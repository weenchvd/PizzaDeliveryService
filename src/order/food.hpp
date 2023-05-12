
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef FOOD_HPP
#define FOOD_HPP

#include<chrono>
#include<string>
#include<utility>

namespace ds {

enum class FoodName : char {
    __INVALID = -1,                 /// invalid, must be the first
    // vvv NAMES vvv
    /// PIZZA
    PEPPERONI,
    PEPPERONI_FRESH,
    DOUBLE_PEPPERONI,
    CHICKEN_BBQ,
    HAWAIIAN,
    FOUR_CHEESE,
    VEGAN_HOT,
    /// SIDES
    CLASSIC_WINGS,
    BBQ_WINGS,
    TERIYAKI_WINGS,
    VEGAN_NUGGETS,
    CHICKEN_STRIPS,
    /// DRINKS
    COLA,
    CITRUS_JUICE,
    WATER,
    // ^^^ NAMES ^^^
    __NUMBER_OF,
    __END                           /// must be the last
};

enum class FoodType : char {
    __INVALID = -1,                 /// invalid, must be the first
    // vvv TYPES vvv
    PIZZA,
    SIDES,
    DRINKS,
    // ^^^ TYPES ^^^
    __NUMBER_OF,
    __END                           /// must be the last
};

enum class FoodStatus : char {
    __INVALID = -1,                 /// invalid, must be the first
    // vvv STATUSES vvv
    WAITING_FOR_MAKING,
    MAKING,
    DONE,
    // ^^^ STATUSES ^^^
    __NUMBER_OF,
    __END                           /// must be the last
};

std::string toString(FoodName value);

std::string toString(FoodStatus value);

///************************************************************************************************

class Food {
public:
    static constexpr std::chrono::seconds doughTime_{ 60 * 2 + 30 };

public:
    Food(unsigned short quantity, const FoodName name) noexcept
        : qty_{ quantity }, name_{ name }, status_{ FoodStatus::__INVALID } {}

    Food(const Food&) = delete;
    Food& operator=(const Food&) = delete;
    Food(Food&&) = default;
    Food& operator=(Food&&) = default;

    virtual ~Food() noexcept {}

public:
    unsigned short getQuantity() const noexcept { return qty_; }

    void setQuantity(unsigned short quantity) noexcept { qty_ = quantity; }

    FoodName getName() const noexcept { return name_; }

    FoodType getType() const noexcept;

    FoodStatus getStatus() const noexcept { return status_; }

    void setStatus(FoodStatus status) noexcept { status_ = status; }

    std::chrono::seconds getPreparationTime() const noexcept;

    std::chrono::seconds getCookingTime() const noexcept;

    std::chrono::seconds getTotalTime() const noexcept;

private:
    unsigned short                              qty_;
    const FoodName                              name_;
    FoodStatus                                  status_;
};

inline std::chrono::seconds Food::getTotalTime() const noexcept
{
    return getPreparationTime() + getCookingTime();
}

} // namespace ds

#endif // !FOOD_HPP
