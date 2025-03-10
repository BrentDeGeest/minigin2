#pragma once
#include "Singleton.h"
#include <memory>
#include <unordered_map>
//#include "Xinput.h"
#include "Command.h"
#include <SDL.h>
#include <unordered_set>
#include "Gamepad.h"

namespace dae
{
	
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager() = default;
		bool ProcessInput();

		// Bind commands to keyboard/controller inputs
		void BindKeyboardInput(SDL_Keycode key, std::shared_ptr<Command> command);
		void BindControllerInput(Gamepad::Button button, std::shared_ptr<Command> command, int controllerIndex = 0);

	private:
		std::vector<std::unique_ptr<Gamepad>> m_Gamepads; 
		std::unordered_map<SDL_Keycode, std::shared_ptr<Command>> m_KeyboardCommands;
		std::unordered_map<int, std::unordered_map<Gamepad::Button, std::shared_ptr<Command>>> m_ControllerCommands; // Map controllers separately
		std::unordered_set<SDL_Keycode> m_KeysHeld; // Store keys currently being held
	};

}
