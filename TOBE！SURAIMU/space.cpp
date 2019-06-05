
// 板場 温樹

#include "space.h"

#include "object_base.h"
#include "collision.h"


/*===========================================================================*/
constexpr unsigned kMask        = 0x03; // マスク用

constexpr RECT kRange{ 305L, -840L, 975L, 1000L };
constexpr unsigned kDivideLevel = 3U;   // 空間分割レベル
constexpr unsigned kDivideNum   = 4U;   // 分割数
const unsigned kBlockNum =              // 空間ブロック数
    (static_cast<unsigned>(std::pow( kDivideNum, kDivideLevel + 1U)) - 1U) /
                          (kDivideNum - 1U );

int toMorton( const RECT& Range, const Vector2& Point );
int bitStep ( short Num );

/*===========================================================================*/
Space::Space() :
    range_(kRange), 
    space_(kBlockNum)
{

}

Space::~Space()
{

}

/*===========================================================================*/
// オブジェクトを空間に登録
void Space::registration( ObjectBase* const Obj,       // オブジェクトのアドレス
                          const Vector2&    Origin,    // 原点
                          const float       Radius )   // 半径
{
    unregistration( Obj );


    // 2頂点から、モートン番号を求める
    const Vector2 kTopLeft    ( Origin.x - Radius, Origin.y - Radius );
    const Vector2 kBottomRight( Origin.x + Radius, Origin.y + Radius );

    if( kTopLeft.x < kRange.left || kTopLeft.y < kRange.top ||
        kBottomRight.x > kRange.right || kBottomRight.y > kRange.bottom )
    {
        // 範囲外オブジェクトは登録しない
        return;
    }

    const int kXorMorton =
        toMorton( range_, kTopLeft ) ^ toMorton( range_, kBottomRight );


    // 所属空間の空間レベルを求める
    int level = kDivideLevel;
    if( kXorMorton != 0 )
    {
        level = 0;

        for( int i = kDivideLevel - 1; i > 0; ++i )
        {
            // 所属空間が判明
            if( kXorMorton | ( kMask << (i * 2) ) )
            {
                break;
            }
            // 空間レベルを子へ
            else
            {
                ++level;
            }
        }
    }


    // 空間へ登録
    int block = (static_cast<unsigned>(std::pow(kDivideNum, level)) - 1U ) / 
                (kDivideNum - 1U);
    block += toMorton(range_, kTopLeft) >> ((kDivideLevel - level) * 2);

    space_[block].push_back( Obj );
}

/*===========================================================================*/
// オブジェクトを空間から登録解除
void Space::unregistration( ObjectBase* const Obj )
{
    auto object = object_list_.find( Obj );
    if( object != object_list_.end() )
    {
        object->second->remove(Obj);
        object_list_.erase(Obj);
    }
}

/*===========================================================================*/
// 登録されているオブジェクトの全衝突判定
void Space::collision()
{
    Collision* kCollision = Collision::getInstance();

    // 空間を走査
    for( int child = kBlockNum - 1, parent = 0; child >= 0; --child )
    {
        // ルート空間までの親と衝突判定
        for( parent = child; ; parent >>= 2 )
        {
            // 判定処理
            for( auto child_obj : space_[child] ){
                for( auto parent_obj : space_[parent] )
                {
                    // TODO : 衝突の検出
                }
            }

            // ルート空間所属のオブジェクトとの判定を終えたら次の空間へ
            if( parent == 0 ) { break; }
        }
    }
}


/*===========================================================================*/
// 引数の座標を引数の範囲内のモートン番号へ
int toMorton( const RECT& Range, const Vector2& Point )
{
    const int kLineNum = static_cast<int>(std::pow(2, kDivideLevel));

    const long kRangeWidth  = Range.right - Range.left;
    const long kRangeHeight = Range.bottom - Range.top;


    // 範囲内でのモートン番号を求める
    const int kMortonX = static_cast<int>(
            (Point.x - Range.left)) / (kRangeWidth  / kLineNum);
    const int kMortonY = static_cast<int>(
            (Point.y - Range.top )) / (kRangeHeight / kLineNum);


    return bitStep(kMortonX) | (bitStep(kMortonY) << 1);
}
// 引数の数字を1ビット飛ばしにした数値を返却
int bitStep( short Num )
{
    Num = (Num | (Num << 8)) & 0x00ff00ff;
    Num = (Num | (Num << 4)) & 0x0f0f0f0f;
    Num = (Num | (Num << 2)) & 0x33333333;
    Num = (Num | (Num << 1)) & 0x55555555;

    return Num;
}