
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
        kitchener.passedTime_ = chrono::seconds{ 0 };
        kitchener.makingTime_ = chrono::seconds{
            cmn::getRandomNumber(Options::minPauseTime_, Options::maxPauseTime_)
        };
    }
    kitchener.prevStatus_ = KitchenerStatus::INACCESSIBLE;
    kitchener.passedTime_ += passedTime;
    if (kitchener.passedTime_ >= kitchener.makingTime_) {
        this->changeState(kitchener, KitchenerWaiting::instance());
    }
}

void KitchenerWaiting::update(Kitchener& kitchener, std::chrono::nanoseconds passedTime)
{
    if (kitchener.prevStatus_ != KitchenerStatus::WAITING_FOR_NEXT) {
        kitchener.passedTime_ = chrono::nanoseconds{ 0 };
    }
    kitchener.prevStatus_ = KitchenerStatus::WAITING_FOR_NEXT;
    kitchener.passedTime_ += passedTime;
}

void KitchenerMaking::update(Kitchener& kitchener, std::chrono::nanoseconds passedTime)
{
    if (kitchener.prevStatus_ != KitchenerStatus::MAKING) {
        kitchener.passedTime_ = chrono::nanoseconds{ 0 };
        assert(kitchener.food_ != nullptr);
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

        int random{ cmn::getRandomNumber(1, 100) };
        if (random >= 1 && random <= Options::pauseChance_) {
            this->changeState(kitchener, KitchenerInaccessible::instance());
        }
        else {
            this->changeState(kitchener, KitchenerWaiting::instance());
        }
    }
}

} // namespace ds
