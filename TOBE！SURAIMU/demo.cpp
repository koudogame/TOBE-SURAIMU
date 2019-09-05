
// 板場　温樹

#include "demo.h"

// ライブラリ的
#include "release.h"
#include "sprite.h"
#include "textureLoder.h"
#include "csvLoader.h"
// 入力
#include "key.h"
#include "pad.h"
// オブジェクト
#include "task_manager.h"
#include "space.h"
#include "ai_demo.h"
#include "wall.h"
#include "fail_wall.h"
#include "star_container.h"
#include "progress.h"
// 遷移先
#include "title.h"

using namespace std::chrono;
using Clock = high_resolution_clock;

bool Demo::is_end_ = true;

/*===========================================================================*/
struct StarState
{
    constexpr StarState( 
        const Vector2& Position,
        const float    Angle,
        const float    Spin,
        const float    SpinRate,
        const float    Size
    ) : position  ( Position ),
        angle     ( Angle ),
        spin      ( Spin ),
        spin_rate ( SpinRate ),
        size      ( Size )
    {}


    Vector2 position;
    float   angle;
    float   spin;
    float   spin_rate;
    float   size;
};

constexpr long long kStartTimeSc    = 2LL;  // 開始時、暗転時間(単位 : 秒)
constexpr long long kDemoPlayTimeSc = 30LL; // デモプレイ時間　(単位 : 秒)
constexpr long long kEndTimeSc      = 1LL;  // 終了時、暗転時間(単位 : 秒)
constexpr long long kDemoTimeSc = kStartTimeSc + kDemoPlayTimeSc + kEndTimeSc;  // シーンの合計時間
const RECT kRangeOfScreen { 0L, 0L, 1280L, 720L };
constexpr float kDepth = 100.0F;
constexpr float kAmountOfAlphaForIn  = 0.01F;       // フェードイン時、　アルファ値変化量
constexpr float kAmountOfAlphaForOut = 0.10F;       // フェードアウト時、アルファ値変化量

constexpr int       kInitStarNum = 3;               // シーン開始時に存在するスターの数
constexpr StarState kInitStarState[kInitStarNum] =  // シーン開始時に存在するスターの情報
{
    { {640.0F, 600.0F}, 90.0F, -3.0F, 0.2F,  80.0F },
    { {816.0F, 297.0F}, 90.0F,  3.0F, 0.2F, 100.0F },
    { {468.0F, 142.0F}, 90.0F,  3.0F, 0.2F, 100.0F }
};
constexpr float kScrollThresholdUp   = getWindowHeight<float>() * 0.10F;
constexpr float kScrollThresholdDown = getWindowHeight<float>() * 0.90F;

constexpr Vector2 kInitPlayerPosition { 600.0F, 565.0F };

constexpr float kStageHeight = 7200.0F;

/*===========================================================================*/
Demo::Demo()
{

}

Demo::~Demo()
{
    destroy();
}


/*===========================================================================*/
bool Demo::init()
{
    destroy();

    // テクスチャ
    texture_ = TextureLoder::getInstance()->load( L"Texture/black.png" );
    if( texture_ == nullptr ) { return false; }

    // Demo用AI
    ai_ = new AIDemo();
    if( ai_->init( kInitPlayerPosition ) == false ) { return false; }

    // 壁
    wall_ = new Wall();
    if( wall_->init() == false ) { return false; }

    // 炎
    fail_wall_ = new FailWall();
    if( fail_wall_->init() == false ) { return false; }

    // スター管理コンテナ
    stars_ = new StarContainer();
    setStarPattern();

    // 進行度
    progress_ = new Progress();
    if( progress_->init(kStageHeight, ai_, fail_wall_) == false ) { return false; } 

    for( int i = 0; i < kInitStarNum; ++i )
    {
    // 初期スターの追加
        Star* star = stars_->addStar(
            kInitStarState[i].position,
            kInitStarState[i].angle,
            kInitStarState[i].spin,
            kInitStarState[i].spin_rate,
            kInitStarState[i].size
        );

        if( star == nullptr )
        {
        // エラー
            return false;
        }
    }


    // メンバ初期化
    is_end_ = false;
    update_ = &Demo::start;
    is_fadein_ = true;
    alpha_ = 1.0F;

    start_time_ = Clock::now();

    return true;
}

void Demo::destroy()
{
    if( progress_ )
    {
    // 進行度の開放
        progress_->destroy();
        safe_delete( progress_ );
    }

    if( stars_ )
    {
    // 星々の開放
        stars_->destroy();
        safe_delete( stars_ );
    }

    if( fail_wall_ )
    {
    // 炎の開放
        fail_wall_->destroy();
        safe_delete( fail_wall_ );
    }

    if( wall_ )
    {
    // 壁の開放
        wall_->destroy();
        safe_delete( wall_ );
    }

    if( ai_ )
    {
    // デモ用AI開放
        ai_->destroy();
        safe_delete( ai_ );
    }

    if( texture_ )
    {
    // テクスチャ開放
        TextureLoder::getInstance()->release( texture_ );
        texture_ = nullptr;
    }
}


