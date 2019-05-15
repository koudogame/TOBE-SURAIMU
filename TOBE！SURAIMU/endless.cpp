
// 板場 温樹

#include "endless.h"

#include "release.h"
#include "textureLoder.h"
#include "sprite.h"
#include "task_manager.h"
#include "star_container.h"
#include "player.h"
#include "wall.h"


/*===========================================================================*/
// 処理に関係
constexpr RECT kTrimmingBackground{       // 背景切り取り範囲
    0L, 720L, 1280L, 1440L };
constexpr RECT kTrimmingEffect{           // 背景エフェクト切り取り範囲
    0L,   0L, 1280L,  720L };



/*===========================================================================*/
Endless::Endless() :
    SceneBase(SceneID::kPlay)
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

    // 初期化処理
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
    if (error != 0) { return false; }
    fscanf_s(player_state,
        "%f %f %f %f %f %f %f %f %f",
        &position.x, &position.y, &jump, &add_vol,&decay, &gravity, &speed,
        &up_boost, &rl_boost);
    fclose(player_state);
    if (player_->init(
        position,jump,add_vol,decay,gravity,speed,up_boost,rl_boost) == false)
    {
        return false;
    }

    if (wall_->init() == false) { return false; }

    return true;
}
// 生成処理
bool Endless::create()
{
    do_create_ = false;

    // テクスチャ
    texture_ = TextureLoder::getInstance()->load(L"Texture/プレイ画面.png");
    if (texture_ == nullptr)        { return false; }

    // タスクマネ－ジャー
    task_manager_       = new (std::nothrow) TaskManager();
    if (task_manager_ == nullptr)   { return false; }

    // スターコンテナ
    star_container_     = new (std::nothrow) StarContainer(task_manager_);
    if (star_container_ == nullptr) { return false; }

    // プレイヤー
    player_             = new (std::nothrow) Player(task_manager_);
    if (player_ == nullptr)         { return false; }

    // 壁
    wall_               = new (std::nothrow) Wall(task_manager_);
    if (wall_ == nullptr)           { return false; }





    // スター生成パターンファイルのリスト化
    FILE* pattern_list;
    errno_t error = fopen_s(&pattern_list, "State/pattern_list.txt", "r");
    if (error != 0) { return false; }
    char file_name[FILENAME_MAX];
    while (fscanf_s(pattern_list, "%s", file_name, FILENAME_MAX) != EOF)
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

    // 壁
    wall_->destroy();           safe_delete(wall_);

    // プレイヤー
    player_->destroy();         safe_delete(player_);

    // スターコンテナ
    star_container_->destroy(); safe_delete(star_container_);

    // タスクマネージャー
    safe_delete(task_manager_);

    // テクスチャ
    TextureLoder::getInstance()->release(texture_);
}

/*===========================================================================*/
// 更新関数
SceneBase* Endless::update()
{
    task_manager_->allUpdate();

    return this;
}

/*===========================================================================*/
// 描画関数
void Endless::draw()
{
    Sprite* const kSprite = Sprite::getInstance();

    // 背景
    kSprite->draw(texture_, Vector2::Zero, &kTrimmingBackground);
    kSprite->draw(texture_, Vector2::Zero, &kTrimmingEffect);


    // オブジェクト
    task_manager_->allDraw();
}

/*===========================================================================*/
// 