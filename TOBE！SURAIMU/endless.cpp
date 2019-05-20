
// 板場 温樹

#include "endless.h"

#include <direct.h>
#include "release.h"
#include "key.h"
#include "pad.h"
#include "textureLoder.h"
#include "sprite.h"

#include "timer.h"
#include "collision.h"
#include "task_manager.h"
#include "background.h"
#include "star_container.h"
#include "player.h"
#include "wall.h"
#include "combo.h"

#include "result.h"

using PadState = GamePad::State;
using PadTracker = GamePad::ButtonStateTracker;

/*===========================================================================*/
// 難易度に関係
constexpr Vector2 kInitStarPosi[] = {
    {640.0F, 600.0F},
    {810.0F, 100.0F},
};
constexpr float kInitStarAngle[] = {
    90.0F,
    90.0F,
    90.0F,
};
constexpr float kInitStarFall[] = {
    2.0F,
    1.0F,
    5.0F,
};
constexpr float kInitStarSpin[] = {
    3.0F,
    3.0F,
    3.0F,
};
constexpr float kInitStarSpinRate[] = {
    0.01F,
    0.01F,
    0.01F,
};
constexpr float kInitStarSize[] = {
    90.0F,
    120.0F
};

// 処理に関係
constexpr RECT kTrimmingBackground{       // 背景切り取り範囲
    0L, 720L, 1280L, 1440L };
constexpr RECT kTrimmingEffect{           // 背景エフェクト切り取り範囲
    0L,   0L, 1280L,  720L };
constexpr float kThresholdY = 360.0F;     // プレイヤーの限界Y座標( 絶対座標 )



/*===========================================================================*/
Endless::Endless()
{
}

Endless::~Endless()
{
    // 開放忘れ対応
    if (do_create_ == false)
    {
        destroy();
    }
}

/*===========================================================================*/
// 初期化処理
bool Endless::init()
{
    // 生成処理
    if (do_create_ && create() == false) { return false; }

    // 初期スターの生成
    for (int i = 0; i < 2; ++i)
    {
        if (star_container_->addStar(
            kInitStarPosi[i],
            kInitStarAngle[i],
            kInitStarSpin[i],
            kInitStarSpinRate[i],
            kInitStarSize[i]
        ) == nullptr)
        {
            return false;
        }
    }


    // プレイヤー初期化
    Vector2 position;
    float jump;
    float add_vol;
    float decay;
    float gravity;
    float speed;
    float up_boost;
    float rl_boost;
    FILE* player_state = nullptr;
    errno_t error = fopen_s(&player_state, "State/player_state.txt", "r");
    if (error != 0 || player_state == nullptr) { return false; }
    fscanf_s(player_state,
        "%f %f %f %f %f %f %f %f %f",
        &position.x, &position.y, &jump, &add_vol, &decay, &gravity, &speed,
        &up_boost, &rl_boost);
    fclose(player_state);
    if (player_->init(
        position,jump,add_vol,decay,gravity,speed,rl_boost) == false)
    {
        return false;
    }

    // 背景
    if (background_->init(L"Texture/vector_cosmos.jpg", 10.0F) == false)
    {
        return false;
    }

    // 壁初期化
    if (wall_->init() == false) { return false; }

    // UI初期化
    if (combo_->init(*clock_) == false) { return false; }



    // 変数初期化
    update_ = &Endless::start;
    climb_ = 0.0F;
    prev_player_owner_ = nullptr;
    prev_player_jump_state_ = false;
    magnification_ = 1.0F;

    clock_->start();

    return true;
}
// 生成処理
bool Endless::create()
{
    do_create_ = false;

    // テクスチャ
    TextureLoder* const kTexture = TextureLoder::getInstance();
    texture_ = kTexture->load(L"Texture/プレイ画面.png");
    if (texture_ == nullptr)        { return false; }
    texture_numbers_ = kTexture->load(L"Texture/数字.png");
    if (texture_numbers_ == nullptr){ return false; }

    // 時計
    clock_              = new (std::nothrow) Timer<Milliseconds>();
    if (clock_ == nullptr)          { return false; }

    // タスクマネ－ジャー
    task_manager_       = new (std::nothrow) TaskManager();
    if (task_manager_ == nullptr)   { return false; }

    // 背景
    background_ = new (std::nothrow) Background(task_manager_);
    if (background_ == nullptr) { return false; }

    // スターコンテナ
    star_container_     = new (std::nothrow) StarContainer(task_manager_);
    if (star_container_ == nullptr) { return false; }

    // プレイヤー
    player_             = new (std::nothrow) Player(task_manager_);
    if (player_ == nullptr)         { return false; }

    // 壁
    wall_               = new (std::nothrow) Wall(task_manager_);
    if (wall_ == nullptr)           { return false; }

    // UI
    combo_              = new (std::nothrow) Combo(task_manager_);
    if (combo_ == nullptr)          { return false; }



    // スター生成パターンファイルのリスト化
    FILE* pattern_list = nullptr;
    errno_t error = fopen_s(&pattern_list, "State/pattern_list.txt", "r");
    if (error != 0 || pattern_list == nullptr) { return false; }
    char file_name[FILENAME_MAX];
    while (fscanf_s(pattern_list, "%s", &file_name, FILENAME_MAX) != EOF)
    {
        pattern_file_.push_back(file_name);
    }
    fclose(pattern_list);


    return true;
}

