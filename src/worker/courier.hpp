
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef COURIER_HPP
#define COURIER_HPP

#include"imgui.h"
#include"map.hpp"
#include"order.hpp"
#include"worker.hpp"
#include<assert.h>
#include<chrono>
#include<limits>
#include<memory>
#include<vector>

namespace ds {

enum class CourierStatus : char {
    __INVALID = -1,                 /// invalid, must be the first
    // vvv STATUSES vvv
    INACCESSIBLE,
    WAITING_FOR_NEXT,
    ACCEPTING_ORDER,
    MOVEMENT_TO_CUSTOMER,
    DELIVERY_AND_PAYMENT,
    RETURNING_TO_OFFICE,
    // ^^^ STATUSES ^^^
    __NUMBER_OF,
    __END                           /// must be the last
};

std::string toString(CourierStatus value);

///************************************************************************************************

class ManagmentSystem;

class Courier {
public:
    friend class CourierState;
    friend class CourierInaccessible;
    friend class CourierWaiting;
    friend class CourierAccepting;
    friend class CourierMovement;
    friend class CourierDeliveryPayment;
    friend class CourierReturning;

public:
    using edge_const_iterator_t = std::vector<Graph::edge_descriptor>::const_iterator;

public:
    Courier(ManagmentSystem& ms, WorkerID workerID);

    Courier(const Courier&) = delete;
    Courier& operator=(const Courier&) = delete;

    virtual ~Courier() noexcept {}

public:
    virtual void update(std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const noexcept;

    WorkerID getID() const noexcept { return id_; }

    auto getCurrentOrder() const noexcept { return curOrder_; }

    ImVec2 getCurrentLocation() const noexcept { return curLocation_; }

    void setRoute(std::unique_ptr<Route>& route) { route_ = std::move(route); }

    const Route* getRoute() const { return route_.get(); }

private:
    ImVec2 getLocation(size_t target) const;

    ImVec2 calculateCurrentLocation();

    ImVec2 getOfficeLocation() const;

    static ImVec2 getInaccessibleLocation();

    long long int calculateFullDistance() const;

    void changeState(CourierState& state) { state_ = &state; }

private:
    CourierState*                               state_;
    ManagmentSystem&                            ms_;
    std::unique_ptr<Route>                      route_;
    std::chrono::nanoseconds                    passedTime_;
    std::chrono::nanoseconds                    makingTime_;
    std::vector<Order*>::iterator               curOrder_;      // current order being delivered
    edge_const_iterator_t                       curEdge_;       // current traversable edge on the path
    long long int                               passedDist_;    // passed distance for current edge, nanometers
    long long int                               fullDist_;      // full distance of current edge, nanometers
    ImVec2                                      curLocation_;
    WorkerID                                    id_;
    CourierStatus                               prevStatus_;
};

///************************************************************************************************

class CourierState {
protected:
    CourierState() noexcept {}

public:
    virtual void update(Courier& courier, std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const noexcept { return CourierStatus::__INVALID; }

protected:
    void changeState(Courier& courier, CourierState& state) { courier.changeState(state); }
};


class CourierInaccessible : public CourierState {
protected:
    CourierInaccessible() noexcept {}

public:
    static CourierInaccessible& instance() noexcept { return state_; }

public:
    virtual void update(Courier& courier, std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const noexcept { return CourierStatus::INACCESSIBLE; }

private:
    static CourierInaccessible state_;
};


class CourierWaiting : public CourierState {
protected:
    CourierWaiting() noexcept {}

public:
    static CourierWaiting& instance() noexcept { return state_; }

public:
    virtual void update(Courier& courier, std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const noexcept { return CourierStatus::WAITING_FOR_NEXT; }

private:
    static CourierWaiting state_;
};


class CourierAccepting : public CourierState {
protected:
    CourierAccepting() noexcept {}

public:
    static CourierAccepting& instance() noexcept { return state_; }

public:
    virtual void update(Courier& courier, std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const noexcept { return CourierStatus::ACCEPTING_ORDER; }

private:
    static CourierAccepting state_;
};


class CourierMovement : public CourierState {
protected:
    CourierMovement() noexcept {}

public:
    static CourierMovement& instance() noexcept { return state_; }

public:
    virtual void update(Courier& courier, std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const noexcept { return CourierStatus::MOVEMENT_TO_CUSTOMER; }

private:
    static CourierMovement state_;
};


class CourierDeliveryPayment : public CourierState {
protected:
    CourierDeliveryPayment() noexcept {}

public:
    static CourierDeliveryPayment& instance() noexcept { return state_; }

public:
    virtual void update(Courier& courier, std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const noexcept { return CourierStatus::DELIVERY_AND_PAYMENT; }

private:
    static CourierDeliveryPayment state_;
};


class CourierReturning : public CourierState {
protected:
    CourierReturning() noexcept {}

public:
    static CourierReturning& instance() noexcept { return state_; }

public:
    virtual void update(Courier& courier, std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const noexcept { return CourierStatus::RETURNING_TO_OFFICE; }

private:
    static CourierReturning state_;
};

///************************************************************************************************

inline void Courier::update(std::chrono::nanoseconds passedTime)
{
    assert(state_);
    return state_->update(*this, passedTime);
}

inline ImVec2 Courier::getInaccessibleLocation()
{
    return ImVec2{
        std::numeric_limits<float>::min() + 1.0e+10F,
        std::numeric_limits<float>::min() + 1.0e+10F
    };
}

inline CourierStatus Courier::getStatus() const noexcept
{
    assert(state_);
    return state_->getStatus();
}

inline void CourierState::update(Courier& courier, std::chrono::nanoseconds passedTime)
{
    courier.update(passedTime);
}

} // namespace ds

#endif // !COURIER_HPP
