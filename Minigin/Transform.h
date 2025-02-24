#pragma once
#include <glm.hpp>
#include "Component.h"


namespace dae
{
	class Transform : public Component
	{
	public:
		void OnStart() {}

		//const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		glm::vec3 GetWorldPosition();

		void SetRotation(float angle);
		float GetRotation() const { return m_Rotation; }

		void MarkDirty(); // Marks the transform as needing an update
		bool IsDirty() const { return m_IsDirty; }
	private:
		glm::vec3 m_position;
		glm::vec3 m_LocalPosition{ 0.0f, 0.0f, 0.0f }; // Local position relative to parent
		glm::vec3 m_WorldPosition{ 0.0f, 0.0f, 0.0f }; // Cached global position
		float m_Rotation = 0.0f; // Rotation in degrees
		bool m_IsDirty = true; // Marks if transform needs recalculating

		// Inherited via Component
		void Update() override {};
		void FixedUpdate() override {};
		void Render() const override {};
	};
}
