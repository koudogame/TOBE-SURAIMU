#include "dinput.h"
#include "commonstates.h"

BOOL CALLBACK EnumJoySticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext);

Dinput::Dinput()
{
}


Dinput::~Dinput()
{
}

bool Dinput::init(const HINSTANCE Hinst, const HWND hWnd)
{
	if (FAILED(DirectInput8Create(
		Hinst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)(&input_device_),
		NULL)))
	{
		return false;
	}

	if (FAILED(input_device_->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoySticksCallback,
		nullptr,
		DIEDFL_ATTACHEDONLY)))
	{
		return false;
	}

	if (FAILED(device_->SetDataFormat(&c_dfDIJoystick)))
	{
		return false;
	}
	if (FAILED(device_->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
	{
		return false;
	}
	caps_.dwSize = sizeof(DIDEVCAPS);
	if (FAILED(device_->GetCapabilities(&caps_)))
	{
		return false;
	}
	if (FAILED(device_->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS)))
	{
		return false;
	}
	ZeroMemory(&state_, sizeof(state_));

	return true;
}

void Dinput::update(GamePad::State * State)
{
	auto old = state_;
	device_->Acquire();
	device_->GetDeviceState(sizeof(state_), &state_);
	setState(State);
}

void Dinput::destroy()
{
	input_device_->Release();
	device_->Release();
}

bool Dinput::setDevice(const DIDEVICEINSTANCE * pdidInstance)
{
	HRESULT hr;
	hr = (*Dinput::getInstance()->getInputDevice())->CreateDevice(pdidInstance->guidInstance, &device_, NULL);
	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}

void Dinput::setState(GamePad::State * State)
{
	State->buttons.a = state_.rgbButtons[1] & 0x80;
	State->buttons.b = state_.rgbButtons[2] & 0x80;
	State->buttons.y = state_.rgbButtons[3] & 0x80;
	State->buttons.x = state_.rgbButtons[0] & 0x80;
	State->buttons.menu = state_.rgbButtons[9] & 0x80;
	State->buttons.view = state_.rgbButtons[8] & 0x80;
	State->buttons.leftShoulder = state_.rgbButtons[4] & 0x80;
	State->buttons.rightShoulder = state_.rgbButtons[6] & 0x80;
	State->buttons.leftStick = state_.rgbButtons[10] & 0x80;
	State->buttons.rightStick = state_.rgbButtons[11] & 0x80;

	State->dpad.up = state_.rgdwPOV[0] == 0;
	State->dpad.right = state_.rgdwPOV[0] == 9000;
	State->dpad.down = state_.rgdwPOV[0] == 18000;
	State->dpad.left = state_.rgdwPOV[0] == 27000;

	State->thumbSticks.leftX = static_cast<float>(state_.lX) / 1000.0F;
	State->thumbSticks.leftY = static_cast<float>(-state_.lY) / 1000.0F;
	State->thumbSticks.rightX = static_cast<float>(state_.lZ) / 1000.0F;
	State->thumbSticks.rightY = static_cast<float>(-state_.lRz) / 1000.0F;

	if (state_.rgbButtons[6] & 0x80)
		State->triggers.left = 1.0F;
	if (state_.rgbButtons[7] & 0x80)
		State->triggers.right = 1.0F;
}

BOOL CALLBACK EnumJoySticksCallback(const DIDEVICEINSTANCE * pdidInstance, VOID * pContext)
{
	return Dinput::getInstance()->setDevice(pdidInstance);
}

BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	HRESULT     hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = -1000;
	diprg.lMax = +1000;
	hr = (*Dinput::getInstance()->getDevice())->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr)) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}
