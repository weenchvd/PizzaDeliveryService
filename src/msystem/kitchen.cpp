
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include"kitchen.hpp"
#include"msystem.hpp"
#include<assert.h>

namespace ds {

using namespace std;

Kitchen::Kitchen()
    :
    ms_             { nullptr },
    ordersCooking_  {},
    remainingTime_  {}
{}

void Kitchen::update(chrono::nanoseconds passedTime)
{
    assert(ms_ != nullptr);
    assert(ordersCooking_.size() == remainingTime_.size());
    for (int i = 0; i < ordersCooking_.size();) {
        ordersCooking_[i]->addPassedTime(passedTime);
        remainingTime_[i] -= passedTime;
        if (remainingTime_[i].count() <= 0) {
            cmn::extractWithShift(remainingTime_, i);
            Order* order{ cmn::extractWithShift(ordersCooking_, i) };
            assert(ordersCooking_.size() == remainingTime_.size());
            assert(order != nullptr);
            order->setStatus(OrderStatus::COOKING_COMPLETED);
            ms_->processOrder(order);
        }
        else {
            ++i;
        }
    }
}

void Kitchen::makeOrder(Order* order)
{
    assert(order != nullptr);
    order->setStatus(OrderStatus::COOKING);
    ordersCooking_.push_back(order);
    remainingTime_.push_back(chrono::nanoseconds{ cmn::getRandomNumber(60 * 4, 60 * 10) * nano::den });
}

} // namespace ds
