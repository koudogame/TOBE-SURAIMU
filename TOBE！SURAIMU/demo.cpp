
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
#include "star_container.h"
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
constexpr float kAmountOfAlphaForIn  = 0.01F;       // フェードイン時、　アルファ値変化量
constexpr float kAmountOfAlphaForOut = 0.10F;       // フェードアウト時、アルファ値変化量

constexpr int       kInitStarNum = 3;               // シーン開始時に存在するスターの数
constexpr StarState kInitStarState[kInitStarNum] =  // シーン開始時に存在するスターの情報
{
    { {640.0F, 600.0F}, 90.0F, -3.0F, 0.2F,  90.0F },
    { {816.0F, 297.0F}, 90.0F,  3.0F, 0.2F, 120.0F },
    { {468.0F, 142.0F}, 90.0F,  3.0F, 0.2F, 120.0F }
};
constexpr float kScrollThresholdUp   = getWindowHeight<float>() * 0.10F;
constexpr float kScrollThresholdDown = getWindowHeight<float>() * 0.90F;


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
    if( ai_->init( { 640.0F, 650.0F } ) == false ) { return false; }

    // 壁
    wall_ = new Wall();
    if( wall_->init() == false ) { return false; }

    // スター管理コンテナ
    stars_ = new StarContainer();
    setStarPattern();

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

        // 落下を付与
        star->setFall();
    }


    // メンバ初期化
    is_end_ = false;
    is_fadein_ = true;
    alpha_ = 1.0F;

    start_time_ = Clock::now();

    return true;
}

void Demo::destroy()
{
    if( stars_ )
    {
    // 星々の開放
        stars_->destroy();
        safe_delete( stars_ );
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

// スター管理用コンテナのupdateを呼んでいない(AIが落下したとき、着地できるようあえて)
SceneBase* Demo::update()
{
    trance();

    // ゲームパッドのボタンがどれか一つでも押されたらtrueを返すラムダ
    auto pad_any_input = [] ()->bool
    {
        size_t struct_size = 0U;
        char*  ptr_struct  = nullptr;
        {
        // ボタン
        struct_size = sizeof( GamePad::Buttons );
        auto input  = Pad::getInstance()->getState().buttons;
        ptr_struct  = reinterpret_cast<char*>(&input);

        for( size_t i = 0; i < struct_size; ++i )
        {
            if( *(ptr_struct + i) )
            {
                return true;
            }
        }
        }
        {
        // Dpad
            struct_size = sizeof( GamePad::DPad );
            auto input  = Pad::getInstance()->getState().dpad;
            ptr_struct  = reinterpret_cast<char*>(&input);

            for( size_t i = 0; i < struct_size; ++i )
            {
                if( *(ptr_struct + i ) )
                {
                    return true;
                }
            }
        }
        return false;
    };
   

    if( ai_->isAlive() == false ) { is_end_ = true; } 
    if( pad_any_input() )         { is_end_ = true; }


    // 一定時間の経過か、endフラグが立っていて暗転も終わっていたら
    auto elapsed = duration_cast<seconds>(Clock::now() - start_time_).count();
    if( elapsed >= kDemoTimeSc ||
        (is_end_ && alpha_ >= 1.0F ))
    {
    // タイトルへ
        return new Title;
    }

    if( isCreateStar() )
    {
    // スターを生成する
        if( createStar() == false ) { return nullptr; } 
    }
    
    // スクロール処理
    scroll();

    // 衝突処理
    Space::getInstance()->collision();

    return this;
}

void Demo::draw()
{
    Sprite::getInstance()->draw(
        texture_,
        Vector2::Zero,
        &kRangeOfScreen,
        alpha_,
        1.0F
    );
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
        pattern_file = file.getString( 0, i );
        if( wcscmp(pattern_file.c_str(), L"") == 0 )
        {
        // ファイル終端
            break;
        }

        pattern_file.insert( 0, L"State/" );    // ファイルパスへの階層追加
        stars_->addPattern( pattern_file );
    }
}

// スターを生成する
bool Demo::createStar()
{
    return stars_->createStar();
}
