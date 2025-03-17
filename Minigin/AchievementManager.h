#pragma once
#include "Singleton.h"
#include "Event.h"
#include <steam_api.h>


namespace dae
{
    class AchievementManager final : public Singleton<AchievementManager>
    {
    public:
        void Initialize(); // Call this when loading the game
        void OnNotify(std::shared_ptr<Event> event); // Handles achievement logic
    };
}
