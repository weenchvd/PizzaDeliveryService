
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"options.hpp"

namespace ds {

using namespace std;

Options Options::uniqueInstance_{};

///************************************************************************************************

OptionsMap::OptionsMap()
    :
    scale_              { defScale_},
    vertexRadius_       { defVertexRadius_},
    edgeWidth_          { defEdgeWidth_ }
{
    static_assert(minScale_ > 0);
    static_assert(defScale_ >= minScale_ && defScale_ <= maxScale_);
    static_assert(minVertexRadius_ > 0);
    static_assert(defVertexRadius_ >= minVertexRadius_ && defVertexRadius_ <= maxVertexRadius_);
    static_assert(minEdgeWidth_ > 0);
    static_assert(defEdgeWidth_ >= minEdgeWidth_ && defEdgeWidth_ <= maxEdgeWidth_);
}

OptionsDelivery::OptionsDelivery()
    :
    deliveryTime_       { defDeliveryTime_ },
    checkTimeFreeCour_  { defCheckTimeFreeCour_ }
{
    static_assert(minDeliveryTime_ > 0);
    static_assert(defDeliveryTime_ >= minDeliveryTime_ && defDeliveryTime_ <= maxDeliveryTime_);
    static_assert(minCheckTimeFreeCour_ > 0);
    static_assert(defCheckTimeFreeCour_ >= minCheckTimeFreeCour_
        && defCheckTimeFreeCour_ <= maxCheckTimeFreeCour_);
}

OptionsCourier::OptionsCourier()
    :
    pauseTime_          { defPauseTime_ },
    pauseChance_        { defPauseChance_ },
    acceptanceTime_     { defAcceptanceTime_ },
    deliveryTime_       { defDeliveryTime_ },
    paymentTime_        { defPaymentTime_ },
    averageSpeed_       { defAverageSpeed_ }
{
    static_assert(defInaccessibleTime_ >= 0);
    static_assert(minPauseTime_ > 0);
    static_assert(defPauseTime_ >= minPauseTime_ && defPauseTime_ <= maxPauseTime_);
    static_assert(minPauseChance_ >= 0);
    static_assert(defPauseChance_ >= minPauseChance_ && defPauseChance_ <= maxPauseChance_);
    static_assert(minAcceptanceTime_ > 0);
    static_assert(defAcceptanceTime_ >= minAcceptanceTime_ && defAcceptanceTime_ <= maxAcceptanceTime_);
    static_assert(minDeliveryTime_ > 0);
    static_assert(defDeliveryTime_ >= minDeliveryTime_ && defDeliveryTime_ <= maxDeliveryTime_);
    static_assert(minPaymentTime_ > 0);
    static_assert(defPaymentTime_ >= minPaymentTime_ && defPaymentTime_ <= maxPaymentTime_);
    static_assert(minAverageSpeed_ > 0);
    static_assert(defAverageSpeed_ >= minAverageSpeed_ && defAverageSpeed_ <= maxAverageSpeed_);
}

Options::Options()
    :
    optMap_             {},
    optDelivery_        {},
    optCourier_         {},
    fps_                { defFPS_ },
    timeSpeed_          { defTimeSpeed_ }
{
    static_assert(minFPS_ > 0);
    static_assert(defFPS_ >= minFPS_ && defFPS_ <= maxFPS_);
    static_assert(minTimeSpeed_ > 0);
    static_assert(defTimeSpeed_ >= minTimeSpeed_ && defTimeSpeed_ <= maxTimeSpeed_);
}

} // namespace ds
