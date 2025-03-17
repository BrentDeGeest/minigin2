#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SDL.h>
#include "InputManager.h"
#include <iostream>
#include <Xinput.h>

dae::InputManager::InputManager()
{
	// Create gamepads for up to 4 controllers
	for (int i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		m_Gamepads.push_back(std::make_unique<Gamepad>(i));
	}
}

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}

		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
		{
			auto it = m_KeyboardCommands.find(e.key.keysym.sym);
			if (it != m_KeyboardCommands.end())
			{
				auto& [command, type] = it->second;
				if ((type == InputType::Pressed && e.type == SDL_KEYDOWN) ||
					(type == InputType::Released && e.type == SDL_KEYUP))
				{
					command->Execute();
				}
			}

			if (e.type == SDL_KEYDOWN)
			{
				m_KeysHeld.insert(e.key.keysym.sym);
			}
			else if (e.type == SDL_KEYUP)
			{
				m_KeysHeld.erase(e.key.keysym.sym);
			}
		}
	}

	

	// Update all connected gamepads
	for (auto& gamepad : m_Gamepads)
	{
		gamepad->Update();
	}

	// Execute commands for all controllers
	for (size_t i = 0; i < m_Gamepads.size(); ++i)
	{
		if (!m_Gamepads[i]->IsConnected()) continue;

		int controllerIndex = static_cast<int>(i); // Explicitly cast size_t to int

		for (auto& [button, commandPair] : m_ControllerCommands[controllerIndex])
		{
			auto& [command, type] = commandPair;
			if ((type == InputType::Pressed && m_Gamepads[controllerIndex]->IsDownThisFrame(button)) ||
				(type == InputType::Released && m_Gamepads[controllerIndex]->IsUpThisFrame(button)) ||
				(type == InputType::Held && m_Gamepads[controllerIndex]->IsPressed(button)))
			{
				command->Execute();
			}
		}
	}

	// Execute commands for keyboard keys being held
	for (const auto& key : m_KeysHeld)
	{
		auto it = m_KeyboardCommands.find(key);
		if (it != m_KeyboardCommands.end() && it->second.second == InputType::Held)
		{
			it->second.first->Execute();
		}
	}

	return true;
}

void dae::InputManager::BindKeyboardInput(SDL_Keycode key, std::shared_ptr<Command> command, InputType type)
{
	m_KeyboardCommands[key] = { std::move(command), type };
}

void dae::InputManager::BindControllerInput(Gamepad::Button button, std::shared_ptr<Command> command, InputType type, int controllerIndex)
{
	m_ControllerCommands[controllerIndex][button] = { std::move(command), type };
}