#pragma once
namespace dae
{
    class GameObject; // Forward declaration

    class Component
    {
    public:
        virtual ~Component() = default;

		//virtual void OnStart() = 0;
        virtual void Update() = 0;
        virtual void FixedUpdate() = 0;
		virtual void Render() const = 0;

        void SetOwner(GameObject* owner) { m_pOwner = owner; }
        GameObject* GetOwner() const { return m_pOwner; }

    protected:
        GameObject* m_pOwner = nullptr; // Pointer to the parent GameObject
    };
}