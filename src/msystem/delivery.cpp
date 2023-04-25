
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include"delivery.hpp"
#include"msystem.hpp"
#include"options.hpp"
#include<algorithm>
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
    const chrono::seconds checkTime{ Options::instance().optDelivery_.checkTimeFreeCour_ };
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
        if (queue_.empty() == true) {
            break;
        }
        bool advance{ true };
        Courier* courier{ *iterCourier };
        if (courier->getStatus() == CourierStatus::WAITING_FOR_NEXT) {
            vector<Graph::vertex_descriptor> tgtVertices{
                vector<Graph::vertex_descriptor>(queue_.size(), Graph::vertex_descriptor{ 0 })
            };
            vector<chrono::seconds> remainingTime{
                vector<chrono::seconds>(queue_.size(), chrono::seconds{ 0 })
            };
            const auto currentTime{ ms_->getCurrentTime() };
            assert(queue_.size() == tgtVertices.size());
            assert(queue_.size() == remainingTime.size());
            for (int i = 0; i < queue_.size(); ++i) {
                Order* order{ queue_[i] };
                assert(order->getStatus() == OrderStatus::WAITING_FOR_DELIVERY);
                tgtVertices[i] = order->getTarget();
                remainingTime[i] = chrono::seconds{
                    Options::instance().optDelivery_.deliveryTime_ -
                    chrono::duration_cast<chrono::seconds>(
                        currentTime - order->getTimeStart()).count()
                };
            }
            MapPath mp{
                ms_->map().getPath(ms_->scheduler().getOffice(), tgtVertices, remainingTime)
            };
            vector<Order*> orders;
            for (auto i : mp.visited_) {
                orders.push_back(queue_[i]);
            }
            unique_ptr<Route> route{ new Route{ *ms_, std::move(orders), std::move(mp.path_) } };
            courier->setRoute(route);
            iterCourier = couriers_.erase(iterCourier);
            delivering.push_back(courier);
            auto newEnd{ queue_.end() };
            for (auto iterOrder{ courier->getRoute()->getOrders().cbegin() };
                iterOrder != courier->getRoute()->getOrders().cend(); ++iterOrder)
            {
                newEnd = remove(queue_.begin(), newEnd, *iterOrder);
            }
            queue_.erase(newEnd, queue_.end());
            advance = false;
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

} // namespace ds
