#pragma once
#include "Event.h"

namespace dae
{
    class PlayerDiedEvent : public Event
    {
    public:
        explicit PlayerDiedEvent(int playerId);
		~PlayerDiedEvent() override = default;
        int GetPlayerId() const;
        std::string GetEventName() const override;

    private:
        int m_PlayerId;
    };
}
