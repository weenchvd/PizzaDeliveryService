
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include"courier.hpp"
#include"msystem.hpp"
#include"options.hpp"
#include<utility>

namespace ds {

using namespace std;

string toString(CourierStatus value)
{
    switch (value) {
    case CourierStatus::__INVALID:
        return u8"INVALID STATUS";
    case CourierStatus::INACCESSIBLE:
        return u8"Inaccessible";
    case CourierStatus::WAITING_FOR_NEXT:
        return u8"Waiting for next";
    case CourierStatus::ACCEPTING_ORDER:
        return u8"Accepting order";
    case CourierStatus::MOVEMENT_TO_CUSTOMER:
        return u8"Movement to customer";
    case CourierStatus::DELIVERY_AND_PAYMENT:
        return u8"Delivery and payment";
    case CourierStatus::RETURNING_TO_OFFICE:
        return u8"Returning to office";
    default:
        return u8"UNKNOWN";
    }
}

///************************************************************************************************

CourierInaccessible     CourierInaccessible::state_{};
CourierWaiting          CourierWaiting::state_{};
CourierAccepting        CourierAccepting::state_{};
CourierMovement         CourierMovement::state_{};
CourierDeliveryPayment  CourierDeliveryPayment::state_{};
CourierReturning        CourierReturning::state_{};

///************************************************************************************************

Courier::Courier(ManagmentSystem& ms, WorkerID workerID)
    :
    state_          { &CourierWaiting::instance() },
    ms_             { ms },
    route_          { nullptr },
    passedTime_     { 0 },
    makingTime_     { 0 },
    curOrder_       {},
    curEdge_        {},
    passedDist_     { 0 },
    fullDist_       { 0 },
    curLocation_    { Courier::getInaccessibleLocation() },
    id_             { workerID },
    prevStatus_     { CourierStatus::__INVALID }
{}

inline ImVec2 Courier::getLocation(size_t target) const
{
    return ImVec2{
        float(ms_.map().graph().m_vertices[target].m_property.x_),
        float(ms_.map().graph().m_vertices[target].m_property.y_)
    };
}

inline ImVec2 Courier::getOfficeLocation() const
{
    return getLocation(ms_.scheduler().getOffice());
}

inline long long int Courier::calculateFullDistance() const
{
    const auto& edge{ *curEdge_ };
    return ms_.map().graph()[edge].distance_ * nano::den;
}

ImVec2 Courier::calculateCurrentLocation()
{
    const double t{ double(passedDist_) / fullDist_ };
    const auto& graph{ ms_.map().graph() };
    const auto& edge{ *curEdge_ };
    const int x{ int(
        graph[edge.m_source].x_ + (graph[edge.m_target].x_ - graph[edge.m_source].x_) * t
    ) };
    const int y{ int(
        graph[edge.m_source].y_ + (graph[edge.m_target].y_ - graph[edge.m_source].y_) * t
    ) };
    return ImVec2{ float(x), float(y) };
}

///************************************************************************************************

void CourierInaccessible::update(Courier& courier, std::chrono::nanoseconds passedTime)
{
    if (courier.prevStatus_ != CourierStatus::INACCESSIBLE) {
        courier.prevStatus_ = CourierStatus::INACCESSIBLE;
        courier.curLocation_ = courier.getInaccessibleLocation();
        courier.route_.reset();
        courier.curOrder_ = vector<Order*>::iterator{};
        courier.curEdge_ = Courier::edge_const_iterator_t{};
        int random{ cmn::getRandomNumber(1, 100) };
        courier.makingTime_ =
            (random >= 1 && random <= Options::instance().optCourier_.pauseChance_) ?
            chrono::seconds{ cmn::getRandomNumber(
                    OptionsCourier::minPauseTime_,
                    Options::instance().optCourier_.pauseTime_
            )} :
            chrono::seconds{ OptionsCourier::defInaccessibleTime_ };
    }
    courier.passedTime_ += passedTime;
    if (courier.passedTime_ >= courier.makingTime_) {
        courier.passedTime_ -= courier.makingTime_;
        this->changeState(courier, CourierWaiting::instance());
    }
}

void CourierWaiting::update(Courier& courier, std::chrono::nanoseconds passedTime)
{
    if (courier.prevStatus_ != CourierStatus::WAITING_FOR_NEXT) {
        courier.prevStatus_ = CourierStatus::WAITING_FOR_NEXT;
        courier.curLocation_ = courier.getOfficeLocation();
        courier.makingTime_ = chrono::nanoseconds{ 0 };
    }
    courier.passedTime_ += passedTime;
    courier.makingTime_ += passedTime;
    if (courier.route_ != nullptr) {
        courier.passedTime_ -= courier.makingTime_;
        this->changeState(courier, CourierAccepting::instance());
    }
}

void CourierAccepting::update(Courier& courier, std::chrono::nanoseconds passedTime)
{
    if (courier.prevStatus_ != CourierStatus::ACCEPTING_ORDER) {
        courier.prevStatus_ = CourierStatus::ACCEPTING_ORDER;
        courier.curOrder_ = courier.route_->getOrders().begin();
        courier.curEdge_ = courier.route_->getPath().cbegin();
        courier.curLocation_ = courier.getOfficeLocation();
        courier.makingTime_ = chrono::seconds{ cmn::getRandomNumber(
            OptionsCourier::minAcceptanceTime_,
            Options::instance().optCourier_.acceptanceTime_
        )};
        for (Order* order : courier.route_->getOrders()) {
            order->setStatus(OrderStatus::DELIVERING);
        }
    }
    courier.passedTime_ += passedTime;
    if (courier.passedTime_ >= courier.makingTime_) {
        courier.passedTime_ -= courier.makingTime_;
        this->changeState(courier, CourierMovement::instance());
    }
}

void CourierMovement::update(Courier& courier, std::chrono::nanoseconds passedTime)
{
    if (courier.prevStatus_ != CourierStatus::MOVEMENT_TO_CUSTOMER) {
        courier.prevStatus_ = CourierStatus::MOVEMENT_TO_CUSTOMER;
        courier.passedDist_ = 0;
        courier.fullDist_ = courier.calculateFullDistance();
        courier.makingTime_ = chrono::nanoseconds{ 0 };
    }
    courier.passedTime_ += passedTime;
    courier.makingTime_ += passedTime;
    courier.passedDist_ += passedTime.count() * Options::instance().optCourier_.averageSpeed_;
    if (courier.passedDist_ >= courier.fullDist_) {
        courier.passedDist_ -= courier.fullDist_;
        if (courier.curEdge_->m_target == (*courier.curOrder_)->getTarget()) {
            courier.curLocation_ = courier.getLocation(courier.curEdge_->m_target);
            courier.passedTime_ -= courier.makingTime_;
            this->changeState(courier, CourierDeliveryPayment::instance());
            return;
        }
        else {
            ++courier.curEdge_;
            courier.fullDist_ = courier.calculateFullDistance();
        }
    }
    courier.curLocation_ = courier.calculateCurrentLocation();
}

void CourierDeliveryPayment::update(Courier& courier, std::chrono::nanoseconds passedTime)
{
    if (courier.prevStatus_ != CourierStatus::DELIVERY_AND_PAYMENT) {
        courier.prevStatus_ = CourierStatus::DELIVERY_AND_PAYMENT;
        courier.curLocation_ = courier.getLocation(courier.curEdge_->m_target);
        courier.makingTime_ = chrono::seconds{ cmn::getRandomNumber(
            Options::instance().optCourier_.minDeliveryTime_,
            Options::instance().optCourier_.deliveryTime_
        ) };
    }
    courier.passedTime_ += passedTime;
    if (courier.passedTime_ >= courier.makingTime_) {
        courier.passedTime_ -= courier.makingTime_;
        if ((*courier.curOrder_)->isPaid() == true) {
            (*courier.curOrder_)->setStatus(OrderStatus::DELIVERING_COMPLETED);
            if (courier.curOrder_ == --courier.route_->getOrders().end()) {
                assert(courier.curEdge_ == --courier.route_->getPath().cend());
                this->changeState(courier, CourierReturning::instance());
                return;
            }
            ++courier.curOrder_;
            if (courier.curEdge_->m_target == (*courier.curOrder_)->getTarget()) {
                courier.prevStatus_ = CourierStatus::MOVEMENT_TO_CUSTOMER;
                return;
            }
            ++courier.curEdge_;
            assert(courier.curEdge_ != courier.route_->getPath().cend());
            this->changeState(courier, CourierMovement::instance());
            return;
        }
        courier.makingTime_ = chrono::seconds{ cmn::getRandomNumber(
            Options::instance().optCourier_.minPaymentTime_,
            Options::instance().optCourier_.paymentTime_
        ) };
        (*courier.curOrder_)->setStatus(OrderStatus::PAYING);
        (*courier.curOrder_)->isPaid(true);
    }
}

void CourierReturning::update(Courier& courier, std::chrono::nanoseconds passedTime)
{
    if (courier.prevStatus_ != CourierStatus::RETURNING_TO_OFFICE) {
        courier.prevStatus_ = CourierStatus::RETURNING_TO_OFFICE;
        const auto source{ courier.curEdge_->m_target };
        auto path{ courier.ms_.map().getPath(source, courier.ms_.scheduler().getOffice()) };
        unique_ptr<Route> route{ new Route{ courier.ms_, vector<Order*>{}, std::move(path)}};
        courier.route_ = std::move(route);
        courier.curOrder_ = vector<Order*>::iterator{};
        courier.curEdge_ = courier.route_->getPath().cbegin();
        courier.passedDist_ = 0;
        courier.fullDist_ = courier.calculateFullDistance();
        courier.makingTime_ = chrono::nanoseconds{ 0 };
    }
    courier.passedTime_ += passedTime;
    courier.makingTime_ += passedTime;
    courier.passedDist_ += passedTime.count() * Options::instance().optCourier_.averageSpeed_;
    if (courier.passedDist_ >= courier.fullDist_) {
        courier.passedDist_ -= courier.fullDist_;
        if (courier.curEdge_ == --courier.route_->getPath().cend()) {
            courier.curLocation_ = courier.getLocation(courier.curEdge_->m_target);
            courier.passedTime_ -= courier.makingTime_;
            this->changeState(courier, CourierInaccessible::instance());
            return;
        }
        else {
            ++courier.curEdge_;
            courier.fullDist_ = courier.calculateFullDistance();
        }
    }
    courier.curLocation_ = courier.calculateCurrentLocation();
}

} // namespace ds
