#pragma once
#include "Command.h"
#include "GameObject.h"
#include "EventQueue.h"
#include "GameEvent.h"

namespace dae
{
    class GainScoreCommand final : public Command
    {
    public:
        GainScoreCommand(GameObject* player, int scoreAmount)
            : m_Player(player), m_ScoreAmount(scoreAmount) {
        }

        void Execute() override
        {
            if (!m_Player) return;

            // Send an event for score change
            EventQueue::GetInstance().PushEvent(
                std::make_shared<GameEvent>(GameEventType::ScoreChanged, m_Player->GetId(), m_ScoreAmount)
            );
        }

    private:
        GameObject* m_Player;
        int m_ScoreAmount;
    };
}
