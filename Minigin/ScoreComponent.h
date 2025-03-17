#pragma once
#include "Component.h"
#include "GameEvent.h"
#include "EventQueue.h"

namespace dae
{
    class ScoreComponent final : public Component
    {
    public:
        ScoreComponent() = default;
		void OnStart() {}  

        void AddScore(int amount);
        

        int GetScore() const { return m_Score; }
        void ResetScore() { m_Score = 0; }

    private:
        int m_Score = 0;

        // Inherited via Component
        void Update() override;
        void FixedUpdate() override;
        void Render() const override;
    };
}
