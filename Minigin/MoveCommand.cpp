#include "MoveCommand.h"
#include <iostream>
#include "Time.h"



dae::MoveCommand::MoveCommand(GameObject* object, float xDir, float yDir, float speed)
	: m_pObject(object), m_XDir(xDir), m_YDir(yDir), m_Speed(speed)
{
}

void dae::MoveCommand::Execute()
{
    if (m_pObject)
    {
        auto transform = m_pObject->GetComponent<Transform>();
        if (transform)
        {
            glm::vec3 pos = transform->GetLocalPosition();

            // Create movement direction vector
            glm::vec2 direction(m_XDir, m_YDir);

            // Normalize direction to prevent diagonal speed boost
            if (glm::length(direction) > 0.0f)
            {
                direction = glm::normalize(direction);
            }

            // Apply movement with deltaTime for frame-independent speed
            float deltaTime = Time::GetDeltaTime();
            transform->SetPosition(pos.x + direction.x * m_Speed * deltaTime,
                pos.y + direction.y * m_Speed * deltaTime,
                pos.z);
        }
    }
}
