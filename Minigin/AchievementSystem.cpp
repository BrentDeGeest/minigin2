#include "AchievementSystem.h"
#include "ScoreChangedEvent.h"
#include "GameEvent.h"
//#include "Steamworks.h" // Placeholder for Steam API

namespace dae
{
    void AchievementSystem::OnNotify(std::shared_ptr<Event> event)
    {
        auto gameEvent = std::dynamic_pointer_cast<GameEvent>(event);
        if (!gameEvent) return; // Ignore if not a GameEvent

        switch (gameEvent->GetType())
        {
        case GameEventType::PlayerDied:
            std::cout << "Player " << gameEvent->GetPlayerId() << " died!" << std::endl;
            break;

        case GameEventType::ScoreChanged:
            std::cout << "Player " << gameEvent->GetPlayerId() << " new score: " << gameEvent->GetValue() << std::endl;
            break;

        case GameEventType::HealthChanged:
            std::cout << "Player " << gameEvent->GetPlayerId() << " new health: " << gameEvent->GetValue() << std::endl;
            break;
        }
    }
}
