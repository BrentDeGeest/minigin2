#pragma once
#include <SDL.h>
#include <chrono>

class Time
{
public:
    static void Init();
    static void Update();

    static float GetDeltaTime() { return m_DeltaTime; }
    static float GetTotalTime() { return m_TotalTime; }
    static float GetFixedDeltaTime() { return m_FixedTimeStep; }

    static bool ShouldUpdateFixed(); // Check if we should run a fixed update
    static void ConsumeFixedStep();  // Consume a fixed step after updating physics

    static void Sleep();

private:
    static std::chrono::high_resolution_clock::time_point m_LastTime;
    static float m_DeltaTime;
    static float m_TotalTime;
    static float m_FixedTimeStep;
    static float m_Lag; // Accumulates time for fixed updates
    static constexpr int m_DesiredFPS = 60; // Target frame rate
};
