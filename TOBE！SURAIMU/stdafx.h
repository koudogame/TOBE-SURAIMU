#pragma once
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <bitset>
#include <list>
#include <cstdint>
#include <wrl/client.h>
#include <string>
#include <functional>
#include <vector>
#include <deque>
#include <map>
#include <utility>
#include <chrono>
#include <fstream>
#include <Audio.h>
#include <thread>
#include <dinput.h>


#include <Windows.h>
#include <CommonStates.h>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <GamePad.h>
#include <crtdbg.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

using namespace DirectX;
using namespace SimpleMath;

template <typename T>
inline constexpr T getWindowWidth()
{
	return static_cast<T>(1280);
}

template <typename T>
inline constexpr T getWindowHeight()
{
	return static_cast<T>(720);
}
