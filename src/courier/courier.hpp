
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef COURIER_HPP
#define COURIER_HPP

#include"imgui.h"
#include"map.hpp"
#include"order.hpp"
#include<assert.h>
#include<chrono>
#include<memory>
#include<vector>

namespace ds {

enum class CourierID : unsigned int {};

enum class CourierStatus : char
{
    INVALID = -1,

    WAITING_FOR_NEXT,
    ACCEPTING_ORDER,
    MOVEMENT_TO_CUSTOMER,
    DELIVERY_AND_PAYMENT,
    RETURNING_TO_OFFICE
};

class ManagmentSystem;

class Courier {
public:
    friend class CourierState;
    friend class CourierWaiting;
    friend class CourierAccepting;
    friend class CourierMovement;
    friend class CourierPayment;
    friend class CourierReturning;

public:
    Courier(ManagmentSystem& ms, CourierID courierID);

    Courier(const Courier&) = delete;
    Courier& operator=(const Courier&) = delete;

    virtual ~Courier() noexcept {}

public:
    virtual void update(std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const;

    CourierID getID() const { return id_; }

    ImVec2 getLocation() const { return curLocation_; }

    std::chrono::nanoseconds getWaitingTime() const noexcept { return waitingTime_; }

    void setRouteList(Order* order, std::vector<Graph::edge_descriptor> path);

    const RouteList* getRouteList() const { return routeList_.get(); }

private:
    ImVec2 getOfficeLocation() const;

    void changeState(CourierState& state) { state_ = &state; }

private:
    CourierState*                               state_;
    ManagmentSystem&                            ms_;
    std::unique_ptr<RouteList>                  routeList_;
    std::chrono::nanoseconds                    waitingTime_;
    ImVec2                                      curLocation_;
    CourierID                                   id_;
    CourierStatus                               prevStatus_;
    bool                                        isDelivering_;
};

///************************************************************************************************

class CourierState {
protected:
    CourierState() noexcept {}

public:
    virtual void update(Courier& courier, std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const { return CourierStatus::INVALID; }

protected:
    void changeState(Courier& courier, CourierState& state) { courier.changeState(state); }
};


class CourierWaiting : public CourierState {
protected:
    CourierWaiting() noexcept {}

public:
    static CourierWaiting& instance() noexcept { return state_; }

public:
    virtual void update(Courier& courier, std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const { return CourierStatus::WAITING_FOR_NEXT; }

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

    virtual CourierStatus getStatus() const { return CourierStatus::ACCEPTING_ORDER; }

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

    virtual CourierStatus getStatus() const { return CourierStatus::MOVEMENT_TO_CUSTOMER; }

private:
    static CourierMovement state_;
};


class CourierPayment : public CourierState {
protected:
    CourierPayment() noexcept {}

public:
    static CourierPayment& instance() noexcept { return state_; }

public:
    virtual void update(Courier& courier, std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const { return CourierStatus::DELIVERY_AND_PAYMENT; }

private:
    static CourierPayment state_;
};


class CourierReturning : public CourierState {
protected:
    CourierReturning() noexcept {}

public:
    static CourierReturning& instance() noexcept { return state_; }

public:
    virtual void update(Courier& courier, std::chrono::nanoseconds passedTime);

    virtual CourierStatus getStatus() const { return CourierStatus::RETURNING_TO_OFFICE; }

private:
    static CourierReturning state_;
};

///************************************************************************************************

inline void Courier::update(std::chrono::nanoseconds passedTime)
{
    assert(state_);
    return state_->update(*this, passedTime);
}

inline CourierStatus Courier::getStatus() const
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
