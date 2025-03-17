#include "UIManager.h"

namespace dae
{
    void UIManager::RegisterUIElement(const std::string& id, std::shared_ptr<TextComponent> textComponent,
        const std::string& format, EventCallback callback)
    {
        m_UIElements[id] = { textComponent, format, callback };
    }

    void UIManager::OnNotify(std::shared_ptr<Event> event)
    {
        for (auto& [id, element] : m_UIElements)
        {
            if (element.callback)
            {
                element.callback(event); // Call the event handler for this UI element
            }
        }
    }
}
