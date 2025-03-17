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
		int GetId() const { return m_ID; }

		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render() const;

		void SetPosition(float x, float y, float z = 0.0f);
		glm::vec3 GetWorldPosition() const;

		template <typename T, typename... Args>
		//T* AddComponent(Args&&... args);
		std::shared_ptr<T> AddComponent(Args&&... args);

		template <typename T>
		//T* GetComponent() const;
		std::shared_ptr<T> GetComponent() const;

		template <typename T>
		void MarkComponentForRemoval(); // Marks a component for deletion

		void CleanupComponents(); // Called at the end of the update loop

		// Parent-Child System
		void SetParent(std::shared_ptr<GameObject> newParent);

		const std::vector<std::shared_ptr<GameObject>>& GetChildren() const { return m_Children; }
		std::shared_ptr<GameObject> GetParent() const { return m_Parent.lock(); }

		GameObject() ;
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<std::shared_ptr<Component>> m_components{};
		//std::vector<std::unique_ptr<Component>> m_components{};
		std::weak_ptr<GameObject> m_Parent; // Pointer to parent
		std::vector<std::shared_ptr<GameObject>> m_Children; // Children list

		static int s_NextID;
		int m_ID;

	};

	// AddComponent: Creates and attaches a component to the GameObject
	template <typename T, typename... Args>
	std::shared_ptr<T> dae::GameObject::AddComponent(Args&&... args)
		//T* dae::GameObject::AddComponent(Args&&... args)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

		//auto component = std::make_unique<T>(std::forward<Args>(args)...);
		auto component = std::make_shared<T>(std::forward<Args>(args)...);
		component->SetOwner(this);
		//T* rawPtr = component.get();
		//m_components.emplace_back(std::move(component));
		m_components.push_back(component);

		if constexpr (std::is_member_function_pointer_v<decltype(&T::OnStart)>)
		{
			//rawPtr->OnStart();
			component->OnStart();
		}

		//return rawPtr;
		return component;
	}

	// GetComponent: Retrieves the first component of a specific type
	/*template <typename T>
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
	}*/

	template <typename T>
	std::shared_ptr<T> dae::GameObject::GetComponent() const
	{
		for (const auto& component : m_components)
		{
			if (auto castedComponent = std::dynamic_pointer_cast<T>(component)) // Use dynamic_pointer_cast
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
	//inline void GameObject::CleanupComponents()
	//{
	//	m_components.erase(
	//		std::remove_if(m_components.begin(), m_components.end(),
	//			[](const std::unique_ptr<Component>& component) {
	//				return component->IsMarkedForDeletion();
	//			}),
	//		m_components.end());
	//}

	inline void GameObject::CleanupComponents()
	{
		m_components.erase(
			std::remove_if(m_components.begin(), m_components.end(),
				[](const std::shared_ptr<Component>& component) { // Now uses shared_ptr
					return component->IsMarkedForDeletion();
				}),
			m_components.end());
	}

}
