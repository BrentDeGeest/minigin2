#include "Time.h"
#include <chrono>

std::chrono::high_resolution_clock::time_point Time::m_LastTime;
float Time::m_DeltaTime = 0.0f;
float Time::m_TotalTime = 0.0f;
float Time::m_FixedTimeStep = 1.0f / 60.0f; // 60 FPS physics update
float Time::m_Lag = 0.0f;

void Time::Init()
{
    m_LastTime = std::chrono::high_resolution_clock::now();
}

void Time::Update()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> deltaTime = currentTime - m_LastTime;

    m_DeltaTime = deltaTime.count(); // Convert to seconds
    m_TotalTime += m_DeltaTime;
    m_Lag += m_DeltaTime; // Accumulate time for fixed updates

    m_LastTime = currentTime;
}

// Check if we have accumulated enough time to process a fixed update
bool Time::ShouldUpdateFixed()
{
    return m_Lag >= m_FixedTimeStep;
}

// Consume the fixed timestep when processing a physics update
void Time::ConsumeFixedStep()
{
    m_Lag -= m_FixedTimeStep;
}

