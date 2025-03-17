#pragma once
#include "Command.h"
#include "GameObject.h"
#include "HealthComponent.h"

namespace dae
{
    class TakeDamageCommand final : public Command
    {
    public:
        explicit TakeDamageCommand(GameObject* player)
            : m_Player(player) {
        }

        void Execute() override
        {
            if (!m_Player) return;

            auto health = m_Player->GetComponent<HealthComponent>();
            if (health)
            {
                health->TakeDamage();
            }
        }

    private:
        GameObject* m_Player;
    };
}
