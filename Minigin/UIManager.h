#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "Observer.h"
#include "TextComponent.h"

namespace dae
{
    class UIManager final : public Observer
    {
    public:
        static UIManager& GetInstance();

        // Generic function to register any UI element
        void RegisterUIElement(const std::string& key, std::shared_ptr<TextComponent> textComponent);

        // Observer pattern - listens to game events
        void OnNotify(std::shared_ptr<Event> event) override;

    private:
        UIManager() = default;
        std::unordered_map<std::string, std::shared_ptr<TextComponent>> m_UIElements;
    };
}
