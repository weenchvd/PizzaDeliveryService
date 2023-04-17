
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
    orders_         {},
    queue_          {},
    couriers_       {}
{}

void Delivery::update(chrono::nanoseconds passedTime)
{
    const chrono::seconds checkTime{ Options::instance().checkTimeFreeCour_ };
    static chrono::nanoseconds time{ 0 };
    time += passedTime;
    if (time >= checkTime) {
        time -= checkTime;
        this->distributeOrders();
    }
    for (auto& c : couriers_) {
        c->update(passedTime);
    }
    this->processOrders();
}

void Delivery::distributeOrders()
{
    vector<Courier*> delivering;
    for (auto iterCourier{ couriers_.begin() }; iterCourier != couriers_.end();) {
        bool advance{ true };
        Courier* courier{ *iterCourier };
        if (courier->getStatus() == CourierStatus::WAITING_FOR_NEXT) {
            for (auto iterOrder{ queue_.begin() }; iterOrder != queue_.end(); ++iterOrder) {
                assert(*iterOrder != nullptr);
                if ((*iterOrder)->getStatus() == OrderStatus::WAITING_FOR_DELIVERY) {
                    Order* order{ *iterOrder };
                    auto path{ ms_->map().getPath(0, order->getTarget()) };
                    unique_ptr<Route> route{ new Route{ *ms_, order, std::move(path) } };
                    courier->setRoute(route);
                    iterOrder = queue_.erase(iterOrder);
                    iterCourier = couriers_.erase(iterCourier);
                    delivering.push_back(courier);
                    advance = false;
                    break;
                }
            }
        }
        if (advance) {
            ++iterCourier;
        }
    }
    couriers_.insert(couriers_.end(), delivering.begin(), delivering.end());
}

void Delivery::processOrders()
{
    assert(ms_ != nullptr);
    for (auto iter{ orders_.begin() }; iter != orders_.end();) {
        Order* order{ *iter };
        assert(order != nullptr);
        if (order->getStatus() == OrderStatus::DELIVERING_COMPLETED) {
            iter = orders_.erase(iter);
            ms_->processOrder(order);
        }
        else {
            ++iter;
        }
    }
}

void Delivery::deliveryOrder(Order* order)
{
    assert(order != nullptr);
    order->setStatus(OrderStatus::WAITING_FOR_DELIVERY);
    orders_.push_back(order);
    queue_.push_back(order);
}

void Delivery::addCourier(Courier* courier)
{
    assert(courier != nullptr);
    couriers_.push_back(courier);
}

void Delivery::deleteCourier(Courier* courier)
{
    assert(courier != nullptr);
    for (auto iter{ couriers_.begin() }; iter != couriers_.end(); ++iter) {
        if (*iter == courier) {
            couriers_.erase(iter);
            break;
        }
    }
}

//vector<Order*> Delivery::getWaitingOrders() const
//{
//    vector<Order*> wo{};
//    for (Order* o : orders_) {
//        assert(o != nullptr);
//        if (o->getStatus() == OrderStatus::WAITING_FOR_DELIVERY) {
//            wo.push_back(o);
//        }
//    }
//    return wo;
//}

} // namespace ds
