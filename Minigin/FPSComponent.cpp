#include "FPSComponent.h"
#include "GameObject.h"
#include "Transform.h"

dae::FPSComponent::FPSComponent(std::shared_ptr<Font> font)
	: m_Font(std::move(font))
{
}

void dae::FPSComponent::OnStart()
{
    // Now m_pOwner is set, so we can safely add the TextComponent
    m_TextComponent = m_pOwner->AddComponent<TextComponent>("FPS: 0", m_Font);
}

void dae::FPSComponent::Update()
{
    m_ElapsedTime += Time::GetDeltaTime();
    m_FrameCount++;

    if (m_ElapsedTime >= 1.0f) // Every second, update the FPS display
    {
        float fps = static_cast<float>(m_FrameCount / m_ElapsedTime);

        // Format FPS with 1 decimal place
        std::stringstream stream;
        stream.precision(1);
        stream << std::fixed << fps;

        if (m_TextComponent) 
        {
            m_TextComponent->SetText("FPS: " + stream.str());
        }

        // Reset the counters
        m_ElapsedTime = 0.0f;
        m_FrameCount = 0;
    }
}


