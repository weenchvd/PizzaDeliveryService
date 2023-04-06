
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"delivery.hpp"
#include"kitchen.hpp"
#include"msystem.hpp"
#include"options.hpp"
#include"scheduler.hpp"
#include<assert.h>

namespace ds {

using namespace std;

Scheduler::Scheduler(Graph::vertex_descriptor office)
    :
    ms_             { nullptr },
    office_         { office },
    orders_         {}
{
    orders_.set_capacity(Options::numComplOrders_);
}

void Scheduler::processOrder(Order* order)
{
    assert(order != nullptr);
    switch (order->getStatus()) {
    case OrderStatus::ACCEPTED:
        ms_->kitchen().makeOrder(order);
        break;
    case OrderStatus::COOKING_COMPLETED:
        ms_->delivery().deliveryOrder(order);
        break;
    case OrderStatus::PAYMENT_COMPLETED:
        order->setStatus(OrderStatus::COMPLETED);
        order->setTimeEnd(ms_->getCurrentTime());
        orders_.push_back(order);
        break;
    }
}

} // namespace ds
