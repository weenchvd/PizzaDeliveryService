
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#ifndef KITCHENER_HPP
#define KITCHENER_HPP

#include"food.hpp"
#include"worker.hpp"
#include<assert.h>
#include<chrono>
#include<string>

namespace ds {

enum class KitchenerType : char {
    __INVALID = -1,                 /// invalid, must be the first
    // vvv TYPES vvv
    DOUGH,
    FILLING,
    PICKER,
    // ^^^ TYPES ^^^
    __NUMBER_OF,
    __END                           /// must be the last
};

enum class KitchenerStatus : char {
    __INVALID = -1,                 /// invalid, must be the first
    // vvv STATUSES vvv
    INACCESSIBLE,
    WAITING_FOR_NEXT,
    MAKING,
    // ^^^ STATUSES ^^^
    __NUMBER_OF,
    __END                           /// must be the last
};

std::string toString(KitchenerType value);

std::string toString(KitchenerStatus value);

///************************************************************************************************

class ManagmentSystem;

class Kitchener {
public:
    friend class KitchenerState;
    friend class KitchenerInaccessible;
    friend class KitchenerWaiting;
    friend class KitchenerMaking;

public:
    Kitchener(ManagmentSystem& ms, WorkerID workerID, KitchenerType type);

    Kitchener(const Kitchener&) = delete;
    Kitchener& operator=(const Kitchener&) = delete;

    virtual ~Kitchener() noexcept {}

public:
    virtual void update(std::chrono::nanoseconds passedTime);

    virtual KitchenerStatus getStatus() const noexcept;

    WorkerID getID() const noexcept { return id_; }

    KitchenerType getType() const noexcept { return type_; }

    Food* getFood() const noexcept { return food_; }

    void makeFood(Food* food);

private:
    void changeState(KitchenerState& state) { state_ = &state; }

private:
    KitchenerState*                             state_;
    ManagmentSystem&                            ms_;
    Food*                                       food_;
    std::chrono::nanoseconds                    passedTime_;
    std::chrono::nanoseconds                    makingTime_;
    const WorkerID                              id_;
    const KitchenerType                         type_;
    KitchenerStatus                             prevStatus_;
};

///************************************************************************************************

class KitchenerState {
protected:
    KitchenerState() noexcept {}

public:
    virtual void update(Kitchener& kitchener, std::chrono::nanoseconds passedTime);

    virtual KitchenerStatus getStatus() const noexcept { return KitchenerStatus::__INVALID; }

protected:
    void changeState(Kitchener& kitchener, KitchenerState& state) { kitchener.changeState(state); }
};


class KitchenerInaccessible : public KitchenerState {
protected:
    KitchenerInaccessible() noexcept {}

public:
    static KitchenerInaccessible& instance() noexcept { return state_; }

public:
    virtual void update(Kitchener& kitchener, std::chrono::nanoseconds passedTime);

    virtual KitchenerStatus getStatus() const noexcept { return KitchenerStatus::INACCESSIBLE; }

private:
    static KitchenerInaccessible state_;
};


class KitchenerWaiting : public KitchenerState {
protected:
    KitchenerWaiting() noexcept {}

public:
    static KitchenerWaiting& instance() noexcept { return state_; }

public:
    virtual void update(Kitchener& kitchener, std::chrono::nanoseconds passedTime);

    virtual KitchenerStatus getStatus() const noexcept { return KitchenerStatus::WAITING_FOR_NEXT; }

private:
    static KitchenerWaiting state_;
};


class KitchenerMaking : public KitchenerState {
protected:
    KitchenerMaking() noexcept {}

public:
    static KitchenerMaking& instance() noexcept { return state_; }

public:
    virtual void update(Kitchener& kitchener, std::chrono::nanoseconds passedTime);

    virtual KitchenerStatus getStatus() const noexcept { return KitchenerStatus::MAKING; }

private:
    static KitchenerMaking state_;
};

///************************************************************************************************

inline void Kitchener::update(std::chrono::nanoseconds passedTime)
{
    assert(state_);
    return state_->update(*this, passedTime);
}

inline KitchenerStatus Kitchener::getStatus() const noexcept
{
    assert(state_);
    return state_->getStatus();
}

inline void Kitchener::makeFood(Food* food)
{
    assert(this->getStatus() == KitchenerStatus::WAITING_FOR_NEXT);
    food_ = food;
}

inline void KitchenerState::update(Kitchener& kitchener, std::chrono::nanoseconds passedTime)
{
    assert(false);
}

} // namespace ds

#endif // !KITCHENER_HPP
