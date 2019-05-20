#pragma once

#include "object_base.h"

struct Milliseconds;
template <typename T>
class Timer;

//-----------------------------------------------------------------------------
// UI : コンボ
//-----------------------------------------------------------------------------
// --説明--
// addCombo関数でコンボの追加を行う、同時に経過時間のリセットがかかる
// 
class Combo :
    public ObjectBase
{
public:
    Combo(TaskManager* const TaskManager);
    ~Combo();

    bool init(const Timer<Milliseconds>& Clock);
    void destroy() override;
    void update() override;
    void draw() override;
    bool isAlive() override { return life_time_ > 0LL; }

    void resetStates(const float Magnification) { magnification_ = Magnification; }

    int getCombo() const { return combo_; }
    void addCombo();

private:
    const Timer<Milliseconds>* clock_ = nullptr;
    long long prev_time_ms_ = 0;
    float magnification_ = 1.0F;
    long long life_time_ = 0;
    bool is_alive_ = true;
    int combo_ = 0;
};
