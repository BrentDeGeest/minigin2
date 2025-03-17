#pragma once
#include "Event.h"

namespace dae
{
    enum class GameEventType
    {
        PlayerDied,
        ScoreChanged,
        HealthChanged,
        // Add more events here...
    };

    class GameEvent : public Event
    {
    public:
        GameEvent(GameEventType type, int playerId, int value = 0)
            : m_Type(type), m_EntityId(playerId), m_Value(value) {
        }
        
        std::string GetEventName() const override
        {
            return "GameEvent";
        }

        //std::string GetEventName() const override
        //{
        //    switch (m_Type)
        //    {
        //    case GameEventType::PlayerDied: return "PlayerDied";
        //    case GameEventType::ScoreChanged: return "ScoreChanged";
        //    case GameEventType::HealthChanged: return "HealthChanged";
        //    default: return "UnknownEvent";
        //    }
        //}

        GameEventType GetType() const { return m_Type; }
        int GetEntityId() const { return m_EntityId; }
        int GetValue() const { return m_Value; } // Useful for score, health, etc.

        std::string GetEventTypeAsString() const
        {
            switch (m_Type)
            {
            case GameEventType::ScoreChanged: return "Score";
            case GameEventType::HealthChanged: return "Lives";
            case GameEventType::PlayerDied: return "PlayerDied";
            default: return "Unknown";
            }
        }

    private:
        GameEventType m_Type;
        int m_EntityId;
        int m_Value; // Can store score, health change, etc.
    };
}
