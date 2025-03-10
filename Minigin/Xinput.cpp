#include <Windows.h>
#include <Xinput.h> // Include the real XInput API
#include "Xinput.h"
#include <iostream>


class dae::Xinput::XinputImpl
{
public:
	XinputImpl()
	{
		ZeroMemory(&m_PreviousControllerState, sizeof(XINPUT_STATE));
		ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
	}
	~XinputImpl() = default;
	void ProcessControllerInput()
	{
		// Store previous state before updating
		CopyMemory(&m_PreviousControllerState, &m_CurrentControllerState, sizeof(XINPUT_STATE));
		// Clear current state and get new input data
		ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
		XInputGetState(m_ControllerIndex, &m_CurrentControllerState);

		if (XInputGetState(0, &m_CurrentControllerState) == ERROR_SUCCESS)
		{
			auto buttonChanges = m_CurrentControllerState.Gamepad.wButtons ^ m_PreviousControllerState.Gamepad.wButtons;
			m_ButtonsPressedThisFrame = buttonChanges & m_CurrentControllerState.Gamepad.wButtons;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentControllerState.Gamepad.wButtons);
		}
	}
	// Check if a button was just pressed this frame
	bool IsDownThisFrame(unsigned int button) const
	{
		return m_ButtonsPressedThisFrame & button;
	}
	// Check if a button was just released this frame
	bool IsUpThisFrame(unsigned int button) const
	{
		return m_ButtonsReleasedThisFrame & button;
	}
	// Check if a button is currently being held down
	bool IsPressed(unsigned int button) const
	{
		return m_CurrentControllerState.Gamepad.wButtons & button;
	}
private:
	int m_ControllerIndex{ 0 }; // Default to controller 0
	XINPUT_STATE m_PreviousControllerState{};
	XINPUT_STATE m_CurrentControllerState{};
	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsReleasedThisFrame{};
};

dae::Xinput::Xinput() : m_pImpl(std::make_unique<XinputImpl>()) {}
//{
//	ZeroMemory(&m_PreviousControllerState, sizeof(XINPUT_STATE));
//	ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
//}

dae::Xinput::~Xinput() = default;

void dae::Xinput::ProcessControllerInput()
{
	m_pImpl->ProcessControllerInput();
    //// Store previous state before updating
    //CopyMemory(&m_PreviousControllerState, &m_CurrentControllerState, sizeof(XINPUT_STATE));

    //// Clear current state and get new input data
    //ZeroMemory(&m_CurrentControllerState, sizeof(XINPUT_STATE));
    //XInputGetState(m_ControllerIndex, &m_CurrentControllerState);

    //// Detect button changes
    //auto buttonChanges = m_CurrentControllerState.Gamepad.wButtons ^ m_PreviousControllerState.Gamepad.wButtons;
    //m_ButtonsPressedThisFrame = buttonChanges & m_CurrentControllerState.Gamepad.wButtons;
    //m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentControllerState.Gamepad.wButtons);
}

// Check if a button was just pressed this frame
bool dae::Xinput::IsDownThisFrame(unsigned int button) const
{
	return m_pImpl->IsDownThisFrame(button);
    //return m_ButtonsPressedThisFrame & button;
}

// Check if a button was just released this frame
bool dae::Xinput::IsUpThisFrame(unsigned int button) const
{
	return m_pImpl->IsUpThisFrame(button);
    //return m_ButtonsReleasedThisFrame & button;
}

// Check if a button is currently being held down
bool dae::Xinput::IsPressed(unsigned int button) const
{
	return m_pImpl->IsPressed(button);
    //return m_CurrentControllerState.Gamepad.wButtons & button;
}