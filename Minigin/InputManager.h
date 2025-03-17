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
	enum class InputType
	{
		Pressed,  // Executes when button is pressed (first frame)
		Released, // Executes when button is released
		Held      // Executes while button is held
	};
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager() = default;
		bool ProcessInput();

		// Bind commands to keyboard/controller inputs
		void BindKeyboardInput(SDL_Keycode key, std::shared_ptr<Command> command, InputType type = InputType::Pressed);
		void BindControllerInput(Gamepad::Button button, std::shared_ptr<Command> command, InputType type = InputType::Pressed, int controllerIndex = 0);


	private:
		std::vector<std::unique_ptr<Gamepad>> m_Gamepads; 
		std::unordered_map<SDL_Keycode, std::pair<std::shared_ptr<Command>, InputType>> m_KeyboardCommands;
		std::unordered_map<int, std::unordered_map<Gamepad::Button, std::pair<std::shared_ptr<Command>, InputType>>> m_ControllerCommands;

		std::unordered_set<SDL_Keycode> m_KeysHeld; // Store keys currently being held
	};

}
