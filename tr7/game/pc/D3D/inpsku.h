#pragma once

#include "game/input/inputdevice.h"

#include "cdc/runtime/cdcSys/Array.h"

class PCSubInputDevice : public InputDevice
{
protected:
	//IDirectInput8W* m_pDIDevice;

	cdc::UTF8String* m_buttonNames[256];
	int m_buttonOffsets[256];
	cdc::UTF8String* m_axisNames[16];
	int m_axisOffsets[16];

	int m_numButtons;
	int m_numAxis;

public:
	PCSubInputDevice();
};

class PCKeyboardDevice : public PCSubInputDevice
{
};

class PCMouseDevice :public PCSubInputDevice
{
};

class PCCombinationInputDevice : public InputDevice
{
private:
	cdc::Array<GamepadInstance> m_gamepadInstances;

	bool m_bIsGamepadEnabled;
	GUID* m_pGamepadGUID;
	bool m_bIsMouseEnabled;
	bool m_bDisableMouseButtons;

	cdc::UTF8String** m_pOverrideButtonNames;

	PCKeyboardDevice* m_pKeybDev;
	PCMouseDevice* m_pMouseDev;
	PCSubInputDevice* m_pGamepadDev;

public:
	PCCombinationInputDevice();

	const char* GetDeviceUniqueID();
	int Poll(UpdateInfo* infoBlocks, int maxBlocks, bool fullState);
	unsigned int GetTimeStamp();
	int GetNumButtons();
	int GetNumAxis();
	cdc::UTF8String* GetButtonName(int buttonIdx);
	cdc::UTF8String* GetAxisName(int axisIdx);
	void SetOverrideButtonName(int buttonIdx, cdc::UTF8String* pName);
	int GetNumMotors();
	int GetMotorCaps(int motorIndex);
	void SetMotorIntensity(int motorIndex, float intensity);
	void ApplyIntensities();
};

class PCXInputDevice : public PCSubInputDevice
{
private:
	unsigned int m_packetNumber;
	unsigned int m_iPort;
	float m_m1Intensity;
	float m_m2Intensity;

public:
	PCXInputDevice(unsigned int iPort);
};