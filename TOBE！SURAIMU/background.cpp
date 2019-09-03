
// 板場

#include "background.h"

#include "textureLoder.h"
#include "task_manager.h"
#include "view_mist.h"
#include "view_star_mini.h"
#include "view_star_big.h"
#include "view_wave.h"




// テクスチャサイズが大きい画像を初期化時、同時に読み込んでおく
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

constexpr float kViewDeathLine = 1000.0F;


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
    create_position.y = kViewDeathLine - kViewOffsetY;
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
    //updateWave();
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
    // 下に入力してゲームオーバーになった後に、もう一度やり直すと下に背景がないので虚無になる
    // 後、初期化時にかなり下までオブジェクトを生成すると、なぜか重なる

    //// カラーセット用ラムダ
    //auto setColorObject = [this]( auto& List )
    //{
    //    for( auto& elem : List )
    //    {
    //        elem->setColor( color_ );
    //    }
    //};

    //color_ = BackObjectBase::Color::kPurple;

    //setColorObject( mist_list_ );
    //setColorObject( mini_star_list_ );
    //setColorObject( big_star_list_ );
    //setColorObject( wave_list_ );


    init();
}

// リスト関係
/*==========================================================================*/
// リストのオブジェクトを更新、必要があれば削除、追加を行う
template <typename T>
bool Background::updateView( std::vector<T*> *List, std::vector<T*> *Free )
{
    // リストのオブジェクトに更新をかける
    for( auto& object : *List )
    {
        object->update();
    }

    // 先頭のオブジェクトは、画面外の可能性があるので判定
    T* top_object = *List->begin();
    if( top_object->getPosition().y > kViewDeathLine )
    {
        List->erase( List->begin() );
        Free->push_back( top_object );
    }

    // 最後のオブジェクトが、オフセット分進んだら新しく生成
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


        if( object->init(kViewPositionInit, color_) == false ) {return false;}

        List->push_back( object );
    }


    return true;
}
