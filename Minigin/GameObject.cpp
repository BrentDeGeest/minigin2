#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

dae::GameObject::GameObject()
{
	AddComponent<Transform>(); 
}

dae::GameObject::~GameObject()
{
	// Detach all children before destruction
	while (!m_Children.empty())
	{
		m_Children.back()->SetParent(nullptr); // ? Properly remove child from hierarchy
		m_Children.pop_back();
	}
	m_Children.clear(); // Remove all child references

	// Clear components before removing from the parent's list
	m_components.clear();

	// Remove from parent safely without `shared_from_this()`
	if (auto parent = m_Parent.lock())
	{
		auto& siblings = parent->m_Children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), shared_from_this()), siblings.end());
	}

	m_Parent.reset(); // Clear weak_ptr reference
}

void dae::GameObject::SetParent(std::shared_ptr<GameObject> newParent)
{
	// Prevent self-parenting
	if (newParent.get() == this) return;

	// If we already have a parent, remove ourselves from its children
	if (auto oldParent = m_Parent.lock())
	{
		auto& siblings = oldParent->m_Children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), shared_from_this()), siblings.end());
	}

	// If newParent is nullptr, we are just detaching
	if (!newParent)
	{
		m_Parent.reset();
		return;
	}

	// Set the new parent and add ourselves as its child
	m_Parent = newParent;
	newParent->m_Children.push_back(shared_from_this());
}

void dae::GameObject::Update() 
{
	for (const auto& component : m_components)
	{
		component->Update();
	}

	// Update all children
	for (auto& child : m_Children)
	{
		child->Update();
	}

	CleanupComponents();
}
void dae::GameObject::FixedUpdate()
{
	for (const auto& component : m_components)
	{
		component->FixedUpdate();
	}

	// Update all children
	for (auto& child : m_Children)
	{
		child->FixedUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_components)
	{
		component->Render();
	}

	// Render all children
	for (const auto& child : m_Children)
	{
		child->Render();
	}
}

void dae::GameObject::SetPosition(float x, float y, float z) 
{
	auto transform = GetComponent<Transform>();
	if (transform)
	{
		transform->SetPosition(x, y, z);
	}
}

glm::vec3 dae::GameObject::GetWorldPosition() const
{
	auto transform = GetComponent<Transform>();
	return transform ? transform->GetWorldPosition() : glm::vec3{};
}

