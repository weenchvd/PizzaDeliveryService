
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include"map.hpp"
#include"order.hpp"
#include<boost/circular_buffer.hpp>
#include<chrono>
#include<utility>

namespace ds {

class ManagmentSystem;

class Scheduler {
public:
    Scheduler(Graph::vertex_descriptor office);

    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;

    virtual ~Scheduler() noexcept {}

public:
    void processOrder(Order* order);

    void setManagmentSystem(ManagmentSystem* ms) { ms_ = ms; }

    Graph::vertex_descriptor getOffice() const noexcept { return office_; }

    auto getOrdersCompleted() const { return std::pair{ ordersCompl_.cbegin(), ordersCompl_.cend() }; }

    auto getOrdersCompleted() { return std::pair{ ordersCompl_.begin(), ordersCompl_.end() }; }

private:
    ManagmentSystem*                            ms_;
    Graph::vertex_descriptor                    office_;
    boost::circular_buffer<Order*>              ordersCompl_;   // completed orders
};

} // namespace ds

#endif // !SCHEDULER_HPP
