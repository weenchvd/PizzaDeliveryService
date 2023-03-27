
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include"order.hpp"
#include<utility>

namespace ds {

using namespace std;

string toString(OrderStatus value)
{
    switch (value) {
    case OrderStatus::INVALID:
        return u8"INVALID STATUS";
    case OrderStatus::ACCEPTED:
        return u8"Accepted";
    case OrderStatus::WAITING_FOR_COOKING:
        return u8"Waiting for cooking";
    case OrderStatus::COOKING:
        return u8"Cooking";
    case OrderStatus::COOKING_COMPLETED:
        return u8"Cooking completed";
    case OrderStatus::WAITING_FOR_DELIVERY:
        return u8"Waiting for delivery";
    case OrderStatus::DELIVERING:
        return u8"Delivering";
    case OrderStatus::DELIVERING_COMPLETED:
        return u8"Delivering completed";
    case OrderStatus::WAITING_FOR_PAYMENT:
        return u8"Waiting for payment";
    case OrderStatus::PAYMENT:
        return u8"Payment";
    case OrderStatus::PAYMENT_COMPLETED:
        return u8"Payment completed";
    case OrderStatus::COMPLETED:
        return u8"Completed";
    case OrderStatus::CANCELLED:
        return u8"Cancelled";
    default:
        return u8"UNKNOWN";
    }
}

///************************************************************************************************

Order::Order(const OrderID orderID, const size_t target, const time_point_t timeStart)
    :
    orderID_        { orderID },
    target_         { target },
    timeStart_      { timeStart },
    timeEnd_        { timeStart },
    food_           {},
    status_         { OrderStatus::INVALID }
{}

void Order::setFood(std::vector<Food> food)
{
    food_ = std::move(food);
    for (auto& f : food_) {
        f.setStatus(FoodStatus::WAITING_FOR_MAKING);
    }
}

RouteList::RouteList(const ManagmentSystem& ms, Order* order,
    std::vector<Graph::edge_descriptor> path)
    :
    ms_             { ms },
    order_          { order },
    path_           { std::move(path) },
    requiredTime_   { 0 },
    curEdge_        { 0 },
    passedDist_     { 0 },
    fullDist_       { 0 }
{}

} // namespace ds
