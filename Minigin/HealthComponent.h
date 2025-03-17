#pragma once
#include "Component.h"
#include <functional>
#include <vector>
#include <memory>

namespace dae
{
    class Observer;
    class Event;

    class HealthComponent : public Component
    {
    public:
        explicit HealthComponent(int lives, bool isPlayer = false);
        void OnStart() {}

        void TakeDamage();
        int GetLives() const;

        void AddObserver(std::shared_ptr<Observer> observer);
        void RemoveObserver(std::shared_ptr<Observer> observer);

        void SetOnDeathCallback(const std::function<void()>& callback); 

    private:
        int m_Lives;
		bool m_IsPlayer; // Flag to check if this is a player
        std::function<void()> m_OnDeathCallback; // Function that executes when HP reaches 0
        std::vector<std::shared_ptr<Observer>> m_Observers; // Track observers for UI updates

        void NotifyObservers(std::shared_ptr<Event> event);

        // Inherited via Component
        void Update() override;
        void FixedUpdate() override;
        void Render() const override;
    };
}
