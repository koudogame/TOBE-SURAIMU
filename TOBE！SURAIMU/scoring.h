#pragma once
//========================================
//CreatorName:YamadaHaruhisa

#include "numbers.h"
#include "audio_loader.h"

class Scoring
{
public:
	Scoring();
	~Scoring();

	//外部公開関数( 必ず実行 )
public:
	bool init();
	void draw();
	void destroy();
	//上昇量加算
	//引数...上昇量
	void addDefaultScore( const double AddScore );
	//スコアリング開始
	inline void start() { scoring_flag_ = true; }
	inline bool isStart() { return scoring_flag_; }

	//外部公開関数
public:
	//移動コンボ増加
	void addCombo();
	//移動コンボリセット
	void resetCombo();
	//テクニック加点
	void addTechnique();
	//下降加点
	void addDown();
	//回転加点
	//引数...回転数( 度数法 )
	void addRotate( float Angle );
	//回転ステータスリセット
	void resetRotate();
	//距離の加点
	void addLength( const float Length );
	//スコアの取得
	inline unsigned long long getScore() { return score_; }
	//最大コンボ数の取得
	inline unsigned int getMaxCombo() { return max_combo_; }
	//上った距離の取得
	inline double getHeight() { return height_; }

	//メンバ変数
private:
	unsigned long long score_;	                    //総スコア
	double height_;									//上った距離
	unsigned int combo_;	                        //移動コンボ
	unsigned int max_combo_;						//最大移動コンボ
	unsigned int rotation_combo_;	                //回転コンボ
	float rotation_;	                            //回転量
	bool scoring_flag_;					            //スコアリング有効フラグ
	ID3D11ShaderResourceView* texture_;	            //テクスチャ
	float spin_combo_pitch_;						//回転コンボのピッチ( コンボがかさむごとに高くなる )

	AudioContainer* combo_sound_;

	//仮描画
	Numbers<unsigned long long> sco_;
	Numbers<unsigned int> move_com_;
	Numbers<unsigned int> rota_com_;

};

