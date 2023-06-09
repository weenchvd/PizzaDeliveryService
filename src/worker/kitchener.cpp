
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include"common.hpp"
#include"kitchener.hpp"
#include"msystem.hpp"
#include"options.hpp"
#include<utility>

namespace ds {

using namespace std;

string toString(KitchenerType value)
{
    switch (value) {
    case KitchenerType::__INVALID:
        return u8"INVALID";
    case KitchenerType::DOUGH:
        return u8"Dough";
    case KitchenerType::FILLING:
        return u8"Filling";
    case KitchenerType::PICKER:
        return u8"Picker";
    default:
        return u8"UNKNOWN";
    }
    static_assert(cmn::numberOf<KitchenerType>() == 3);
}

string toString(KitchenerStatus value)
{
    switch (value) {
    case KitchenerStatus::__INVALID:
        return u8"INVALID";
    case KitchenerStatus::INACCESSIBLE:
        return u8"Inaccessible";
    case KitchenerStatus::WAITING_FOR_NEXT:
        return u8"Waiting for next";
    case KitchenerStatus::MAKING:
        return u8"Making";
    default:
        return u8"UNKNOWN";
    }
    static_assert(cmn::numberOf<KitchenerStatus>() == 3);
}

///************************************************************************************************

KitchenerInaccessible   KitchenerInaccessible::state_{};
KitchenerWaiting        KitchenerWaiting::state_{};
KitchenerMaking         KitchenerMaking::state_{};

///************************************************************************************************

Kitchener::Kitchener(ManagmentSystem& ms, WorkerID workerID, KitchenerType type)
    :
    state_          { &KitchenerWaiting::instance() },
    ms_             { ms },
    food_           { nullptr },
    passedTime_     { 0 },
    makingTime_     { 0 },
    id_             { workerID },
    type_           { type },
    prevStatus_     { KitchenerStatus::__INVALID }
{}

///************************************************************************************************

void KitchenerInaccessible::update(Kitchener& kitchener, std::chrono::nanoseconds passedTime)
{
    assert(kitchener.food_ == nullptr);
    if (kitchener.prevStatus_ != KitchenerStatus::INACCESSIBLE) {
        int random{ cmn::getRandomNumber(1, 100) };
        kitchener.makingTime_ = (random >= 1 && random <= Options::pauseChance_) ?
            chrono::seconds{ cmn::getRandomNumber(Options::minPauseTime_, Options::maxPauseTime_) } :
            chrono::seconds{ 5 };
    }
    kitchener.prevStatus_ = KitchenerStatus::INACCESSIBLE;
    kitchener.passedTime_ += passedTime;
    if (kitchener.passedTime_ >= kitchener.makingTime_) {
        kitchener.passedTime_ -= kitchener.makingTime_;
        this->changeState(kitchener, KitchenerWaiting::instance());
    }
}

void KitchenerWaiting::update(Kitchener& kitchener, std::chrono::nanoseconds passedTime)
{
    if (kitchener.prevStatus_ != KitchenerStatus::WAITING_FOR_NEXT) {
        kitchener.makingTime_ = chrono::nanoseconds{ 0 };
    }
    kitchener.prevStatus_ = KitchenerStatus::WAITING_FOR_NEXT;
    kitchener.passedTime_ += passedTime;
    kitchener.makingTime_ += passedTime;
    if (kitchener.food_ != nullptr) {
        kitchener.passedTime_ -= kitchener.makingTime_;
        this->changeState(kitchener, KitchenerMaking::instance());
    }
}

void KitchenerMaking::update(Kitchener& kitchener, std::chrono::nanoseconds passedTime)
{
    assert(kitchener.food_ != nullptr);
    if (kitchener.prevStatus_ != KitchenerStatus::MAKING) {
        kitchener.food_->setStatus(FoodStatus::MAKING);
        switch (kitchener.getType()) {
        case KitchenerType::DOUGH:
            kitchener.makingTime_ = kitchener.food_->doughTime_;
            break;
        case KitchenerType::FILLING:
            kitchener.makingTime_ = kitchener.food_->getPreparationTime();
            if (kitchener.food_->getType() == FoodType::PIZZA) {
                kitchener.makingTime_ -= kitchener.food_->doughTime_;
            }
            break;
        case KitchenerType::PICKER:
            kitchener.makingTime_ = kitchener.food_->getCookingTime();
            break;
        default:
            assert(false);
            kitchener.makingTime_ = chrono::nanoseconds{ 0 };
            break;
        }
    }
    kitchener.prevStatus_ = KitchenerStatus::MAKING;
    kitchener.passedTime_ += passedTime;
    if (kitchener.passedTime_ >= kitchener.makingTime_) {
        switch (kitchener.getType()) {
        case KitchenerType::DOUGH:
            kitchener.ms_.kitchen().pushFrontQueueFilling(kitchener.food_);
            break;
        case KitchenerType::FILLING:
            kitchener.ms_.kitchen().pushFrontQueuePicker(kitchener.food_);
            break;
        case KitchenerType::PICKER:
            kitchener.food_->setStatus(FoodStatus::DONE);
            break;
        default:
            assert(false);
            break;
        }
        kitchener.food_ = nullptr;
        kitchener.passedTime_ -= kitchener.makingTime_;
        this->changeState(kitchener, KitchenerInaccessible::instance());
    }
}

} // namespace ds
