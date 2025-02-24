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
	// First, detach all children explicitly
	while (!m_Children.empty())
	{
		auto child = m_Children.back();
		m_Children.pop_back();
		child->m_Parent.reset(); // Reset parent reference
	}

	// Clear components before removing from the parent's list
	m_components.clear();

	// Avoid calling shared_from_this() in the destructor
	if (!m_Parent.expired()) // Check if parent still exists
	{
		if (auto parent = m_Parent.lock()) // Convert weak_ptr to shared_ptr safely
		{
			parent->RemoveChild(this->shared_from_this()); // Remove from parent's list
		}
	}

	m_Parent.reset(); // ? Reset parent reference
}

void dae::GameObject::AddChild(std::shared_ptr<GameObject> child)
{
	if (!child || child.get() == this || child->GetParent() == shared_from_this())
		return; // Prevent self-parenting or redundant additions

	// If the child has a previous parent, detach it
	if (auto oldParent = child->GetParent())
	{
		oldParent->RemoveChild(child);
	}

	// Set the parent-child relationship
	child->m_Parent = shared_from_this();
	m_Children.push_back(child);
}

void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> child)
{
	if (!child)
		return;

	auto it = std::remove(m_Children.begin(), m_Children.end(), child);
	if (it != m_Children.end())
	{
		child->m_Parent.reset(); 
		m_Children.erase(it, m_Children.end());
	}
}

void dae::GameObject::DetachFromParent()
{
	if (auto parent = m_Parent.lock()) 
	{
		parent->RemoveChild(shared_from_this());
		m_Parent.reset();
	}
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