SceneBase* Demo::update()
{
    trance();

    // ゲームパッドのボタンがどれか一つでも押されたらtrueを返すラムダ
    auto padAnyInput = [] ()->bool
    {
        // 構造体(要素がchar型)の要素がどれか一つでもtrueだったらtrueを返すラムダ
        auto isAnyTrue = [](void* const PtrToStructure, const size_t Size)->bool
        {
            char* idx = reinterpret_cast<char*>(PtrToStructure);

            for( size_t i = 0U; i < Size; ++i )
            {
            // 構造体の先頭から順に見ていく
                if( *(idx + i ) )
                {
                    return true;
                }
            }
            return false;
        };


        auto button = Pad::getInstance()->getState().buttons;
        auto dpad = Pad::getInstance()->getState().dpad;

        return isAnyTrue( &button, sizeof( button ) ) ||
               isAnyTrue( &dpad,   sizeof( dpad ) );
    };
    if (padAnyInput()) { is_end_ = true; }
   

    return (this->*update_)();
}

void Demo::draw()
{
    Sprite::getInstance()->reserveDraw(
        texture_,
        Vector2::Zero,
        kRangeOfScreen,
        alpha_,
        kDepth
    );
}
// スタート
SceneBase* Demo::start()
{
    if( ai_->Player::isJump() )
    {
        update_ = &Demo::play;
        stars_->setFall();
        ai_->onStartFlag();
        fail_wall_->start();
    }

    // 衝突処理
    Space::getInstance()->collision();

    return this;
}
// プレイ
SceneBase* Demo::play()
{
    if (ai_->isAlive() == false) { is_end_ = true; }


    // 一定時間の経過か、endフラグが立っていて暗転も終わっていたら
    auto elapsed = duration_cast<seconds>(Clock::now() - start_time_).count();
    if (elapsed >= kDemoTimeSc ||
        (is_end_ && alpha_ >= 1.0F))
    {
        // タイトルへ
        return new Title;
    }


    stars_->update();
    if (isCreateStar())
    {
        // スターを生成する
        if (createStar() == false) { return nullptr; }
        stars_->setFall();
    }

    // スクロール処理
    scroll();

    // 衝突処理
    Space::getInstance()->collision();

    return this;
}


/*===========================================================================*/
// 画面遷移演出
void Demo::trance()
{
    auto elapsed = duration_cast<seconds>(Clock::now() - start_time_).count();

    if( is_fadein_ && elapsed > kStartTimeSc )
    {
    // フェードイン
        alpha_ -= kAmountOfAlphaForIn;
        if( alpha_ < 0.0F )
        {
            alpha_ = 0.0F;
            is_fadein_ = false;
        }
    }
    if( (!is_fadein_ && (elapsed >= kStartTimeSc + kDemoPlayTimeSc)) || is_end_ )
    {
    // フェードアウト
        alpha_ += kAmountOfAlphaForOut;
        if( alpha_ > 1.0F )
        {
            alpha_ = 1.0F;
        }
    }
}


/*===========================================================================*/
// スクロール
void Demo::scroll()
{
    float scroll = 0.0F;

    const Vector2& kBasePosition = ai_->getPosition();

    if( kBasePosition.y < kScrollThresholdUp )
    {
    // 下にスクロール
        scroll = kScrollThresholdUp - kBasePosition.y;
    }
    else if( kBasePosition.y > kScrollThresholdDown )
    {
    // 上にスクロール
        scroll = kScrollThresholdDown - kBasePosition.y ;
    }

    TaskManager::getInstance()->allSetOver( scroll );
}

/*===========================================================================*/
// 画面外(上方向)にスターがなかったらtrue
bool Demo::isCreateStar()
{
    // 後ろから見たほうが、画面外にあるスターを早い段階で見つけられる(かも...)
    auto itr = stars_->active().rbegin();
    auto end = stars_->active().rend();
    for( ; itr != end; ++itr )
    {
    // 画面外( 上方向 )のスターがあったらループ抜ける
        if( (*itr)->getPosition().y < 0.0F )
        {
            break;
        }
    }

    return itr == end;
}

// スターの生成パターンファイルを設定する
void Demo::setStarPattern()
{
    stars_->resetPattern();

    std::wstring pattern_file;
    CsvLoader file( L"State/pattern_list.csv" );
    for( int i = 0;; ++i )
    {
    // ファイルに保存されているパターンファイル名を追加していく
        pattern_file = file.getString( i, 0 );
        if( wcscmp(pattern_file.c_str(), L"") == 0 )
        {
        // ファイル終端
            break;
        }

        pattern_file.insert( 0, L"State/" );    // ファイルパスの階層追加
        stars_->addPattern( pattern_file );
    }
}

// スターを生成する
bool Demo::createStar()
{
    return stars_->createStar();
}
