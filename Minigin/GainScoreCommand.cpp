#include "GainScoreCommand.h"
#include "ScoreComponent.h"
#include "EventQueue.h"
#include "GameEvent.h"

void dae::GainScoreCommand::Execute()
{
    if (m_pPlayer)
    {
        auto scoreComponent = m_pPlayer->GetComponent<ScoreComponent>();
        if (scoreComponent)
        {
            scoreComponent->AddScore(m_ScoreIncrease); 
        }
    }
}
