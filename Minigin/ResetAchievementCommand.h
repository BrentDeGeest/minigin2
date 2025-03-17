#pragma once
#include "Command.h"
#include "steam_api.h"
#include <string>

namespace dae
{
    class ResetAchievementCommand final : public Command
    {
    public:
        explicit ResetAchievementCommand(const std::string& achievementName)
            : m_AchievementName(achievementName) {
        }

        void Execute() override;

    private:
        std::string m_AchievementName;
    };
}
