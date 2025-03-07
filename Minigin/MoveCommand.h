#pragma once
#include "Command.h"
#include "GameObject.h"

namespace dae
{
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject* object, float xDir, float yDir, float speed);
			
		void Execute() override;

	private:
		GameObject* m_pObject;
		float m_XDir, m_YDir;
		float m_Speed;
	};

	
}

