#include "SpriteComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include <iostream>

dae::SpriteComponent::SpriteComponent(const std::string& texturePath)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(texturePath);
}

void dae::SpriteComponent::Render() const
{
	if (!m_pOwner) return; // Ensure the component has an owner

	Transform* transform = m_pOwner->GetComponent<Transform>();
	if (!transform) return;
	//const auto& pos = transform->GetLocalPosition();
	const auto& pos = transform->GetWorldPosition();
	//std::cout << "SpriteComponent::Render() | Position: " << pos.x << ", " << pos.y << std::endl;
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);

}


