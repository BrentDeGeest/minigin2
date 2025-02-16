#pragma once
#include <memory>
#include "Transform.h"
#include "Component.h"
#include <vector>

namespace dae
{
	class Texture2D;

	// todo: this should become final.
	class GameObject final
	{
	public:
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render() const;

		//void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		template <typename T, typename... Args>
		T* AddComponent(Args&&... args);

		template <typename T>
		T* GetComponent() const;

		template <typename T>
		void RemoveComponent();

		GameObject() ;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<std::unique_ptr<Component>> m_components{};
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

	// RemoveComponent: Deletes a specific component type from the GameObject
	template <typename T>
	void GameObject::RemoveComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "T must be a Component");

		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			if (dynamic_cast<T*>(it->get()))
			{
				m_components.erase(it);
				return; // Only remove the first found instance
			}
		}
	}
}
