#pragma once
#include "Component.h"
#include <memory>
#include <string>
#include "Texture2D.h"

namespace dae
{

	class SpriteComponent : public Component
	{
	public:
		SpriteComponent(const std::string& texturePath);
		void OnStart() {}
		void Render() const;

	private:
		std::shared_ptr<Texture2D> m_Texture;


		// Inherited via Component
		void Update() override {};
		void FixedUpdate() override {};

	};

}