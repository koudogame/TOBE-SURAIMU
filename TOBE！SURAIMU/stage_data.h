
// 板場

#pragma once

#include "csvLoader.h"

// ステージデータ格納構造体
//
// load関数でデータの読み込みを行う
// load関数戻り値[ true : 読み込み成功　false : 読み込み失敗 ]
//
// データのフォーマット
// 　0
// 0 Sランククリアタイム( 秒 )
// 1 Aランククリアタイム( 秒 )
// 2 Bランククリアタイム( 秒 )
// 3 ファイアスピード
// 4 スター生成パターンファイル名

struct StageData
{
    bool load( const std::wstring& FileName )
    {
        CsvLoader data;

        if( data.load( FileName ) == false ) { return false; }

        s_rank_clear_sc = data.getNumber( 0U, 0U );
        a_rank_clear_sc = data.getNumber( 0U, 1U );
        b_rank_clear_sc = data.getNumber( 0U, 2U );

        fire_speed_ = data.getNumber_f( 0U, 3U );

        star_pattern_file_name = data.getString( 0U, 4U );


        return true;
    }

    int s_rank_clear_sc = 0;
    int a_rank_clear_sc = 0;
    int b_rank_clear_sc = 0;

    float fire_speed_ = 0.0F;

    std::wstring star_pattern_file_name;
};
