
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

CourierWaiting          CourierWaiting::state_{};
CourierAccepting        CourierAccepting::state_{};
CourierMovement         CourierMovement::state_{};
CourierPayment          CourierPayment::state_{};
CourierReturning        CourierReturning::state_{};

///************************************************************************************************

Courier::Courier(ManagmentSystem& ms, CourierID courierID)
    :
    state_          { &CourierWaiting::instance() },
    ms_             { ms },
    routeList_      { nullptr },
    waitingTime_    { 0 },
    curLocation_    { 0.0f, 0.0f },
    id_             { courierID },
    prevStatus_     { CourierStatus::INVALID },
    isDelivering_   { false }
{}

void Courier::setRouteList(Order* order, std::vector<Graph::edge_descriptor> path)
{
    unique_ptr<RouteList> newRL{ new RouteList{ ms_, order, std::move(path) } };
    routeList_ = move(newRL);
    changeState(CourierAccepting::instance());
}

ImVec2 Courier::getOfficeLocation() const
{
    return ImVec2{
        float(ms_.map().graph().m_vertices[ms_.scheduler().getOffice()].m_property.x_),
        float(ms_.map().graph().m_vertices[ms_.scheduler().getOffice()].m_property.y_)
    };
}

///************************************************************************************************

void CourierWaiting::update(Courier& courier, std::chrono::nanoseconds passedTime)
{
    if (courier.prevStatus_ != CourierStatus::WAITING_FOR_NEXT) {
        courier.waitingTime_ = chrono::nanoseconds{ 0 };
    }
    courier.prevStatus_ = CourierStatus::WAITING_FOR_NEXT;
    courier.waitingTime_ += passedTime;
    courier.curLocation_ = courier.getOfficeLocation();
}

void CourierAccepting::update(Courier& courier, std::chrono::nanoseconds passedTime)
{
    if (courier.prevStatus_ != CourierStatus::ACCEPTING_ORDER) {
        courier.routeList_->requiredTime_ += chrono::nanoseconds{
            cmn::getRandomNumber(60 * 2, 60 * 4) * nano::den
        };
        courier.routeList_->order_->setStatus(OrderStatus::DELIVERING);
    }
    courier.prevStatus_ = CourierStatus::ACCEPTING_ORDER;
    courier.routeList_->requiredTime_ -= passedTime;
    if (courier.routeList_->requiredTime_.count() <= 0) {
        courier.isDelivering_ = true;
        this->changeState(courier, CourierMovement::instance());
    }
    courier.curLocation_ = courier.getOfficeLocation();
}

void CourierMovement::update(Courier& courier, std::chrono::nanoseconds passedTime)
{
    if (courier.prevStatus_ != CourierStatus::MOVEMENT_TO_CUSTOMER) {
        courier.routeList_->passedDist_ = 0;
        const auto& edge{ courier.routeList_->path_[courier.routeList_->curEdge_] };
        courier.routeList_->fullDist_ = courier.ms_.map().graph()[edge].distance_ * nano::den;
    }
    courier.prevStatus_ = CourierStatus::MOVEMENT_TO_CUSTOMER;
    courier.routeList_->passedDist_ += passedTime.count() * Map::avgSpeed_;
    if (courier.routeList_->passedDist_ >= courier.routeList_->fullDist_) {
        ++courier.routeList_->curEdge_;
        if (courier.routeList_->curEdge_ >= courier.routeList_->path_.size()) {
            if (courier.isDelivering_) {
                this->changeState(courier, CourierPayment::instance());
            }
            else {
                this->changeState(courier, CourierWaiting::instance());
            }
            const auto target{
                courier.routeList_->path_[courier.routeList_->path_.size() - 1].m_target
            };
            courier.curLocation_ = ImVec2 {
                float(courier.ms_.map().graph().m_vertices[target].m_property.x_),
                float(courier.ms_.map().graph().m_vertices[target].m_property.y_)
            };
            return;
        }
        courier.routeList_->passedDist_ -= courier.routeList_->fullDist_;
        const auto& edge{ courier.routeList_->path_[courier.routeList_->curEdge_] };
        courier.routeList_->fullDist_ = courier.ms_.map().graph()[edge].distance_ * nano::den;
    }
    const double t{ double(courier.routeList_->passedDist_) / courier.routeList_->fullDist_ };
    const auto& g{ courier.ms_.map().graph() };
    const auto& edge{ courier.routeList_->path_[courier.routeList_->curEdge_] };
    const int x{
        int(g[edge.m_source].x_ + (g[edge.m_target].x_ - g[edge.m_source].x_) * t)
    };
    const int y{
        int(g[edge.m_source].y_ + (g[edge.m_target].y_ - g[edge.m_source].y_) * t)
    };
    courier.curLocation_ = ImVec2{ float(x), float(y) };
}

void CourierPayment::update(Courier& courier, std::chrono::nanoseconds passedTime)
{
    if (courier.prevStatus_ != CourierStatus::DELIVERY_AND_PAYMENT) {
        courier.routeList_->requiredTime_ += chrono::nanoseconds{
            cmn::getRandomNumber(Options::minPaymentTime_, Options::instance().paymentTime_)
            * nano::den
        };
    }
    courier.prevStatus_ = CourierStatus::DELIVERY_AND_PAYMENT;
    courier.routeList_->requiredTime_ -= passedTime;
    if (courier.routeList_->requiredTime_.count() <= 0) {
        //++courier.curEdge_;
        //if (courier.curEdge_ < courier.path_.size()) {
        //    this->changeState(courier, CourierMovement::instance());
        //}
        //else {
        //    this->changeState(courier, CourierReturning::instance());
        //}
        courier.routeList_->order_->setStatus(OrderStatus::PAYMENT_COMPLETED);
        this->changeState(courier, CourierReturning::instance());
    }
    const auto target{ courier.routeList_->path_[courier.routeList_->path_.size() - 1].m_target};
    courier.curLocation_ = ImVec2 {
        float(courier.ms_.map().graph().m_vertices[target].m_property.x_),
        float(courier.ms_.map().graph().m_vertices[target].m_property.y_)
    };
}

void CourierReturning::update(Courier& courier, std::chrono::nanoseconds passedTime)
{
    const auto target{ courier.routeList_->path_[courier.routeList_->path_.size() - 1].m_target };
    courier.routeList_->curEdge_ = 0;
    courier.routeList_->path_ =
        courier.ms_.map().getPath(target, courier.ms_.scheduler().getOffice());
    courier.isDelivering_ = false;
    this->changeState(courier, CourierMovement::instance());
    courier.curLocation_ = ImVec2 {
        float(courier.ms_.map().graph().m_vertices[target].m_property.x_),
        float(courier.ms_.map().graph().m_vertices[target].m_property.y_)
    };
}

} // namespace ds
