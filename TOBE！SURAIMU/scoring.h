#pragma once
//========================================
//CreatorName:YamadaHaruhisa


#include "score_number.h"
#include "timer.h"

class Scoring
{
public:
	Scoring();
	~Scoring();

	//外部公開関数( 必ず実行 )
public:
	bool init();
	void update();
	void draw();
	void destroy();
	//上昇量加算
	//引数...上昇量
	void addDefaultScore( const double AddScore );
	//スコアリング開始
	inline void start() { scoring_flag_ = true; combo_timer_.start(); }
	inline void stop() { scoring_flag_ = false; }
	inline bool isStart() { return scoring_flag_; }
	//タイマー操作
	inline void timeRestart(){combo_timer_.restart();}
	inline void timeStop(){combo_timer_.stop();}

	//外部公開関数
public:
	//移動コンボ増加
	void addCombo();
	//テクニック加点
	void addTechnique();
	//テクニックコンボのリセット
	void resettechnique();
	//下降加点
	void addDown();
	//回転加点
	//引数...回転数( 度数法 )
	void addRotate( float Angle );
	//回転ステータスリセット
	void resetRotate();
	//距離の加点
	void addLength( const float Length );
	//レベルアップ
	void addLevel();
	inline unsigned int getLevel() { return level_; }
	//スコアの取得
	inline unsigned long long getScore() { return score_; }
	//最大コンボ数の取得
	inline unsigned int getMaxCombo() { return max_combo_; }
	//現在のコンボ数を取得
	unsigned int getCombo();
	//上った距離の取得
	inline double getHeight() { return height_; }
	//プレイヤーの位置を保持
	inline void setPlayerPosition(const Vector2& Posit) { player_position_ = Posit; }
	//プレイヤーのジャンプフラグを格納
	inline void setPlayerJampFlag(const bool Jump) { player_jump_now_flag_ = Jump; }


	//メンバ変数
private:
	unsigned long long score_;	                    //総スコア
	double height_;									//上った距離
	unsigned int combo_;	                        //移動コンボ
	unsigned int max_combo_;						//最大移動コンボ
	unsigned int rotation_combo_;	                //回転コンボ
	unsigned int technique_combo_;					//テクニックコンボ
	float rotation_;	                            //回転量
	bool scoring_flag_;					            //スコアリング有効フラグ
	float combo_circle_scale_;						//コンボ円用拡縮値
	float combo_sprite_scale_;						//コンボ文字用拡縮値
	float combo_alpha_;
	Vector2 player_position_;						//プレイヤーのposition
	bool player_jump_now_flag_;						//プレイヤーがジャンプ中かのフラグ
	bool isexp_now_;								//拡大中かのフラグ
	ID3D11ShaderResourceView* texture_;	            //テクスチャ
	ID3D11ShaderResourceView* num_texture_;
	ID3D11ShaderResourceView* combo_texture_;		//コンボエフェクト用テクスチャ
	ID3D11ShaderResourceView* add_num_texture_;		//数字のテクスチャ
	float spin_combo_pitch_;						//回転コンボのピッチ( コンボがかさむごとに高くなる )
	unsigned int level_;							//ゲームレベル

	double all_height_;								//減衰までの上昇値

	std::list<std::shared_ptr<ScoreNumber>> addition_list_;			//加点リスト
	Timer<Milliseconds> combo_timer_;

private:
	//描画加点の生成
	//引数...描画数値：テクスチャハンドル
	void createNumber( unsigned int Num , ID3D11ShaderResourceView* Handle );
};

