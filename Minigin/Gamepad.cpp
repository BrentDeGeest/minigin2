#include "Gamepad.h"
#include <Windows.h>
#include <Xinput.h>
#include <iostream>

class dae::Gamepad::GamepadImpl
{
public:
    explicit GamepadImpl(int controllerIndex)
        : m_ControllerIndex(controllerIndex)
    {
        ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
    }

    void Update()
    {
        // Save previous state before updating
        CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));

        // Clear current state and get new input data
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        if (XInputGetState(m_ControllerIndex, &m_CurrentState) == ERROR_SUCCESS)
        {
            auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
            m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
            m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
        }
    }

    bool IsDownThisFrame(dae::Gamepad::Button button/*unsigned int button*/) const
    {
        return (m_ButtonsPressedThisFrame & GetXInputValue(button)) != 0;
        //return m_ButtonsPressedThisFrame & button;
    }

    bool IsUpThisFrame(dae::Gamepad::Button button/*unsigned int button*/) const
    {
        return (m_ButtonsReleasedThisFrame & GetXInputValue(button)) != 0;
        //return m_ButtonsReleasedThisFrame & button;
    }

    bool IsPressed(dae::Gamepad::Button button/*unsigned int button*/) const
    {
        return (m_CurrentState.Gamepad.wButtons & GetXInputValue(button)) != 0;
        //return m_CurrentState.Gamepad.wButtons & button;
    }

    bool IsConnected() const
    {
        XINPUT_STATE tempState;
        return XInputGetState(m_ControllerIndex, &tempState) == ERROR_SUCCESS;
    }

private:
    int m_ControllerIndex;
    XINPUT_STATE m_PreviousState{};
    XINPUT_STATE m_CurrentState{};
    WORD m_ButtonsPressedThisFrame{};
    WORD m_ButtonsReleasedThisFrame{};

    static WORD GetXInputValue(dae::Gamepad::Button button)
    {
        switch (button)
        {
        case dae::Gamepad::Button::DPadUp:        return static_cast<WORD>(XINPUT_GAMEPAD_DPAD_UP);
        case dae::Gamepad::Button::DPadDown:      return static_cast<WORD>(XINPUT_GAMEPAD_DPAD_DOWN);
        case dae::Gamepad::Button::DPadLeft:      return static_cast<WORD>(XINPUT_GAMEPAD_DPAD_LEFT);
        case dae::Gamepad::Button::DPadRight:     return static_cast<WORD>(XINPUT_GAMEPAD_DPAD_RIGHT);
        case dae::Gamepad::Button::Start:         return static_cast<WORD>(XINPUT_GAMEPAD_START);
        case dae::Gamepad::Button::Back:          return static_cast<WORD>(XINPUT_GAMEPAD_BACK);
        case dae::Gamepad::Button::LeftThumb:     return static_cast<WORD>(XINPUT_GAMEPAD_LEFT_THUMB);
        case dae::Gamepad::Button::RightThumb:    return static_cast<WORD>(XINPUT_GAMEPAD_RIGHT_THUMB);
        case dae::Gamepad::Button::LeftShoulder:  return static_cast<WORD>(XINPUT_GAMEPAD_LEFT_SHOULDER);
        case dae::Gamepad::Button::RightShoulder: return static_cast<WORD>(XINPUT_GAMEPAD_RIGHT_SHOULDER);
        case dae::Gamepad::Button::A:             return static_cast<WORD>(XINPUT_GAMEPAD_A);
        case dae::Gamepad::Button::B:             return static_cast<WORD>(XINPUT_GAMEPAD_B);
        case dae::Gamepad::Button::X:             return static_cast<WORD>(XINPUT_GAMEPAD_X);
        case dae::Gamepad::Button::Y:             return static_cast<WORD>(XINPUT_GAMEPAD_Y);
        default:                                  return 0; // Prevents unhandled cases
        }
    }
};


dae::Gamepad::Gamepad(int controllerIndex)
	: m_ControllerIndex(controllerIndex), m_pImpl(std::make_unique<GamepadImpl>(controllerIndex))
{
    /*ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
    ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));*/
}

dae::Gamepad::~Gamepad() = default;

void dae::Gamepad::Update()
{
	m_pImpl->Update();
    //// Save previous state before updating
    //CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));

    //// Clear current state and get new input data
    //ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
    //if (XInputGetState(m_ControllerIndex, &m_CurrentState) == ERROR_SUCCESS)
    //{
    //    auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
    //    m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
    //    m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
    //}
}

bool dae::Gamepad::IsDownThisFrame(Button button/*unsigned int button*/) const
{
	return m_pImpl->IsDownThisFrame(button);
    //return m_ButtonsPressedThisFrame & button;
}

bool dae::Gamepad::IsUpThisFrame(Button button/*unsigned int button*/) const
{
	return m_pImpl->IsUpThisFrame(button);
    //return m_ButtonsReleasedThisFrame & button;
}

bool dae::Gamepad::IsPressed(Button button/*unsigned int button*/) const
{
	return m_pImpl->IsPressed(button);
    //return m_CurrentState.Gamepad.wButtons & button;
}



bool dae::Gamepad::IsConnected() const
{
	return m_pImpl->IsConnected();
	
	//XINPUT_STATE tempState;
    //return XInputGetState(m_ControllerIndex, &tempState) == ERROR_SUCCESS;
}
