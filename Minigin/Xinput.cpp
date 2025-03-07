#include <Windows.h>
#include "Xinput.h"
#include <iostream>


dae::Xinput::Xinput()
{
	ZeroMemory(&m_PreviousControllerState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
}

dae::Xinput::~Xinput() = default;

void dae::Xinput::ProcessControllerInput()
{
    // Store previous state before updating
    CopyMemory(&m_PreviousControllerState, &m_CurrentControllerState, sizeof(XINPUT_STATE));

    // Clear current state and get new input data
    ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
    XInputGetState(m_ControllerIndex, &m_CurrentControllerState);

    // Detect button changes
    auto buttonChanges = m_CurrentControllerState.Gamepad.wButtons ^ m_PreviousControllerState.Gamepad.wButtons;
    m_ButtonsPressedThisFrame = buttonChanges & m_CurrentControllerState.Gamepad.wButtons;
    m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentControllerState.Gamepad.wButtons);
}

// Check if a button was just pressed this frame
bool dae::Xinput::IsDownThisFrame(unsigned int button) const
{
    return m_ButtonsPressedThisFrame & button;
}

// Check if a button was just released this frame
bool dae::Xinput::IsUpThisFrame(unsigned int button) const
{
    return m_ButtonsReleasedThisFrame & button;
}

// Check if a button is currently being held down
bool dae::Xinput::IsPressed(unsigned int button) const
{
    return m_CurrentControllerState.Gamepad.wButtons & button;
}