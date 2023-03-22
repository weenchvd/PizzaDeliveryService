
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

namespace ds {

class Options {
public:
    static constexpr unsigned int numComplOrders_{ 10 };        // number of completed orders


    static constexpr unsigned int defFPS_{ 60 };
    static constexpr unsigned int minFPS_{ 1 };
    static constexpr unsigned int maxFPS_{ 1000 };

    static constexpr unsigned int defTimeSpeed_{ 1 };
    static constexpr unsigned int minTimeSpeed_{ 1 };
    static constexpr unsigned int maxTimeSpeed_{ 100 };

    static constexpr unsigned int defPaymentTime_{ 240 };       // seconds
    static constexpr unsigned int minPaymentTime_{ 60 };        // seconds
    static constexpr unsigned int maxPaymentTime_{ 300 };       // seconds

    static constexpr unsigned int defCheckTimeFreeCour_{ 1 };   // seconds
    static constexpr unsigned int minCheckTimeFreeCour_{ 1 };   // seconds
    static constexpr unsigned int maxCheckTimeFreeCour_{ 300 }; // seconds

    static constexpr unsigned int defVertexRadius_{ 6 };
    static constexpr unsigned int minVertexRadius_{ 2 };
    static constexpr unsigned int maxVertexRadius_{ 10 };

    static constexpr unsigned int defEdgeWidth_{ 2 };
    static constexpr unsigned int minEdgeWidth_{ 1 };
    static constexpr unsigned int maxEdgeWidth_{ 5 };

private:
    Options();

public:
    Options(const Options&) = delete;
    Options& operator=(const Options&) = delete;

    static Options& instance() { return uniqueInstance_; }

public:
    int                             fps_;                       // frames per second
    int                             timeSpeed_;

    int                             paymentTime_;
    int                             checkTimeFreeCour_;         // time to check free couriers

    int                             vertexRadius_;              // in pixels
    int                             edgeWidth_;                 // in pixels

private:
    static Options                  uniqueInstance_;
};

} // namespace ds

#endif // !OPTIONS_HPP
