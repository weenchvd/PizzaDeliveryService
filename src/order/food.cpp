
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include"food.hpp"

namespace ds {

using namespace std;

string toString(FoodName value)
{
    switch (value) {
    case FoodName::PEPPERONI:
        return u8"Pepperoni";
    case FoodName::PEPPERONI_FRESH:
        return u8"Pepperoni Fresh";
    case FoodName::DOUBLE_PEPPERONI:
        return u8"Double Pepperoni";
    case FoodName::CHICKEN_BBQ:
        return u8"Chicken BBQ";
    case FoodName::HAWAIIAN:
        return u8"Hawaiian";
    case FoodName::FOUR_CHEESE:
        return u8"4 Cheese";
    case FoodName::VEGAN_HOT:
        return u8"Vegan Hot";
    case FoodName::CLASSIC_WINGS:
        return u8"Classic Wings";
    case FoodName::BBQ_WINGS:
        return u8"BBQ Wings";
    case FoodName::TERIYAKI_WINGS:
        return u8"Teriyaki Wings";
    case FoodName::VEGAN_NUGGETS:
        return u8"Vegan Nuggets";
    case FoodName::CHICKEN_STRIPS:
        return u8"Chicken Strips";
    case FoodName::COLA:
        return u8"Cola";
    case FoodName::CITRUS_JUICE:
        return u8"Citrus Juice";
    case FoodName::WATER:
        return u8"Water";
    default:
        return u8"UNKNOWN";
    }
    static_assert(cmn::numberOf<FoodName>() == 15);
}

string toString(FoodStatus value)
{
    switch (value) {
    case FoodStatus::WAITING_FOR_MAKING:
        return u8"Waiting for making";
    case FoodStatus::MAKING:
        return u8"Making";
    case FoodStatus::DONE:
        return u8"Done";
    default:
        return u8"UNKNOWN";
    }
    static_assert(cmn::numberOf<FoodStatus>() == 3);
}

///************************************************************************************************

FoodType Food::getType() const noexcept
{
    switch (name_) {
    case FoodName::PEPPERONI:
    case FoodName::PEPPERONI_FRESH:
    case FoodName::DOUBLE_PEPPERONI:
    case FoodName::CHICKEN_BBQ:
    case FoodName::HAWAIIAN:
    case FoodName::FOUR_CHEESE:
    case FoodName::VEGAN_HOT:
        return FoodType::PIZZA;
    case FoodName::CLASSIC_WINGS:
    case FoodName::BBQ_WINGS:
    case FoodName::TERIYAKI_WINGS:
    case FoodName::VEGAN_NUGGETS:
    case FoodName::CHICKEN_STRIPS:
        return FoodType::SIDES;
    case FoodName::COLA:
    case FoodName::CITRUS_JUICE:
    case FoodName::WATER:
        return FoodType::DRINKS;
    default:
        return FoodType::__INVALID;
    }
}

chrono::seconds Food::getPreparationTime() const noexcept
{
    switch (name_) {
    case FoodName::PEPPERONI:
        return chrono::seconds{ 60 * 4 + 30 };
    case FoodName::PEPPERONI_FRESH:
        return chrono::seconds{ 60 * 5 };
    case FoodName::DOUBLE_PEPPERONI:
        return chrono::seconds{ 60 * 4 + 50 };
    case FoodName::CHICKEN_BBQ:
        return chrono::seconds{ 60 * 5 };
    case FoodName::HAWAIIAN:
        return chrono::seconds{ 60 * 6 + 20 };
    case FoodName::FOUR_CHEESE:
        return chrono::seconds{ 60 * 4 };
    case FoodName::VEGAN_HOT:
        return chrono::seconds{ 60 * 5 + 40 };
    case FoodName::CLASSIC_WINGS:
        return chrono::seconds{ 50 };
    case FoodName::BBQ_WINGS:
        return chrono::seconds{ 60 };
    case FoodName::TERIYAKI_WINGS:
        return chrono::seconds{ 60 + 30 };
    case FoodName::VEGAN_NUGGETS:
        return chrono::seconds{ 50 };
    case FoodName::CHICKEN_STRIPS:
        return chrono::seconds{ 60 };
    case FoodName::COLA:
        return chrono::seconds{ 0 };
    case FoodName::CITRUS_JUICE:
        return chrono::seconds{ 0 };
    case FoodName::WATER:
        return chrono::seconds{ 0 };
    default:
        return chrono::seconds{ 0 };
    }
}

chrono::seconds Food::getCookingTime() const noexcept
{
    switch (name_) {
    case FoodName::PEPPERONI:
        return chrono::seconds{ 60 * 5 };
    case FoodName::PEPPERONI_FRESH:
        return chrono::seconds{ 60 * 5 };
    case FoodName::DOUBLE_PEPPERONI:
        return chrono::seconds{ 60 * 5 };
    case FoodName::CHICKEN_BBQ:
        return chrono::seconds{ 60 * 5 };
    case FoodName::HAWAIIAN:
        return chrono::seconds{ 60 * 5 };
    case FoodName::FOUR_CHEESE:
        return chrono::seconds{ 60 * 5 };
    case FoodName::VEGAN_HOT:
        return chrono::seconds{ 60 * 5 };
    case FoodName::CLASSIC_WINGS:
        return chrono::seconds{ 60 * 6 };
    case FoodName::BBQ_WINGS:
        return chrono::seconds{ 60 * 6 };
    case FoodName::TERIYAKI_WINGS:
        return chrono::seconds{ 60 * 6 };
    case FoodName::VEGAN_NUGGETS:
        return chrono::seconds{ 60 * 2 + 40 };
    case FoodName::CHICKEN_STRIPS:
        return chrono::seconds{ 60 * 5 + 10 };
    case FoodName::COLA:
        return chrono::seconds{ 30 };
    case FoodName::CITRUS_JUICE:
        return chrono::seconds{ 30 };
    case FoodName::WATER:
        return chrono::seconds{ 30 };
    default:
        return chrono::seconds{ 0 };
    }
}

} // namespace ds
