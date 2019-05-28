
// 板場 温樹

#include "combo.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"
#include "timer.h"

#include "numbers.h"

const long long kLifeTimeMs = 3000LL;

/*===========================================================================*/
Combo::Combo()
{
}

Combo::~Combo()
{
    destroy();
}

/*===========================================================================*/
// 初期化処理
bool Combo::init(const Timer<Milliseconds>& Clock)
{
    destroy();

    // テクスチャ読み込み
    texture_ = TextureLoder::getInstance()->load(L"Texture/数字.png");
    if (texture_ == nullptr) { return false; }

    // タスクの設定
    TaskManager::getInstance()->registerTask(this, TaskUpdate::kComboUpdate);
    TaskManager::getInstance()->registerTask(this, TaskDraw::kObject);

    // 各メンバ初期化
    clock_ = &Clock; 
    prev_time_ms_ = clock_->getCount();
    magnification_ = 1.0F;
    life_time_ = kLifeTimeMs;
    is_alive_ = true;
    combo_ = 0;
    

    return true;
}

/*===========================================================================*/
// 終了処理
void Combo::destroy()
{
    TaskManager::getInstance()->unregisterObject(this);

    TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新処理
void Combo::update()
{
    const long long kNow = clock_->getCount();
    const long long kElapsed = kNow - prev_time_ms_;
    prev_time_ms_ = kNow;

    life_time_ -= static_cast<long long>(static_cast<long double>(kElapsed) * static_cast<long double>(magnification_));
    if (life_time_ <= 0LL) 
    {
        // 一定時間経過でコンボリセット
        life_time_ = combo_ = 0LL;
    }
}

/*===========================================================================*/
// 描画処理
void Combo::draw()
{
    Numbers<long long> combo(combo_);

    if (combo_ > 0LL)
    {
        combo.draw(
            texture_,
            Vector2(320.0F, 0.0F),
            64L, 128L);
    }
}

/*===========================================================================*/
// コンボ加算
void Combo::addCombo()
{
    combo_ += 1LL;
    life_time_ = kLifeTimeMs;
}
