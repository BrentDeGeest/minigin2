#include <memory>
#include "AchievementManager.h"
#include "GameEvent.h"
#include "EventQueue.h"
#include <iostream> // For debugging


namespace dae
{
    void AchievementManager::Initialize()
    {
        // Register this manager as an event listener
        EventQueue::GetInstance().AddListener(
            [this](std::shared_ptr<Event> event)
            {
                this->OnNotify(event);
            });
    }

    void AchievementManager::OnNotify(std::shared_ptr<Event> event)
    {
       auto gameEvent = std::dynamic_pointer_cast<GameEvent>(event);
    if (!gameEvent) return;

    // Unlock "Winner" achievement when score reaches 500
    if (gameEvent->GetType() == GameEventType::ScoreChanged)
    {
        int newScore = gameEvent->GetValue();
        if (newScore >= 500)
        {
            std::cout << "Unlocking Steam Achievement: WINNER!" << std::endl;
            SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
            SteamUserStats()->StoreStats();
        }
    }

    // Unlock "Orbiter" achievement when a player dies
    if (gameEvent->GetType() == GameEventType::PlayerDied)
    {
        std::cout << "Entity " <<  gameEvent->GetEntityId() << " died..." << std::endl;
       
        std::cout << "Unlocking Steam Achievement: ORBITER!" << std::endl;
        SteamUserStats()->SetAchievement("ACH_TRAVEL_FAR_SINGLE");
        SteamUserStats()->StoreStats();
    }
    }
}
