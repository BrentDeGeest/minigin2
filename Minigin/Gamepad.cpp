#include "Gamepad.h"
#include <iostream>

dae::Gamepad::Gamepad(int controllerIndex)
    : m_ControllerIndex(controllerIndex)
{
    ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
    ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
}

void dae::Gamepad::Update()
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

bool dae::Gamepad::IsDownThisFrame(unsigned int button) const
{
    return m_ButtonsPressedThisFrame & button;
}

bool dae::Gamepad::IsUpThisFrame(unsigned int button) const
{
    return m_ButtonsReleasedThisFrame & button;
}

bool dae::Gamepad::IsPressed(unsigned int button) const
{
    return m_CurrentState.Gamepad.wButtons & button;
}

bool dae::Gamepad::IsConnected() const
{
	XINPUT_STATE tempState;
    return XInputGetState(m_ControllerIndex, &tempState) == ERROR_SUCCESS;
}
