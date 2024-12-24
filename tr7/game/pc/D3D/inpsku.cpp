#include "inpsku.h"

#include <Windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

static int s_enumNumJoysticks = 0;
static PCCombinationInputDevice* s_pComboInputDevice = nullptr;
static IDirectInput8W* s_pDI;

HMODULE hXInputModule;

static void InitXInput()
{
	hXInputModule = LoadLibrary("xinput9_1_0.dll");

	if (hXInputModule)
	{

	}
}

void InputDevice::Create()
{
	s_enumNumJoysticks = 0;

	CoInitialize(NULL);
	if (SUCCEEDED(CoCreateInstance(CLSID_DirectInput8, NULL, CLSCTX_INPROC_SERVER, IID_IDirectInput8W, (LPVOID*)&s_pDI)))
	{
		if (SUCCEEDED(s_pDI->Initialize(GetModuleHandle(NULL), DIRECTINPUT_VERSION)))
		{
			InitXInput();

			s_pComboInputDevice = new PCCombinationInputDevice();
		}
	}
}

void InputDevice::Destroy()
{
}

int InputDevice::GetNumDevices()
{
	return 1;
}

void InputDevice::Update()
{
}

InputDevice* InputDevice::GetDevicePtr()
{
	return s_pComboInputDevice;
}

PCCombinationInputDevice::PCCombinationInputDevice() : m_gamepadInstances()
{
	m_pOverrideButtonNames = nullptr;
	m_pKeybDev = nullptr;
	m_pMouseDev = nullptr;
	m_pGamepadDev = nullptr;
	m_pGamepadGUID = nullptr;
	m_bIsGamepadEnabled = true;
	m_bIsMouseEnabled = true;
	m_bDisableMouseButtons = false;
}

const char* PCCombinationInputDevice::GetDeviceUniqueID()
{
	if (m_pGamepadDev)
	{
		return m_pGamepadDev->GetDeviceUniqueID();
	}

	return "Unknown";
}

int PCCombinationInputDevice::Poll(UpdateInfo* infoBlocks, int maxBlocks, bool fullState)
{
	return maxBlocks;
}

unsigned int PCCombinationInputDevice::GetTimeStamp()
{
	return m_pKeybDev->GetTimeStamp();
}

int PCCombinationInputDevice::GetNumButtons()
{
	return m_pKeybDev->GetNumButtons() + 128;
}

int PCCombinationInputDevice::GetNumAxis()
{
	return m_pKeybDev->GetNumAxis() + 20;
}

cdc::UTF8String* PCCombinationInputDevice::GetButtonName(int buttonIdx)
{
	if (buttonIdx >= GetNumButtons())
	{
		return nullptr;
	}

	if (m_pOverrideButtonNames[buttonIdx])
	{
		return m_pOverrideButtonNames[buttonIdx];
	}

	if (m_pGamepadDev && buttonIdx >= 64 && buttonIdx < m_pGamepadDev->GetNumButtons() + 64)
	{
		return m_pGamepadDev->GetButtonName(buttonIdx - 64);
	}

	if (m_pMouseDev && buttonIdx >= 96 && buttonIdx < m_pMouseDev->GetNumButtons() + 96)
	{
		return m_pMouseDev->GetButtonName(buttonIdx - 96);
	}

	if (m_pKeybDev && buttonIdx >= 128 && buttonIdx < m_pKeybDev->GetNumButtons() + 128)
	{
		return m_pKeybDev->GetButtonName(buttonIdx - 128);
	}

	return nullptr;
}

cdc::UTF8String* PCCombinationInputDevice::GetAxisName(int axisIdx)
{
	if (m_pGamepadDev && axisIdx >= 8 && axisIdx < m_pGamepadDev->GetNumAxis() + 8)
	{
		return m_pGamepadDev->GetAxisName(axisIdx - 8);
	}

	if (m_pMouseDev && axisIdx >= 16 && axisIdx < m_pMouseDev->GetNumAxis() + 16)
	{
		return m_pMouseDev->GetAxisName(axisIdx - 16);
	}

	if (m_pKeybDev && axisIdx >= 20 && axisIdx < m_pKeybDev->GetNumAxis() + 20)
	{
		return m_pKeybDev->GetAxisName(axisIdx - 20);
	}

	return nullptr;
}

void PCCombinationInputDevice::SetOverrideButtonName(int buttonIdx, cdc::UTF8String* pName)
{
}

int PCCombinationInputDevice::GetNumMotors()
{
	if (m_pGamepadDev)
	{
		return m_pGamepadDev->GetNumMotors();
	}

	return 0;
}

int PCCombinationInputDevice::GetMotorCaps(int motorIndex)
{
	if (m_pGamepadDev)
	{
		return m_pGamepadDev->GetMotorCaps(motorIndex);
	}

	return 4;
}

void PCCombinationInputDevice::SetMotorIntensity(int motorIndex, float intensity)
{
	if (m_pGamepadDev)
	{
		m_pGamepadDev->SetMotorIntensity(motorIndex, intensity);
	}
}

void PCCombinationInputDevice::ApplyIntensities()
{
	if (m_pGamepadDev)
	{
		m_pGamepadDev->ApplyIntensities();
	}
}

PCSubInputDevice::PCSubInputDevice() : m_buttonNames(), m_axisNames()
{
}

PCXInputDevice::PCXInputDevice(unsigned int iPort) : PCSubInputDevice()
{
	m_packetNumber = 0;
	m_numAxis = 4;
	m_numButtons = 16;
	m_iPort = iPort;

	m_buttonNames[0]  = new cdc::UTF8String("Y Button");
	m_buttonNames[1]  = new cdc::UTF8String("X Button");
	m_buttonNames[2]  = new cdc::UTF8String("B Button");
	m_buttonNames[3]  = new cdc::UTF8String("A Button");
	m_buttonNames[4]  = new cdc::UTF8String("Left Trigger");
	m_buttonNames[5]  = new cdc::UTF8String("Right Trigger");
	m_buttonNames[6]  = new cdc::UTF8String("Left Shoulder");
	m_buttonNames[7]  = new cdc::UTF8String("Right Shoulder");
	m_buttonNames[8]  = new cdc::UTF8String("Left Thumbstick");
	m_buttonNames[9]  = new cdc::UTF8String("Right Thumbstick");
	m_buttonNames[10] = new cdc::UTF8String("Start Button");
	m_buttonNames[11] = new cdc::UTF8String("Back Button");
	m_buttonNames[12] = new cdc::UTF8String("DPad Up");
	m_buttonNames[13] = new cdc::UTF8String("DPad Down");
	m_buttonNames[14] = new cdc::UTF8String("DPad Left");
	m_buttonNames[15] = new cdc::UTF8String("DPad Right");

	m_axisNames[1] = new cdc::UTF8String("Left Stick X");
	m_axisNames[2] = new cdc::UTF8String("Left Stick Y");
	m_axisNames[3] = new cdc::UTF8String("Right Stick X");
	m_axisNames[4] = new cdc::UTF8String("Right Stick Y");
}