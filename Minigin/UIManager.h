#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include "Singleton.h"
#include "TextComponent.h"
#include "Observer.h"

namespace dae
{
    class UIManager : public Singleton<UIManager>, public Observer
    {
    public:
        using EventCallback = std::function<void(std::shared_ptr<Event>)>;

        void RegisterUIElement(const std::string& id, std::shared_ptr<TextComponent> textComponent,
            const std::string& format = "{value}", EventCallback callback = nullptr);

        void OnNotify(std::shared_ptr<Event> event) override;

    private:
        struct UIElement
        {
            std::shared_ptr<TextComponent> textComponent;
            std::string format;
            EventCallback callback;
        };

        std::unordered_map<std::string, UIElement> m_UIElements;
    };
}
