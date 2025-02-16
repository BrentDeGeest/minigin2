#include "SpriteComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"

dae::SpriteComponent::SpriteComponent(const std::string& texturePath)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(texturePath);
}

void dae::SpriteComponent::Render() const
{
	if (!m_pOwner) return; // Ensure the component has an owner

	Transform* transform = m_pOwner->GetComponent<Transform>();
	if (transform)
	{
		const auto& pos = transform->GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}

}


