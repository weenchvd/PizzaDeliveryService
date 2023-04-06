
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef KITCHEN_HPP
#define KITCHEN_HPP

#include"kitchener.hpp"
#include"order.hpp"
#include<chrono>
#include<utility>
#include<vector>

namespace ds {

class ManagmentSystem;

class Kitchen {
public:
    friend KitchenerMaking;

public:
    Kitchen();

    Kitchen(const Kitchen&) = delete;
    Kitchen& operator=(const Kitchen&) = delete;

    virtual ~Kitchen() noexcept {}

public:
    void update(std::chrono::nanoseconds passedTime);

    void makeOrder(Order* order);

    void addKitchener(Kitchener* kitchener);

    void deleteKitchener(Kitchener* kitchener);

    void setManagmentSystem(ManagmentSystem* ms) { ms_ = ms; }

    auto getOrders() const { return std::pair{ orders_.cbegin(), orders_.cend() }; }

    //auto getOrders() { return std::pair{ orders_.begin(), orders_.end() }; }

private:
    void distributeOrders();

    void processOrders();

    Order& getOrder(Food* food);

    void pushFrontQueueDough(Food* food) { pushFrontQueue(queueDough_, food); }

    void pushFrontQueueFilling(Food* food) { pushFrontQueue(queueFilling_, food); }

    void pushFrontQueuePicker(Food* food) { pushFrontQueue(queuePicker_, food); }

    void pushFrontQueue(std::vector<Food*>& queue, Food* food);

private:
    ManagmentSystem*                            ms_;
    std::vector<Order*>                         orders_;        // current orders in the kitchen
    std::vector<Food*>                          queueDough_;
    std::vector<Food*>                          queueFilling_;
    std::vector<Food*>                          queuePicker_;
    std::vector<Kitchener*>                     kitcheners_;    // working kitcheners
};

} // namespace ds

#endif // !KITCHEN_HPP
