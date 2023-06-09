
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef OPTIONS_HPP
#define OPTIONS_HPP

namespace ds {

class OptionsBase {
public:
    OptionsBase() noexcept {}

    OptionsBase(const OptionsBase&) = delete;
    OptionsBase& operator=(const OptionsBase&) = delete;
};


class OptionsMap : OptionsBase {
public:
    static constexpr unsigned int defScale_{ 10 };
    static constexpr unsigned int minScale_{ 10 };
    static constexpr unsigned int maxScale_{ 10 };

    static constexpr unsigned int defVertexRadius_{ 6 };        // pixels
    static constexpr unsigned int minVertexRadius_{ 2 };        // pixels
    static constexpr unsigned int maxVertexRadius_{ 10 };       // pixels

    static constexpr unsigned int defEdgeWidth_{ 1 };           // pixels
    static constexpr unsigned int minEdgeWidth_{ 1 };           // pixels
    static constexpr unsigned int maxEdgeWidth_{ 5 };           // pixels

public:
    OptionsMap();

public:
    int                             scale_;
    int                             vertexRadius_;              // pixels
    int                             edgeWidth_;                 // pixels
    bool                            showGrid_;
};


class OptionsDelivery : OptionsBase {
public:
    static constexpr unsigned int defDeliveryTime_{ 60 * 55 };  // seconds
    static constexpr unsigned int minDeliveryTime_{ 60 * 30 };  // seconds
    static constexpr unsigned int maxDeliveryTime_{ 60 * 80 };  // seconds

    static constexpr unsigned int defCheckTimeFreeCour_{ 1 };   // seconds
    static constexpr unsigned int minCheckTimeFreeCour_{ 1 };   // seconds
    static constexpr unsigned int maxCheckTimeFreeCour_{ 10 };  // seconds

public:
    OptionsDelivery();

public:
    int                             deliveryTime_;              // seconds
    int                             checkTimeFreeCour_;         // time to check free couriers
};


class OptionsCourier : OptionsBase {
public:
    static constexpr unsigned int defInaccessibleTime_{ 5 };    // seconds

    static constexpr unsigned int defPauseTime_{ 60 * 15 };     // seconds
    static constexpr unsigned int minPauseTime_{ 60 };          // seconds
    static constexpr unsigned int maxPauseTime_{ 60 * 30 };     // seconds

    static constexpr unsigned int defPauseChance_{ 10 };        // percent
    static constexpr unsigned int minPauseChance_{ 0 };         // percent
    static constexpr unsigned int maxPauseChance_{ 100 };       // percent

    static constexpr unsigned int defAcceptanceTime_{ 60 * 4 }; // seconds
    static constexpr unsigned int minAcceptanceTime_{ 60 };     // seconds
    static constexpr unsigned int maxAcceptanceTime_{ 60 * 5 }; // seconds

    static constexpr unsigned int defDeliveryTime_{ 60 * 2 };   // seconds
    static constexpr unsigned int minDeliveryTime_{ 60 };       // seconds
    static constexpr unsigned int maxDeliveryTime_{ 60 * 5 };   // seconds

    static constexpr unsigned int defPaymentTime_{ 60 * 2 };    // seconds
    static constexpr unsigned int minPaymentTime_{ 60 };        // seconds
    static constexpr unsigned int maxPaymentTime_{ 60 * 2 };    // seconds

    static constexpr unsigned int defAverageSpeed_{ 10 };       // meters per second
    static constexpr unsigned int minAverageSpeed_{ 1 };        // meters per second
    static constexpr unsigned int maxAverageSpeed_{ 20 };       // meters per second

public:
    OptionsCourier();

public:
    int                             pauseTime_;                 // seconds
    int                             pauseChance_;               // percent
    int                             acceptanceTime_;            // seconds
    int                             deliveryTime_;              // seconds
    int                             paymentTime_;               // seconds
    int                             averageSpeed_;              // meters per second
};


class Options : OptionsBase {
public:
    static constexpr unsigned int numComplOrders_{ 20 };        // number of completed orders

    static constexpr unsigned int minPauseTime_{ 60 };          // seconds
    static constexpr unsigned int maxPauseTime_{ 60 * 15 };     // seconds

    static constexpr unsigned int pauseChance_{ 4 };            // percent

    static constexpr unsigned int defFPS_{ 60 };
    static constexpr unsigned int minFPS_{ 1 };
    static constexpr unsigned int maxFPS_{ 1000 };

    static constexpr unsigned int defTimeSpeed_{ 1 };
    static constexpr unsigned int minTimeSpeed_{ 1 };
    static constexpr unsigned int maxTimeSpeed_{ 120 };

private:
    Options();

public:
    static Options& instance() { return uniqueInstance_; }

public:
    OptionsMap                      optMap_;
    OptionsDelivery                 optDelivery_;
    OptionsCourier                  optCourier_;

    int                             fps_;                       // frames per second
    int                             timeSpeed_;
private:
    static Options                  uniqueInstance_;
};

} // namespace ds

#endif // !OPTIONS_HPP
