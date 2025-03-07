#pragma once
#include <Windows.h>
#include <Xinput.h>

namespace dae
{
    class Gamepad
    {
    public:
		explicit Gamepad(int controllerIndex); // Allow multiple controllers
        ~Gamepad() = default;

        void Update(); // Updates button states
        bool IsConnected() const; // Checks if controller is active
		int GetControllerIndex() const { return m_ControllerIndex; }

        bool IsDownThisFrame(unsigned int button) const;
        bool IsUpThisFrame(unsigned int button) const;
        bool IsPressed(unsigned int button) const;


        enum class Button
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
        int m_ControllerIndex;
        XINPUT_STATE m_PreviousState{};
        XINPUT_STATE m_CurrentState{};
        WORD m_ButtonsPressedThisFrame{};
        WORD m_ButtonsReleasedThisFrame{};
    };
}
