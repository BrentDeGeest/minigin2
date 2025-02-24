#pragma once
#include "Component.h"
#include "Transform.h"
#include "Time.h"

namespace dae
{
    class RotateComponent final : public Component
    {
    public:
        explicit RotateComponent(float radius, float speed, std::shared_ptr<GameObject> centerObject = nullptr);

		void OnStart() {};
        void Update() override;

    private:
        // Degrees per second
        std::weak_ptr<GameObject> m_CenterObject; // Object to orbit around
        float m_OrbitRadius;
        float m_OrbitSpeed; // Degrees per second
        float m_CurrentAngle = 0.0f; // Current angle in degrees

        // Inherited via Component
        void FixedUpdate() {};
        void Render() const {};
    };
}
