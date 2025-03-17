#pragma once
#include "Observer.h"
#include <iostream>

namespace dae
{
    class AchievementSystem : public Observer
    {
    public:
        void OnNotify(std::shared_ptr<Event> event) override;
    };
}
