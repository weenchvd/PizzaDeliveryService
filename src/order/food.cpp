
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"food.hpp"

namespace ds {

using namespace std;

string toString(FoodType value)
{
    switch (value) {
    case FoodType::PEPPERONI:
        return u8"Pepperoni";
    case FoodType::PEPPERONI_FRESH:
        return u8"Pepperoni Fresh";
    case FoodType::DOUBLE_PEPPERONI:
        return u8"Double Pepperoni";
    case FoodType::CHICKEN_BBQ:
        return u8"Chicken BBQ";
    case FoodType::HAWAIIAN:
        return u8"Hawaiian";
    case FoodType::FOUR_CHEESE:
        return u8"4 Cheese";
    case FoodType::VEGAN_HOT:
        return u8"Vegan Hot";
    case FoodType::CLASSIC_WINGS:
        return u8"Classic Wings";
    case FoodType::BBQ_WINGS:
        return u8"BBQ Wings";
    case FoodType::TERIYAKI_WINGS:
        return u8"Teriyaki Wings";
    case FoodType::VEGAN_NUGGETS:
        return u8"Vegan Nuggets";
    case FoodType::CHICKEN_STRIPS:
        return u8"Chicken Strips";
    case FoodType::COLA:
        return u8"Cola";
    case FoodType::CITRUS_JUICE:
        return u8"Citrus Juice";
    case FoodType::WATER:
        return u8"Water";
    default:
        return u8"UNKNOWN";
    }
}

///************************************************************************************************

chrono::seconds Food::getPreparationTime() const noexcept
{
    switch (type_) {
    case FoodType::PEPPERONI:
        return chrono::seconds{ 60 * 3 + 30 };
    case FoodType::PEPPERONI_FRESH:
        return chrono::seconds{ 60 * 4 };
    case FoodType::DOUBLE_PEPPERONI:
        return chrono::seconds{ 60 * 3 + 50 };
    case FoodType::CHICKEN_BBQ:
        return chrono::seconds{ 60 * 4 };
    case FoodType::HAWAIIAN:
        return chrono::seconds{ 60 * 5 + 20 };
    case FoodType::FOUR_CHEESE:
        return chrono::seconds{ 60 * 3 };
    case FoodType::VEGAN_HOT:
        return chrono::seconds{ 60 * 4 + 40 };
    case FoodType::CLASSIC_WINGS:
        return chrono::seconds{ 50 };
    case FoodType::BBQ_WINGS:
        return chrono::seconds{ 60 };
    case FoodType::TERIYAKI_WINGS:
        return chrono::seconds{ 60 + 30 };
    case FoodType::VEGAN_NUGGETS:
        return chrono::seconds{ 50 };
    case FoodType::CHICKEN_STRIPS:
        return chrono::seconds{ 60 };
    case FoodType::COLA:
        return chrono::seconds{ 30 };
    case FoodType::CITRUS_JUICE:
        return chrono::seconds{ 30 };
    case FoodType::WATER:
        return chrono::seconds{ 30 };
    default:
        return chrono::seconds{ 0 };
    }
}

chrono::seconds Food::getCookingTime() const noexcept
{
    switch (type_) {
    case FoodType::PEPPERONI:
        return chrono::seconds{ 60 * 6 };
    case FoodType::PEPPERONI_FRESH:
        return chrono::seconds{ 60 * 6 };
    case FoodType::DOUBLE_PEPPERONI:
        return chrono::seconds{ 60 * 6 };
    case FoodType::CHICKEN_BBQ:
        return chrono::seconds{ 60 * 6 };
    case FoodType::HAWAIIAN:
        return chrono::seconds{ 60 * 6 };
    case FoodType::FOUR_CHEESE:
        return chrono::seconds{ 60 * 6 };
    case FoodType::VEGAN_HOT:
        return chrono::seconds{ 60 * 6 };
    case FoodType::CLASSIC_WINGS:
        return chrono::seconds{ 60 * 8 };
    case FoodType::BBQ_WINGS:
        return chrono::seconds{ 60 * 8 };
    case FoodType::TERIYAKI_WINGS:
        return chrono::seconds{ 60 * 8 };
    case FoodType::VEGAN_NUGGETS:
        return chrono::seconds{ 60 * 2 + 40 };
    case FoodType::CHICKEN_STRIPS:
        return chrono::seconds{ 60 * 5 + 10 };
    case FoodType::COLA:
        return chrono::seconds{ 0 };
    case FoodType::CITRUS_JUICE:
        return chrono::seconds{ 0 };
    case FoodType::WATER:
        return chrono::seconds{ 0 };
    default:
        return chrono::seconds{ 0 };
    }
}

} // namespace ds
