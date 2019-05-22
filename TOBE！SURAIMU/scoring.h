#pragma once

class Scoring
{
public:
	Scoring();
	~Scoring();

	//外部公開関数( 必ず実行 )
public:
	bool init();
	void draw();
	void addDefaultScore( float AddScore );

	//外部公開関数
public:
	//移動コンボ増加
	void addCombo();
	//移動コンボリセット
	void resetCombo();
	//テクニック加点
	void addtechnique();
	//下降加点
	void addDown();
	//回転加点
	//引数...回転数( 度数法 )
	void addRotate( float Angle );
	//回転ステータスリセット
	void resetRotate();

	//メンバ変数
private:
	long long score_;
	int combo_;
	int rotation_combo_;
	float rotation_;
	ID3D11ShaderResourceView* texture_;
};

