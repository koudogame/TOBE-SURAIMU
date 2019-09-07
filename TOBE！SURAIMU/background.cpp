
// 板場

#include "background.h"

#include "textureLoder.h"
#include "task_manager.h"
#include "view_mist.h"
#include "view_star_mini.h"
#include "view_star_big.h"
#include "view_wave.h"




// テクスチャサイズが大きい画像を初期化時、同時に読み込んでおく
// *このクラスはテクスチャの開放を行っていない。TextureLoderクラスのデストラクタにて
//  自動で開放されることを前提に処理を行っている
/*===========================================================================*/
const wchar_t* const kTextureFileName[]
{
    L"Texture/back_wave_purple.png",    // kPurple
    L"Texture/back_wave_blue.png",      // kBule
    L"Texture/back_wave_red.png",       // kRed
};
namespace
{
    class Loader
    {
    public:
        Loader()
        {
            TextureLoder* texture_loader = TextureLoder::getInstance();

            for (auto& file : kTextureFileName)
            {
                texture_loader->load(file);
            }
        }
    };
}

constexpr Vector2 kViewPositionInit { 190.0F, -900.0F };
constexpr float kViewOffsetY = 600.0F;
constexpr float kThresholdCreateViewY = -300.0F;
constexpr Vector2 kWavePositionInit { 0.0F, -1200.0F };

constexpr float kCreateLine = 1000.0F;

constexpr int kWaveMaxNum = 4;
constexpr int kWaveCreateDenominator = 500;


/*===========================================================================*/
Background::Background()
{

}

Background::~Background()
{

}


// override
/*===========================================================================*/
// 初期化処理
bool Background::init()
{
    static ::Loader load_texture;   /*サイズの大きい画像を読み込んでおく*/

    destroy();

    // タスク登録
    TaskManager* task_manager = TaskManager::getInstance();
    task_manager->registerTask( this, TaskUpdate::kBackground );
    task_manager->registerTask( this, TaskDraw::kBackground );


    // 背景を画面いっぱいになるまで生成する( 下から生成 )
    ViewMist     *mist      = nullptr;
    ViewStarMini *mini_star = nullptr;
    ViewStarBig  *big_star  = nullptr;

    Vector2 create_position = kViewPositionInit;
    create_position.y = kCreateLine;
    for( ; create_position.y >= kViewPositionInit.y; create_position.y -= kViewOffsetY )
    {
        // 霧
        mist = new ViewMist();
        if( mist->init( create_position, 
                        BackObjectBase::Color::kPurple ) == false ) 
        {
            return false;
        }
        mist_list_.push_back( mist );

        // 小さい星
        mini_star = new ViewStarMini();
        if( mini_star->init( create_position,
                             BackObjectBase::Color::kPurple ) == false )
        {
            return false;
        }
        mini_star_list_.push_back( mini_star );

        // 大きい星
        big_star = new ViewStarBig();
        if( big_star->init( create_position,
                            BackObjectBase::Color::kPurple ) == false )
        {
            return false;
        }
        big_star_list_.push_back( big_star );
    }


    // その他のメンバ初期化
    color_ = BackObjectBase::Color::kPurple;
    fail_wall_ = nullptr;


    return true;
}
// 終了処理
void Background::destroy()
{
    // リストのオブジェクトを開放する処理
    auto destroyList = []( auto *List )
    {
        for( auto& object : *List )
        {
            object->destroy();
            delete object;
        }
        List->clear();
    };

    destroyList( &mist_list_ );
    destroyList( &mist_free_ );

    destroyList( &mini_star_list_ );
    destroyList( &mini_star_free_ );

    destroyList( &big_star_list_ );
    destroyList( &big_star_free_ );

    destroyList( &wave_list_ );
    destroyList( &wave_free_ );

    // タスク解除
    TaskManager::getInstance()->unregisterObject( this );
}
// 更新処理
void Background::update()
{

    // 背景の更新
    updateView( &mist_list_, &mist_free_ );
    updateView( &mini_star_list_, &mini_star_free_ );
    updateView( &big_star_list_, &big_star_free_ );


    // 波の更新
    updateWave();
}
// 描画処理
void Background::draw()
{
    // 描画用ラムダ
    auto drawObject = []( auto& List )
    {
        for( auto& object : List )
        {
            object->draw();
        }
    };

    drawObject( mist_list_ );
    drawObject( mini_star_list_ );
    drawObject( big_star_list_ );
    drawObject( wave_list_ );
}


