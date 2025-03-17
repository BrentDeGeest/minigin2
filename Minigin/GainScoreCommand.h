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
        GainScoreCommand(GameObject* player, int scoreIncrease)
            : m_pPlayer(player), m_ScoreIncrease(scoreIncrease) {
        }

        void Execute() override;
       

    private:
        GameObject* m_pPlayer;
        int m_ScoreIncrease;
    };
}
