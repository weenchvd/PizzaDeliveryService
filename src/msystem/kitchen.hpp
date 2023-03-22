
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef KITCHEN_HPP
#define KITCHEN_HPP

#include"order.hpp"
#include<chrono>
#include<utility>
#include<vector>

namespace ds {

class ManagmentSystem;

class Kitchen {
public:
    Kitchen();

    Kitchen(const Kitchen&) = delete;
    Kitchen& operator=(const Kitchen&) = delete;

    virtual ~Kitchen() noexcept {}

public:
    void update(std::chrono::nanoseconds passedTime);

    void makeOrder(Order* order);

    void setManagmentSystem(ManagmentSystem* ms) { ms_ = ms; }

    auto getOrders() const { return std::pair{ ordersCooking_.cbegin(), ordersCooking_.cend() }; }

    auto getOrders() { return std::pair{ ordersCooking_.begin(), ordersCooking_.end() }; }

private:
    ManagmentSystem*                            ms_;
    std::vector<Order*>                         ordersCooking_; // current orders with "Cooking" status
    std::vector<std::chrono::nanoseconds>       remainingTime_;
};

} // namespace ds

#endif // !KITCHEN_HPP
