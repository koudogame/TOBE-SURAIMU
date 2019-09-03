#pragma once
class Dinput
{
//singleton
private:
	Dinput();
	Dinput(const Dinput&) = delete;
	Dinput& operator= (const Dinput&) = delete;
public:
	static Dinput* getInstance() { static Dinput instance; return &instance; }

public:
	~Dinput();

public:
	bool init(const HINSTANCE Hinst, const HWND hWnd);
	void update();
	void destroy();
	bool setDevice(const DIDEVICEINSTANCE * pdidInstance);
	const DIJOYSTATE& getState() { return state_; }
	const LPDIRECTINPUT8* getInputDevice() { return &input_device_; }
	LPDIRECTINPUTDEVICE8* getDevice() { return &device_; }

private:
	LPDIRECTINPUT8 input_device_;
	LPDIRECTINPUTDEVICE8 device_;
	DIDEVCAPS caps_;
	DIJOYSTATE state_;
	DIJOYSTATE old_state_;
};

