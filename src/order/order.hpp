
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef ORDER_HPP
#define ORDER_HPP

#include"map.hpp"
#include<chrono>
#include<memory>
#include<string>
#include<vector>

namespace ds {

enum class OrderID : unsigned long long int {};

enum class OrderStatus : char
{
    INVALID = -1,

    ACCEPTED,
    WAITING_FOR_COOKING,
    COOKING,
    COOKING_COMPLETED,
    WAITING_FOR_DELIVERY,
    DELIVERING,
    DELIVERING_COMPLETED,
    WAITING_FOR_PAYMENT,
    PAYMENT,
    PAYMENT_COMPLETED,
    COMPLETED,
    CANCELLED
};

std::string toString(OrderStatus value);

///************************************************************************************************

class Order {
public:
    using time_point_t = std::chrono::system_clock::time_point;

public:
    Order(const OrderID orderID, const size_t target);

    Order(const Order&) = delete;
    Order& operator=(const Order&) = delete;
    Order(Order&&) = default;
    Order& operator=(Order&&) = default;

    virtual ~Order() noexcept {}

public:
    OrderID getID() const noexcept { return orderID_; }

    size_t getTarget() const noexcept { return target_; }

    void addPassedTime(std::chrono::nanoseconds passedTime) { passedTime_ += passedTime; }

    void setTimeEnd() { timeEnd_ = std::chrono::system_clock::now(); }

    OrderStatus getStatus() const noexcept { return status_; }

    void setStatus(OrderStatus status) noexcept { status_ = status; }

private:
    const OrderID                               orderID_;
    const size_t                                target_;
    const time_point_t                          timeStart_;
    time_point_t                                timeEnd_;
    std::chrono::nanoseconds                    passedTime_;    // time has passed since the order was received
    OrderStatus                                 status_;
};

///************************************************************************************************

class ManagmentSystem;

class RouteList {
public:
    friend class CourierState;
    friend class CourierWaiting;
    friend class CourierAccepting;
    friend class CourierMovement;
    friend class CourierPayment;
    friend class CourierReturning;

public:
    RouteList(const ManagmentSystem& ms, Order* order,
        std::vector<Graph::edge_descriptor> path);

    RouteList(const RouteList&) = delete;
    RouteList& operator=(const RouteList&) = delete;
    RouteList(RouteList&&) = default;
    RouteList& operator=(RouteList&&) = default;

    virtual ~RouteList() noexcept {}

private:
    const ManagmentSystem&                      ms_;
    Order*                                      order_;
    std::vector<Graph::edge_descriptor>         path_;
    std::chrono::nanoseconds                    requiredTime_;
    long long int                               curEdge_;       // current traversable edge on the path
    long long int                               passedDist_;    // passed distance for current edge, nanometers
    long long int                               fullDist_;      // full distance of current edge, nanometers
};

} // namespace ds

#endif // !ORDER_HPP
