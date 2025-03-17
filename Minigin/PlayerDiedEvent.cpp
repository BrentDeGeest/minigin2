#include "PlayerDiedEvent.h"

namespace dae
{
    PlayerDiedEvent::PlayerDiedEvent(int playerId) : m_PlayerId(playerId) {}

    int PlayerDiedEvent::GetPlayerId() const { return m_PlayerId; }
    std::string PlayerDiedEvent::GetEventName() const { return "PlayerDied"; }
}
