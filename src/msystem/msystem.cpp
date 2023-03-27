
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include"msystem.hpp"
#include<assert.h>
#include<utility>

namespace ds {

using namespace std;

ManagmentSystem::ManagmentSystem(Map& map, Scheduler& scheduler, Kitchen& kitchen, Delivery& delivery)
    :
    map_                { map },
    scheduler_          { scheduler },
    kitchen_            { kitchen },
    delivery_           { delivery },
    orders_             {},
    couriers_           {},
    startTime_          { chrono::system_clock::now() },
    passedTime_         { 0 },
    nextOrderID_        { 0 }
{}

void ManagmentSystem::update(chrono::nanoseconds passedTime)
{
    passedTime_ += passedTime;
    kitchen_.update(passedTime);
    delivery_.update(passedTime);
}

Order* ManagmentSystem::createOrder()
{
    int randomTarget{ cmn::getRandomNumber(1, map_.graph().m_vertices.size() - 1) };
    assert(randomTarget >= 0);
    unique_ptr<Order> order{ new Order{ nextOrderID_, size_t(randomTarget), getCurrentTime() } };
    assert(order.get() != nullptr);
    nextOrderID_ = OrderID{ cmn::toUnderlying(nextOrderID_) + 1 };
    order->setStatus(OrderStatus::ACCEPTED);
    order->setFood(createRandomFood());
    orders_.push_back(std::move(order));
    Order* o{ orders_.back().get() };
    scheduler_.processOrder(o);
    return o;
}

ManagmentSystem::time_point_t ManagmentSystem::getCurrentTime() const noexcept
{
    return startTime_ + chrono::duration_cast<time_point_t::duration>(passedTime_);
}

Courier* ManagmentSystem::activateCourier(CourierID courierID)
{
    for (int i = 0; i < couriers_.size(); ++i) {
        if (couriers_[i]->getID() == courierID) {
            return nullptr;
        }
    }
    unique_ptr<Courier> newCourier{ new Courier{ *this, courierID } };
    couriers_.push_back(std::move(newCourier));
    Courier* c{ couriers_.back().get() };
    delivery_.addCourier(c);
    return c;
}

bool ManagmentSystem::deactivateCourier(CourierID courierID)
{
    for (int i = 0; i < couriers_.size(); ++i) {
        if (couriers_[i]->getID() == courierID) {
            swap(couriers_[i], couriers_.back());
            delivery_.deleteCourier(couriers_.back().get());
            couriers_.pop_back();
            return true;
        }
    }
    return false;
}

vector<Food> ManagmentSystem::createRandomFood() const
{
    vector<Food> food;
    int n{ cmn::getRandomNumber(1, 6) };
    for (int i = 0; i < n; ++i) {
        FoodType type{ char(cmn::getRandomNumber(
            cmn::toUnderlying(cmn::firstEnum<FoodType>()),
            cmn::toUnderlying(cmn::lastEnum<FoodType>())
        ))};
        bool isExist{ false };
        for (const auto& f : food) {
            if (f.getType() == type) {
                isExist = true;
                break;
            }
        }
        if (isExist == false) {
            food.push_back(Food{ unsigned short(cmn::getRandomNumber(1, 3)), type });
        }
    }
    return food;
}

} // namespace ds