/*===========================================================================*/
void Background::setMove( const float Offset ) 
{
    // 移動用ラムダ
    auto moveObject = [=]( auto& List )
    {
        for( auto& object : List )
        {
            object->setMove( Offset );
        }
    };

    moveObject( mist_list_ );
    moveObject( mini_star_list_ );
    moveObject( big_star_list_ );
    moveObject( wave_list_ );
}

void Background::changeColor()
{
    color_ = color_ != BackObjectBase::Color::kRed ?
    static_cast<BackObjectBase::Color>(static_cast<int>(color_) + 1) :
    BackObjectBase::Color::kPurple;
}

void Background::reset()
{
    init();
}

// 背景( 波以外 )
/*==========================================================================*/
// リストのオブジェクトを更新、必要があれば削除、追加を行う *波以外専用*
template <typename T>
bool Background::updateView( std::vector<T*> *List, std::vector<T*> *Free )
{
    // リストのオブジェクトに更新をかける
    for( auto& object : *List )
    {
        object->setFailWall( fail_wall_ );
        object->update();
    }

    // 先頭のオブジェクトは、死んでいる可能性があるので判定
    T* top_object = *List->begin();
    if( top_object->isAlive() == false )
    {
        List->erase( List->begin() );
        Free->push_back( top_object );
    }

    // 最後のオブジェクトが、オフセット分進んだら新しく生成
    if( List->size() > 0 )
    {
        Vector2 last_position = List->back()->getPosition();
        if( last_position.y > kThresholdCreateViewY )
        {
            T* object = nullptr;

            if( Free->size() > 0 )
            {
                object = Free->back();
                Free->pop_back();
            }
            else
            {
                object = new T();
            }


            if( object->init( kViewPositionInit, color_ ) == false ) {return false;}

            List->push_back( object );
        }
    }
    

    return true;
}

// 背景( 波 )
/*==========================================================================*/
// 波の生成を行うか判定
bool Background::isCreateWave() const
{
    // アクティブな波としてカウントする範囲
    static const float kJudgeRangeTop = -1200;
    static const float kJudgeRangeBottom = 0.0F;
    int wave_sum = 0;
    for( auto& wave : wave_list_ )
    {
        if( wave->getPosition().y >= kJudgeRangeTop &&
            wave->getPosition().y <= kJudgeRangeBottom )
        {
            ++wave_sum;
        }
    }


    // アクティブな波が一つもなかったら生成
    if( wave_sum == 0 ) { return true; }

    // アクティブな波が一定の値より少ない場合
    // 一定の確率で生成
    if( wave_sum < kWaveMaxNum &&
        !(rand() % kWaveCreateDenominator)
       )
    {
        return true;
    }


    return false;
}
// リストのオブジェクトを更新、必要があれば削除、追加を行う 
bool Background::updateWave()
{
    // リストのオブジェクトに更新をかける
    for( auto& wave : wave_list_ )
    {
        wave->setFailWall( fail_wall_ );
        wave->update();
    }


    // 先頭のオブジェクトは、死んでいる可能性があるので判定する
    if( wave_list_.size() > 0 )
    {
        ViewWave *top_wave = *wave_list_.begin();
        if( top_wave->isAlive() == false )
        {
            wave_list_.erase(wave_list_.begin());
            wave_free_.push_back(top_wave);
        }
    }


    // 波の生成処理
    if( isCreateWave() )
    {
        ViewWave *wave = nullptr;

        if( wave_free_.size() > 0 )
        {
            wave = wave_free_.back();
            wave_free_.pop_back();
        }
        else
        {
            wave = new ViewWave();
        }

        if( wave->init( kWavePositionInit, color_ ) == false ) { return false; }

        wave_list_.push_back( wave );
    }


    return true;
}
