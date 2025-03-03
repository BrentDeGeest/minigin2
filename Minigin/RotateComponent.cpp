#include "RotateComponent.h"
#include "GameObject.h"
#include <iostream>

dae::RotateComponent::RotateComponent(float radius, float speed, std::shared_ptr<GameObject> centerObject)
    : m_OrbitRadius(radius), m_OrbitSpeed(speed), m_CenterObject(centerObject)
{
}

void dae::RotateComponent::Update()
{
    // If no center is set, try to use the parent
    if (m_CenterObject.expired())
    {
        if (m_pOwner && m_pOwner->GetParent())
        {
            m_CenterObject = m_pOwner->GetParent(); // Use parent as center
        }
    }

    // Convert weak_ptr to shared_ptr safely
    auto centerObj = m_CenterObject.lock();
    glm::vec3 centerPosition{ 0.0f, 0.0f, 0.0f }; // Default center at (0,0)

    if (centerObj) // If a valid center exists, use its position
    {
        auto* centerTransform = centerObj->GetComponent<Transform>();
        if (centerTransform)
        {
            centerPosition = centerTransform->GetWorldPosition();
        }
    }

    auto* selfTransform = m_pOwner->GetComponent<Transform>();
    if (!selfTransform) return;
    auto worldPos = selfTransform->GetWorldPosition();
   // std::cout << "Object: " << m_pOwner << " | World Pos: "
     //   << worldPos.x << ", " << worldPos.y << std::endl;

    // Update the angle
    m_CurrentAngle += m_OrbitSpeed * Time::GetDeltaTime();
    if (m_CurrentAngle >= 360.0f)
        m_CurrentAngle -= 360.0f;

    // Convert degrees to radians
    float radians = glm::radians(m_CurrentAngle);

    glm::vec3 localOffset = {
        m_OrbitRadius * cos(radians),
        m_OrbitRadius * sin(radians),
        0.0f
    };

    glm::vec3 finalPosition = centerPosition + localOffset;

    selfTransform->SetPosition(finalPosition.x, finalPosition.y, finalPosition.z);

}
