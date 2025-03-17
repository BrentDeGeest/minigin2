#include "HealthComponent.h"
#include "Event.h"
#include "Observer.h"
#include "EventQueue.h"
#include "PlayerDiedEvent.h"
#include "GameObject.h"
#include "GameEvent.h"

namespace dae
{
    HealthComponent::HealthComponent(int lives, bool isPlayer) : m_Lives(lives), m_IsPlayer(isPlayer) {}

    void HealthComponent::TakeDamage()
    {
        if (m_Lives <= 0) return; // Avoid triggering logic if already at 0 lives

        m_Lives--;

        if (m_pOwner)
        {
            //NotifyObservers(std::make_shared<GameEvent>(GameEventType::HealthChanged, m_pOwner->GetId(), m_Lives));
            EventQueue::GetInstance().PushEvent(
                std::make_shared<GameEvent>(GameEventType::HealthChanged, m_pOwner->GetId(), m_Lives)
            );
        }

        // If health reaches 0, notify the game
        if (m_pOwner && m_Lives <= 0 && m_IsPlayer)
        {
            EventQueue::GetInstance().PushEvent(
                std::make_shared<GameEvent>(GameEventType::PlayerDied, m_pOwner->GetId()));
        }
    }

    int HealthComponent::GetLives() const { return m_Lives; }

    void HealthComponent::SetOnDeathCallback(const std::function<void()>& callback)
    {
        m_OnDeathCallback = callback;
    }

    void HealthComponent::AddObserver(std::shared_ptr<Observer> observer)
    {
        m_Observers.push_back(std::move(observer)); 
    }

    void HealthComponent::RemoveObserver(std::shared_ptr<Observer> observer)
    {
        m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
    }

    void HealthComponent::NotifyObservers(std::shared_ptr<Event> event)
    {
        for (const auto& observer : m_Observers)
        {
            observer->OnNotify(event);
        }
    }
    void HealthComponent::Update(){}
    void HealthComponent::FixedUpdate(){}
    void HealthComponent::Render() const{}
}
