#pragma once

#include "direct3d.h"

class Common
{
private:
	Common() = default;
    std::unique_ptr<CommonStates> status_;

public:
	static Common* getInstance(){ static Common instance; return &instance; }
    bool init();
    CommonStates* getStates() { return status_.get(); }
};
