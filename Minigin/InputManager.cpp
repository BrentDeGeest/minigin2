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

		if (e.type == SDL_KEYDOWN) {
			// Add key to the set when first pressed
			m_KeysHeld.insert(e.key.keysym.sym);
		}

		if (e.type == SDL_KEYUP)
		{
			// Remove key from the set when released
			m_KeysHeld.erase(e.key.keysym.sym);
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

		for (const auto& [button, command] : m_ControllerCommands[int(i)])
		{
			if (command && m_Gamepads[i]->IsPressed(button) )
				//if (m_Gamepads[i]->IsPressed(static_cast<unsigned int>(button)) && command)
			{
				command->Execute();
			}
		}
	}

	// Execute commands for **keyboard keys being held**
	for (const auto& key : m_KeysHeld)
	{
		auto it = m_KeyboardCommands.find(key);
		if (it != m_KeyboardCommands.end() && it->second)
		{
			it->second->Execute();
		}
	}

	return true;
}

void dae::InputManager::BindKeyboardInput(SDL_Keycode key, std::shared_ptr<Command> command)
{
	m_KeyboardCommands[key] = std::move(command);
}

void dae::InputManager::BindControllerInput(Gamepad::Button button, std::shared_ptr<Command> command, int controllerIndex)
{
	m_ControllerCommands[controllerIndex][button] = std::move(command);
}