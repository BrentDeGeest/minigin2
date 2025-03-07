#pragma once

// Prevent Windows bloat and reduce macro conflicts
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <Xinput.h> // Include the real XInput API

namespace dae
{
	class Xinput
	{
	public:
		Xinput();
		~Xinput();

		void ProcessControllerInput(); // Handles controller input processing

		// Button state functions
		bool IsDownThisFrame(unsigned int button) const;
		bool IsUpThisFrame(unsigned int button) const;
		bool IsPressed(unsigned int button) const;

		// Controller button enum
		enum class ControllerButton
		{
			DPadUp = XINPUT_GAMEPAD_DPAD_UP,
			DPadDown = XINPUT_GAMEPAD_DPAD_DOWN,
			DPadLeft = XINPUT_GAMEPAD_DPAD_LEFT,
			DPadRight = XINPUT_GAMEPAD_DPAD_RIGHT,
			Start = XINPUT_GAMEPAD_START,
			Back = XINPUT_GAMEPAD_BACK,
			LeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
			RightThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
			LeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
			RightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
			A = XINPUT_GAMEPAD_A,
			B = XINPUT_GAMEPAD_B,
			X = XINPUT_GAMEPAD_X,
			Y = XINPUT_GAMEPAD_Y
		};

	private:
		int m_ControllerIndex{ 0 }; // Default to controller 0
		XINPUT_STATE m_PreviousControllerState{};
		XINPUT_STATE m_CurrentControllerState{};
		WORD m_ButtonsPressedThisFrame{};
		WORD m_ButtonsReleasedThisFrame{};
	};
}