
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef MANAGMENT_SYSTEM_HPP
#define MANAGMENT_SYSTEM_HPP

#include"courier.hpp"
#include"delivery.hpp"
#include"kitchen.hpp"
#include"map.hpp"
#include"order.hpp"
#include"scheduler.hpp"
#include<chrono>
#include<memory>
#include<utility>
#include<vector>

namespace ds {

class ManagmentSystem {
public:
    using time_point_t = std::chrono::system_clock::time_point;

public:
    ManagmentSystem(Map& map, Scheduler& scheduler, Kitchen& kitchen, Delivery& delivery);

    ManagmentSystem(const ManagmentSystem&) = delete;
    ManagmentSystem& operator=(const ManagmentSystem&) = delete;

    virtual ~ManagmentSystem() noexcept {}

public:
    void update(std::chrono::nanoseconds passedTime);

    const Map& map() const noexcept { return map_; }

    Map& map() noexcept { return map_; }

    const Scheduler& scheduler() const noexcept { return scheduler_; }

    Scheduler& scheduler() noexcept { return scheduler_; }

    const Kitchen& kitchen() const noexcept { return kitchen_; }

    Kitchen& kitchen() noexcept { return kitchen_; }

    const Delivery& delivery() const noexcept { return delivery_; }

    Delivery& delivery() noexcept { return delivery_; }

public:
    Order* createOrder();

    void processOrder(Order* order) { scheduler_.processOrder(order); }

    time_point_t getCurrentTime() const noexcept;

    void setCurrentTime() noexcept;

    auto getOrders() const { return std::pair{ orders_.cbegin(), orders_.cend() }; }

    //auto getOrders() { return std::pair{ orders_.begin(), orders_.end() }; }

    auto getCouriers() const { return std::pair{ couriers_.cbegin(), couriers_.cend() }; }

    //auto getCouriers() { return std::pair{ couriers_.begin(), couriers_.end() }; }

    Courier* activateCourier(WorkerID workerID);

    bool deactivateCourier(WorkerID workerID);

    Kitchener* activateKitchener(WorkerID workerID, KitchenerType type);

    bool deactivateKitchener(WorkerID workerID);

private:
    Map&                                        map_;
    Scheduler&                                  scheduler_;
    Kitchen&                                    kitchen_;
    Delivery&                                   delivery_;
    std::vector<std::unique_ptr<Order>>         orders_;
    std::vector<std::unique_ptr<Courier>>       couriers_;
    std::vector<std::unique_ptr<Kitchener>>     kitcheners_;
    time_point_t                                startTime_;     // program start time
    std::chrono::nanoseconds                    passedTime_;    // time passed since startTime_
    OrderID                                     nextOrderID_;
};

///************************************************************************************************

inline void ManagmentSystem::setCurrentTime() noexcept
{
    passedTime_ = std::chrono::nanoseconds{ 0 };
    startTime_ = std::chrono::system_clock::now();
}

///************************************************************************************************

std::vector<Food> createRandomFood();

} // namespace ds

#endif // !MANAGMENT_SYSTEM_HPP
