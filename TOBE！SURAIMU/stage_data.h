
// 板場

#pragma once

#include "csvLoader.h"

// ステージデータ格納構造体
//
// load関数でデータの読み込みを行う
// load関数戻り値[ true : 読み込み成功　false : 読み込み失敗 ]
//
// データのフォーマット !行と列が反転しているので、くれぐれも注視すること!
// 　   0    (: 行)
// 0    プレイヤー初期座標(x)
// 1    プレイヤー初期座標(y)
// 2    スタートライン(y座標)
// 3    ゴールライン(y座標)
// 4    炎上昇スピード
// 5    Sランククリアタイム(秒)
// 6    Aランククリアタイム(秒)
// 7    ランククリアタイム(秒)
// 8    スター生成パターンファイル名
//(: 列)

struct StageData
{
    bool load( const std::wstring& FileName )
    {
        CsvLoader data;

        if( data.load( FileName ) == false ) { return false; }

        // プレイヤーの初期座標を取得
        player_position.x = data.getNumber_f( 0U, 0U );
        player_position.y = data.getNumber_f( 1U, 0U );

        // スタートライン
        start_line = data.getNumber_f( 2U, 0U );
        // ゴールライン
        goal_line = data.getNumber_f( 3U, 0U );
        // ステージ長さ
        height = start_line - goal_line;

        // 炎上昇スピード
        fire_speed = data.getNumber_f( 4U, 0U );

        // ランク別クリアタイム
        s_rank_clear_sc = data.getNumber( 5U, 0U );
        a_rank_clear_sc = data.getNumber( 6U, 0U );
        b_rank_clear_sc = data.getNumber( 7U, 0U );

        // スター生成パターンファイル名
        star_pattern_file_name = data.getString( 8U, 0U );

        return true;
    }

    Vector2 player_position;    // プレイヤー初期座標

    float start_line;           // スタートライン
    float goal_line;            // ゴールライン
    float height;               // ステージ長さ

    float fire_speed;           // 炎スピード

    int s_rank_clear_sc = 0;    // Sランククリアタイム
    int a_rank_clear_sc = 0;    // Aランククリアタイム
    int b_rank_clear_sc = 0;    // Bランククリアタイム

    std::wstring star_pattern_file_name;    // スター生成パターンファイル名
};
