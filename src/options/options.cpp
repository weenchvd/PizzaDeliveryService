
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"options.hpp"

namespace ds {

using namespace std;

Options Options::uniqueInstance_{};

///************************************************************************************************

Options::Options()
    :
    fps_                { defFPS_ },
    timeSpeed_          { defTimeSpeed_ },
    paymentTime_        { defPaymentTime_ },
    checkTimeFreeCour_  { defCheckTimeFreeCour_ },
    vertexRadius_       { defVertexRadius_},
    edgeWidth_          { defEdgeWidth_ }
{
    static_assert(minFPS_ > 0);
    static_assert(defFPS_ >= minFPS_ && defFPS_ <= maxFPS_);
    static_assert(minTimeSpeed_ > 0);
    static_assert(defTimeSpeed_ >= minTimeSpeed_ && defTimeSpeed_ <= maxTimeSpeed_);
    static_assert(minPaymentTime_ > 0);
    static_assert(defPaymentTime_ >= minPaymentTime_ && defPaymentTime_ <= maxPaymentTime_);
    static_assert(minCheckTimeFreeCour_ > 0);
    static_assert(defCheckTimeFreeCour_ >= minCheckTimeFreeCour_
        && defCheckTimeFreeCour_ <= maxCheckTimeFreeCour_);
    static_assert(minVertexRadius_ > 0);
    static_assert(defVertexRadius_ >= minVertexRadius_ && defVertexRadius_ <= maxVertexRadius_);
    static_assert(minEdgeWidth_ > 0);
    static_assert(defEdgeWidth_ >= minEdgeWidth_ && defEdgeWidth_ <= maxEdgeWidth_);
}

} // namespace ds
