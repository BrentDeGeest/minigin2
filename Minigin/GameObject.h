#pragma once
#include <memory>
#include "Transform.h"
#include "Component.h"
#include <vector>

namespace dae
{
	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render() const;

		void SetPosition(float x, float y, float z = 0.0f);
		glm::vec3 GetWorldPosition() const;

		template <typename T, typename... Args>
		T* AddComponent(Args&&... args);

		template <typename T>
		T* GetComponent() const;

		template <typename T>
		void MarkComponentForRemoval(); // Marks a component for deletion

		void CleanupComponents(); // Called at the end of the update loop

		// Parent-Child System
		void AddChild(std::shared_ptr<GameObject> child);
		void RemoveChild(std::shared_ptr<GameObject> child);
		void DetachFromParent();

		const std::vector<std::shared_ptr<GameObject>>& GetChildren() const { return m_Children; }
		std::shared_ptr<GameObject> GetParent() const { return m_Parent.lock(); }

		GameObject() ;
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<std::unique_ptr<Component>> m_components{};
		std::weak_ptr<GameObject> m_Parent; // Pointer to parent
		std::vector<std::shared_ptr<GameObject>> m_Children; // Children list
	};

	// AddComponent: Creates and attaches a component to the GameObject
	template <typename T, typename... Args>
	T* dae::GameObject::AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

		auto component = std::make_unique<T>(std::forward<Args>(args)...);
		component->SetOwner(this);
		T* rawPtr = component.get();
		m_components.emplace_back(std::move(component));

		if constexpr (std::is_member_function_pointer_v<decltype(&T::OnStart)>)
		{
			rawPtr->OnStart();
		}

		return rawPtr;
	}

	// GetComponent: Retrieves the first component of a specific type
	template <typename T>
	T* dae::GameObject::GetComponent() const
	{
		for (const auto& component : m_components)
		{
			if (auto castedComponent = dynamic_cast<T*>(component.get()))
			{
				return castedComponent;
			}
		}
		return nullptr;
	}

	// MarkComponentForRemoval: Flags a specific component type for deletion
	template <typename T>
	void GameObject::MarkComponentForRemoval()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

		for (auto& component : m_components)
		{
			if (dynamic_cast<T*>(component.get()))
			{
				component->MarkForDeletion();
				return; // Only mark the first found instance
			}
		}
	}

	// CleanupComponents: Actually removes flagged components at the end of the update loop
	inline void GameObject::CleanupComponents()
	{
		m_components.erase(
			std::remove_if(m_components.begin(), m_components.end(),
				[](const std::unique_ptr<Component>& component) {
					return component->IsMarkedForDeletion();
				}),
			m_components.end());
	}

}
