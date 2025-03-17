#include "ResetAchievementCommand.h"
#include <iostream> // For debugging

namespace dae
{
    void ResetAchievementCommand::Execute()
    {
        if (!SteamUserStats())
        {
            std::cerr << "SteamUserStats not initialized!" << std::endl;
            return;
        }

        // Clear the achievement
        SteamUserStats()->ClearAchievement(m_AchievementName.c_str());
        SteamUserStats()->StoreStats(); // Ensure changes are applied

        std::cout << "Achievement " << m_AchievementName << " has been reset." << std::endl;
    }
}
