#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

dae::GameObject::GameObject()
{
	AddComponent<Transform>(); 
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update() 
{
	for (const auto& component : m_components)
	{
		component->Update();
	}

	CleanupComponents();
}
void dae::GameObject::FixedUpdate()
{
	for (const auto& component : m_components)
	{
		component->FixedUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_components)
	{
		component->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	if (auto transform = GetComponent<Transform>())
		transform->SetPosition(x, y, 0.0f);
}

