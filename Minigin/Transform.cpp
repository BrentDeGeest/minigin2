#include "Transform.h"
#include "GameObject.h"

//void dae::Transform::SetPosition(const float x, const float y, const float z)
//{
//	m_position.x = x;
//	m_position.y = y;
//	m_position.z = z;
//}


void dae::Transform::SetPosition(const float x, const float y, const float z)
{

    if (m_LocalPosition.x == x && m_LocalPosition.y == y && m_LocalPosition.z == z)
        return; // Avoid unnecessary updates if the position is unchanged

    m_LocalPosition = { x, y, z };
    MarkDirty(); // Mark this transform and its children as needing updates
}

glm::vec3 dae::Transform::GetWorldPosition()
{
    if (!m_IsDirty) // If not dirty, return cached world position
        return m_WorldPosition;

    if (m_pOwner && m_pOwner->GetParent()) // If there's a parent, calculate world position
    {
        auto parentTransform = m_pOwner->GetParent()->GetComponent<Transform>();
        if (parentTransform)
        {
            m_WorldPosition = parentTransform->GetWorldPosition() + m_LocalPosition;
        }
    }
    else
    {
        m_WorldPosition = m_LocalPosition;
    }

    m_IsDirty = false; // Mark as clean after updating
    return m_WorldPosition;
}

void dae::Transform::SetRotation(float angle)
{
    if (m_Rotation == angle) return;

    m_Rotation = angle;
    MarkDirty();
}

void dae::Transform::MarkDirty()
{
    if (m_IsDirty)
        return; // Already dirty, no need to propagate

    m_IsDirty = true;

    // Propagate to all children
    if (m_pOwner)
    {
        for (auto& child : m_pOwner->GetChildren())
        {
            auto childTransform = child->GetComponent<Transform>();
            if (childTransform)
            {
                childTransform->MarkDirty();
            }
        }
    }
}