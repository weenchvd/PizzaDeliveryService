
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef DELIVERY_HPP
#define DELIVERY_HPP

#include"courier.hpp"
#include"order.hpp"
#include<chrono>
#include<utility>
#include<vector>

namespace ds {

class ManagmentSystem;

class Delivery {
public:
    Delivery();

    Delivery(const Delivery&) = delete;
    Delivery& operator=(const Delivery&) = delete;

    virtual ~Delivery() noexcept {}

public:
    void update(std::chrono::nanoseconds passedTime);

    void deliveryOrder(Order* order);

    void addCourier(Courier* courier);

    void deleteCourier(Courier* courier);

    void setManagmentSystem(ManagmentSystem* ms) { ms_ = ms; }

    auto getOrders() const { return std::pair{ orders_.cbegin(), orders_.cend() }; }

    //auto getOrders() { return std::pair{ orders_.begin(), orders_.end() }; }

private:
    //std::vector<Order*> getWaitingOrders() const;

    void distributeOrders();

    void processOrders();

private:
    ManagmentSystem*                            ms_;
    std::vector<Order*>                         orders_;        // current orders in delivery
    std::vector<Order*>                         queue_;         // order queue for couriers
    std::vector<Courier*>                       couriers_;      // working couriers
};

} // namespace ds

#endif // !DELIVERY_HPP
