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

void Dinput::update()
{
	device_->Acquire();
	ZeroMemory(&state_, sizeof(state_));
	auto test = device_->GetDeviceState(sizeof(state_), &state_);
	if (FAILED(test))
	{
		auto hr = device_->Acquire();
		while (hr == DIERR_INPUTLOST)
			auto hr = device_->Acquire();
		device_->GetDeviceState(sizeof(state_), &state_);
	}
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
