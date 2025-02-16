#pragma once
#include "Component.h"
#include <memory>
#include "TextComponent.h"
#include "Time.h"

namespace dae
{
    class FPSComponent final : public Component
    {
    public:
        explicit FPSComponent(std::shared_ptr<Font> font);
		void OnStart();
        void Update() override;

    private:
        TextComponent* m_TextComponent = nullptr;
        float m_ElapsedTime = 0.0f;
        int m_FrameCount = 0;
		std::shared_ptr<Font> m_Font;

        // Inherited via Component
        void FixedUpdate() override {};
        void Render() const override {};
    };
}
