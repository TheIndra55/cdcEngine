#pragma once

#include "guiddef.h"

#include "cdc/runtime/cdcSys/UTF8String.h"

class InputDevice
{
public:
	struct GamepadInstance
	{
		cdc::UTF8String* name;
		GUID instanceGuid;
	};

	struct UpdateInfo
	{
	};

	static void Create();
	static void Destroy();
	static int GetNumDevices();
	static void Update();
	static InputDevice* GetDevicePtr();

	virtual const char* GetDeviceUniqueID() = 0;
	virtual int Poll(UpdateInfo* infoBlocks, int maxBlocks, bool fullState) = 0;
	virtual unsigned int GetTimeStamp() = 0;
	virtual int GetNumButtons() = 0;
	virtual int GetNumAxis() = 0;
	virtual cdc::UTF8String* GetButtonName(int buttonIdx) = 0;
	virtual cdc::UTF8String* GetAxisName(int axisIdx) = 0;
	virtual void SetOverrideButtonName(int buttonIdx, cdc::UTF8String* pName) = 0;
	virtual int GetNumMotors() = 0;
	virtual int GetMotorCaps(int motorIndex) = 0;
	virtual void SetMotorIntensity(int motorIndex, float intensity) = 0;
	virtual void ApplyIntensities() = 0;
};