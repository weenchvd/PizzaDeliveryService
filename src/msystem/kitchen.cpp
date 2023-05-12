
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include"kitchen.hpp"
#include"msystem.hpp"
#include<assert.h>

namespace ds {

using namespace std;

Kitchen::Kitchen()
    :
    ms_             { nullptr },
    orders_         {},
    queueDough_     {},
    queueFilling_   {},
    queuePicker_    {},
    kitcheners_     {}
{}

void Kitchen::update(chrono::nanoseconds passedTime)
{
    this->distributeOrders();
    for (auto& k : kitcheners_) {
        k->update(passedTime);
    }
    this->processOrders();
}

void Kitchen::distributeOrders()
{
    vector<Kitchener*> making;
    for (auto iter{ kitcheners_.begin() }; iter != kitcheners_.end();) {
        Kitchener* kitchener{ *iter };
        if (kitchener->getStatus() == KitchenerStatus::WAITING_FOR_NEXT) {
            switch (kitchener->getType()) {
            case KitchenerType::DOUGH:
                if (!queueDough_.empty()) {
                    kitchener->makeFood(queueDough_.front());
                    queueDough_.erase(queueDough_.begin());
                    iter = kitcheners_.erase(iter);
                    making.push_back(kitchener);
                    continue;
                }
                break;
            case KitchenerType::FILLING:
                if (!queueFilling_.empty()) {
                    kitchener->makeFood(queueFilling_.front());
                    queueFilling_.erase(queueFilling_.begin());
                    iter = kitcheners_.erase(iter);
                    making.push_back(kitchener);
                    continue;
                }
                break;
            case KitchenerType::PICKER:
                if (!queuePicker_.empty()) {
                    kitchener->makeFood(queuePicker_.front());
                    queuePicker_.erase(queuePicker_.begin());
                    iter = kitcheners_.erase(iter);
                    making.push_back(kitchener);
                    continue;
                }
                break;
            default:
                assert(false);
                break;
            }
        }
        ++iter;
    }
    kitcheners_.insert(kitcheners_.end(), making.begin(), making.end());
}

void Kitchen::processOrders()
{
    assert(ms_ != nullptr);
    for (auto iter{ orders_.begin() }; iter != orders_.end();) {
        Order* order{ *iter };
        bool orderDone{ true };
        bool orderCooking{ false };
        for (const auto& food : order->getFood()) {
            const FoodStatus status{ food.getStatus() };
            if (status != FoodStatus::DONE) {
                orderDone = false;
                if (status == FoodStatus::MAKING) {
                    orderCooking = true;
                }
            }
        }
        if (orderDone) {
            iter = orders_.erase(iter);
            order->setStatus(OrderStatus::COOKING_COMPLETED);
            ms_->processOrder(order);
        }
        else {
            ++iter;
            if (orderCooking) {
                order->setStatus(OrderStatus::COOKING);
            }
        }
    }
}

void Kitchen::makeOrder(Order* order)
{
    assert(order != nullptr);
    order->setStatus(OrderStatus::WAITING_FOR_COOKING);
    orders_.push_back(order);
    for (auto& food : order->getFood()) {
        switch (food.getType()) {
        case FoodType::PIZZA:
            queueDough_.push_back(&food);
            break;
        case FoodType::SIDES:
            queueFilling_.push_back(&food);
            break;
        case FoodType::DRINKS:
            queuePicker_.push_back(&food);
            break;
        default:
            assert(false);
            break;
        }
    }
}

void Kitchen::addKitchener(Kitchener* kitchener)
{
    assert(kitchener != nullptr);
    kitcheners_.push_back(kitchener);
}

void Kitchen::deleteKitchener(Kitchener* kitchener)
{
    assert(kitchener != nullptr);
    for (auto iter{ kitcheners_.begin() }; iter != kitcheners_.end(); ++iter) {
        if (*iter == kitchener) {
            kitcheners_.erase(iter);
            break;
        }
    }
}

const Order& Kitchen::getOrder(Food* food) const
{
    assert(food != nullptr);
    for (Order* order : orders_) {
        for (Food& f : order->getFood()) {
            if (food == &f) {
                return *order;
            }
        }
    }
    throw;
}

void Kitchen::pushFrontQueue(std::vector<Food*>& queue, Food* food)
{
    assert(food != nullptr);
    OrderID orderID{ this->getOrder(food).getID() };
    auto iter{ queue.cbegin() };
    for (; iter != queue.cend(); ++iter) {
        if (orderID < this->getOrder(*iter).getID()) {
            break;
        }
    }
    queue.insert(iter, food);
}

} // namespace ds
