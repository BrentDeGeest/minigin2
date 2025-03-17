#pragma once
#include "Event.h"

namespace dae
{
    class ScoreChangedEvent : public Event
    {
    public:
        explicit ScoreChangedEvent(int playerId, int newScore);
		~ScoreChangedEvent() override = default;
        int GetPlayerId() const;
        int GetNewScore() const;
        std::string GetEventName() const override;

    private:
        int m_PlayerId;
        int m_NewScore;
    };
}
