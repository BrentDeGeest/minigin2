#include "UIManager.h"
#include "EventQueue.h"
#include "GameEvent.h"

namespace dae
{
    UIManager& UIManager::GetInstance()
    {
        static UIManager instance;
        return instance;
    }

    void UIManager::RegisterUIElement(const std::string& key, std::shared_ptr<TextComponent> textComponent)
    {
        m_UIElements[key] = textComponent;
    }

	
    //void UIManager::OnNotify(std::shared_ptr<Event> event)
    //{
    //    auto gameEvent = std::dynamic_pointer_cast<GameEvent>(event);
    //    if (!gameEvent) return;
    //
    //    int playerId = gameEvent->GetPlayerId();
    //    std::string key;
    //
    //    switch (gameEvent->GetType())
    //    {
    //    case GameEventType::ScoreChanged:
    //        key = "Player" + std::to_string(playerId) + "_Score";
    //        break;
    //
    //    case GameEventType::HealthChanged:
    //        key = "Player" + std::to_string(playerId) + "_Lives";
    //        break;
    //    }
    //
    //    if (!key.empty() && m_UIElements.find(key) != m_UIElements.end())
    //    {
    //        m_UIElements[key]->SetText(std::to_string(gameEvent->GetValue()));
    //    }
    //}
    

    void UIManager::OnNotify(std::shared_ptr<Event> event) 
    {
        auto gameEvent = std::dynamic_pointer_cast<GameEvent>(event);
        if (!gameEvent) return;

        std::string key = gameEvent->GetEventTypeAsString() + "_" + std::to_string(gameEvent->GetPlayerId());

        if (m_UIElements.find(key) != m_UIElements.end())
        {
            m_UIElements[key]->SetText(gameEvent->GetEventTypeAsString() + ": " + std::to_string(gameEvent->GetValue()));
        }
    }
}
