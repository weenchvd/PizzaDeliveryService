
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef ORDER_HPP
#define ORDER_HPP

#include"food.hpp"
#include"map.hpp"
#include<chrono>
#include<memory>
#include<string>
#include<type_traits>
#include<vector>

namespace ds {

enum class OrderID : unsigned long long int {};

enum class OrderStatus : char {
    __INVALID = -1,                 /// invalid, must be the first
    // vvv STATUSES vvv
    CANCELLED,
    ACCEPTED,
    WAITING_FOR_COOKING,
    COOKING,
    COOKING_COMPLETED,
    WAITING_FOR_DELIVERY,
    DELIVERING,
    PAYING,
    DELIVERING_COMPLETED,
    COMPLETED,
    // ^^^ STATUSES ^^^
    __NUMBER_OF,
    __END                           /// must be the last
};

std::string toString(OrderStatus value);

///************************************************************************************************

class Order {
public:
    using time_point_t = std::chrono::system_clock::time_point;

public:
    Order(const OrderID orderID, const size_t target, const time_point_t timeStart);

    Order(const Order&) = delete;
    Order& operator=(const Order&) = delete;
    Order(Order&&) = default;
    Order& operator=(Order&&) = default;

    virtual ~Order() noexcept {}

public:
    OrderID getID() const noexcept { return orderID_; }

    size_t getTarget() const noexcept { return target_; }

    time_point_t getTimeStart() const noexcept { return timeStart_; }

    time_point_t getTimeEnd() const noexcept { return timeEnd_; }

    void setTimeEnd(time_point_t timeEnd) { timeEnd_ = timeEnd; }

    const std::vector<Food>& getFood() const noexcept { return food_; }

    std::vector<Food>& getFood() noexcept { return food_; }

    void setFood(std::vector<Food> food);

    OrderStatus getStatus() const noexcept { return status_; }

    void setStatus(OrderStatus status) noexcept { status_ = status; }

    bool isPaid() const noexcept { return isPaid_; }

    void isPaid(bool value) noexcept { isPaid_ = value; }

private:
    const OrderID                               orderID_;
    const size_t                                target_;
    const time_point_t                          timeStart_;
    time_point_t                                timeEnd_;
    std::vector<Food>                           food_;
    OrderStatus                                 status_;
    bool                                        isPaid_;
};

///************************************************************************************************

class ManagmentSystem;

class Route {
public:
    Route(ManagmentSystem& ms, std::vector<Order*> orders,
        std::vector<Graph::edge_descriptor> path);

    Route(const Route&) = delete;
    Route& operator=(const Route&) = delete;
    Route(Route&&) = default;
    Route& operator=(Route&&) = default;

    virtual ~Route() noexcept {}

public:
    const std::vector<Order*>& getOrders() const noexcept { return orders_; }

    std::vector<Order*>& getOrders() noexcept { return orders_; }

    const std::vector<Graph::edge_descriptor>& getPath() const { return path_; }

private:
    ManagmentSystem&                            ms_;
    std::vector<Order*>                         orders_;
    std::vector<Graph::edge_descriptor>         path_;
};

} // namespace ds

#endif // !ORDER_HPP
