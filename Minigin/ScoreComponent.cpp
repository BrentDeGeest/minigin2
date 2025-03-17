#include "ScoreComponent.h"
#include "GameEvent.h"
#include "EventQueue.h"
#include "GameObject.h"



void dae::ScoreComponent::AddScore(int amount)
{
	m_Score += amount;
	// Notify event queue that the score has changed
	if (m_pOwner)
	{
		EventQueue::GetInstance().PushEvent(
			std::make_shared<GameEvent>(GameEventType::ScoreChanged, m_pOwner->GetId(), m_Score));
	}
}

void dae::ScoreComponent::Update()
{
}

void dae::ScoreComponent::FixedUpdate()
{
}

void dae::ScoreComponent::Render() const
{
}
