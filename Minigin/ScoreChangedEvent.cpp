#include "ScoreChangedEvent.h"

namespace dae
{
    ScoreChangedEvent::ScoreChangedEvent(int playerId, int newScore)
        : m_PlayerId(playerId), m_NewScore(newScore) {
    }

    int ScoreChangedEvent::GetPlayerId() const { return m_PlayerId; }
    int ScoreChangedEvent::GetNewScore() const { return m_NewScore; }
    std::string ScoreChangedEvent::GetEventName() const { return "ScoreChanged"; }
}