/*===========================================================================*/
// 終了処理
void Endless::destroy()
{
    do_create_ = true;

    //UI
    combo_->destroy();          safe_delete(combo_);

    // 壁
    wall_->destroy();           safe_delete(wall_);

    // プレイヤー
    player_->destroy();         safe_delete(player_);

    // スターコンテナ
    star_container_->destroy(); safe_delete(star_container_);

    // 背景
    background_->destroy();     safe_delete(background_);

    // タスクマネージャー
    safe_delete(task_manager_);

    // 時計
    safe_delete(clock_);

    // テクスチャ
    TextureLoder* const kTexture = TextureLoder::getInstance();
    kTexture->release(texture_);
    kTexture->release(texture_numbers_);
}

/*===========================================================================*/
// 更新関数
SceneBase* Endless::update()
{
    return (this->*update_)();
}

/*===========================================================================*/
// 描画関数
void Endless::draw()
{
    Sprite* const kSprite = Sprite::getInstance();

    // オブジェクト
    task_manager_->allDraw();


    // 背景エフェクト
    kSprite->draw(texture_, Vector2::Zero, &kTrimmingEffect);


    Numbers<int> climb(static_cast<int>(climb_));
    climb.draw(
        texture_numbers_,
        Vector2(1280.0F, 0.0F),
        64L,
        128L
    );
}

/*===========================================================================*/
// スタート部
SceneBase* Endless::start()
{
    // プレイヤーがジャンプしたらplay部へ
    if (player_->isJump())
    {
        update_ = &Endless::play;
        for (auto& star : star_container_->active())
        {
            star->setFall();
        }
        clock_->start();
    }

    // オブジェクト更新
    task_manager_->allUpdate();

    // 星との衝突処理
    for (auto& star : star_container_->active())
    {
        Collision::getInstance()->collision(player_, star);
    }


    return this;
}
// プレイ部
SceneBase* Endless::play()
{
    // 画面外待機している星が無くなったら星の生成
    auto itr = star_container_->active().begin();
    auto end = star_container_->active().end();
    for (; itr != end; ++itr)
    {
        if ((*itr)->getPosition().y < 0.0F) { break; }
    }
    if (itr == end && createStar() == false) { return nullptr; }


    // オブジェクト更新
    task_manager_->allUpdate();
    scoring();

    // プレイヤーが死んでいたらリザルト画面へ
    if (player_->isAlive() == false)
    {
        return new Result;
    }

    // 座標調整( スクロール )
    const float kOver = kThresholdY - player_->getPosition().y;
    if (kOver > 0.0F) { climb_ += kOver; }
    adjustObjectPosition(kOver);

    // オブジェクトの状態倍率を更新
    const float kMagnification = climb_ / 100000.0F + 1.0F;
    if ((kMagnification - magnification_) > 0.1F)
    {
        magnification_ = kMagnification - magnification_ + 1.0F;
        player_->resetStatus(magnification_);
        star_container_->resetStates(magnification_);
    }

    // 衝突処理
    Collision* const kCollision = Collision::getInstance();
    for (auto& star : star_container_->active())
    {
        kCollision->collision(player_, star);
    }
    kCollision->collision(player_, wall_);


    return this;
}

/*===========================================================================*/
// 星の生成
bool Endless::createStar()
{
    // 生成パターンの選択
    if (_chdir("State") == -1) { return false; }
    std::string pattern = pattern_file_[rand() % pattern_file_.size()];
    FILE* file = nullptr;
    errno_t error = fopen_s(&file, pattern.c_str(), "r");
    if (_chdir("../") == -1) { return false; }
    if (error != 0 || file == nullptr) { return false; }


    Star* star;
    Vector2 position;
    float angle;
    float fall;
    float spin;
    float spin_rate;
    float size;

    int end = 0;
    while (true)
    {
        end = fscanf_s(file,
            "%f %f %f %f %f %f %f",
            &position.x, &position.y,
            &angle,
            &fall,
            &spin, &spin_rate,
            &size);
        if (end == EOF) { break; }


        position.y -= 720.0F;
        star = star_container_->addStar(
            position, angle, spin, spin_rate, size);
        if (star == nullptr)
        {
            fclose(file);
            return false;
        }
        star->setFall();
    }


    fclose(file);
    return true;
}

/*===========================================================================*/
// オブジェクトの座標調整
void Endless::adjustObjectPosition(const float Over)
{
    if (Over > 0)
    {
		background_->setMove( Over );

		for( auto& star : star_container_->active() )
			star->setMove( Over );

		player_->setMove( Over );

		wall_->setMove( Over );
    }
}

/*===========================================================================*/
// スコアリング
void Endless::scoring()
{
    ObjectBase* const kPlayerOwner = player_->getOwner();
    const bool kPlayerJump = player_->isJump();

    // 前回の星と違う星に着地したときにコンボ
    if ((kPlayerJump == false && prev_player_jump_state_)&&
        (kPlayerOwner != prev_player_owner_))
    {
        combo_->addCombo();
    }

    if (kPlayerOwner) { prev_player_owner_ = kPlayerOwner; }
    prev_player_jump_state_ = kPlayerJump;
}
