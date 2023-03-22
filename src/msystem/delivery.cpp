
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include"delivery.hpp"
#include"msystem.hpp"
#include"options.hpp"
#include<assert.h>

namespace ds {

using namespace std;

Delivery::Delivery()
    :
    ms_             { nullptr },
    ordDeliv_       {},
    couriers_       {}
{}

void Delivery::update(chrono::nanoseconds passedTime)
{
    assert(ms_ != nullptr);
    for (int i = 0; i < couriers_.size(); ++i) {
        couriers_[i]->update(passedTime);
    }
    const chrono::nanoseconds checkTime{ Options::instance().checkTimeFreeCour_ * nano::den };
    static chrono::nanoseconds time{ 0 };
    time += passedTime;
    if (time >= checkTime) {
        time -= checkTime;
        deleteCompletedOrders();
        deliverWaitingOrders();
    }
}

void Delivery::deliveryOrder(Order* order)
{
    assert(order != nullptr);
    order->setStatus(OrderStatus::WAITING_FOR_DELIVERY);
    ordDeliv_.push_back(order);
}

void Delivery::addCourier(Courier* courier)
{
    assert(courier != nullptr);
    couriers_.push_back(courier);
}

void Delivery::deleteCourier(Courier* courier)
{
    assert(courier != nullptr);
    for (int i = 0; i < couriers_.size(); ++i) {
        if (couriers_[i] == courier) {
            cmn::extractWithShift(couriers_, i);
            break;
        }
    }
}

Courier* Delivery::getFreeCourier() const
{
    Courier* c{ nullptr };
    for (int i = 0; i < couriers_.size(); ++i) {
        assert(couriers_[i] != nullptr);
        if (couriers_[i]->getStatus() == CourierStatus::WAITING_FOR_NEXT) {
            if (c != nullptr) {
                if (c->getWaitingTime() < couriers_[i]->getWaitingTime()) {
                    c = couriers_[i];
                }
            }
            else {
                c = couriers_[i];
            }
        }
    }
    return c;
}

vector<Order*> Delivery::getWaitingOrders() const
{
    vector<Order*> wo{};
    for (Order* o : ordDeliv_) {
        assert(o != nullptr);
        if (o->getStatus() == OrderStatus::WAITING_FOR_DELIVERY) {
            wo.push_back(o);
        }
    }
    return wo;
}

void Delivery::deleteCompletedOrders()
{
    for (int i = 0; i < ordDeliv_.size(); ++i) {
        Order* o{ ordDeliv_[i] };
        assert(o != nullptr);
        if (o->getStatus() == OrderStatus::PAYMENT_COMPLETED) {
            cmn::extractWithShift(ordDeliv_, i);
            ms_->processOrder(o);
        }
    }
}

void Delivery::deliverWaitingOrders()
{
    vector<Order*> wo{ getWaitingOrders() };
    Courier* c{ nullptr };
    while (wo.size() > 0 && (c = getFreeCourier()) != nullptr) {
        assert(c->getStatus() == CourierStatus::WAITING_FOR_NEXT);
        Order* o{ cmn::extractWithShift(wo, 0) };
        assert(o != nullptr);
        c->setRouteList(o, ms_->map().getPath(0, o->getTarget()));
    }
}

} // namespace ds
