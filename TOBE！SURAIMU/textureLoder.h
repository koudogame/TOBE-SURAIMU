#pragma once
#include "container.h"

//********************************************
//Class
//********************************************
class TextureLoder
{
private:
	//コンストラクタ( デフォルト )
	TextureLoder() = default;
	std::unordered_map<std::wstring, std::unique_ptr<Container>> container_;		//フライウェイトマップ
	//コンテナの作成
	//引数:ファイル名　戻り値:コンテナ
	Container* creatContainer(const std::wstring& FileName);

public:
	//自身の実体の取得
	static TextureLoder* getInstance() { static TextureLoder instance; return &instance; }
	//デストラクタ
	~TextureLoder();
	//テクスチャの解放
	//引数:テクスチャハンドル
	void release(ID3D11ShaderResourceView* Texture);
	//テクスチャの読み込み
	//引数:ファイル名( ワイド文字 )　戻り値:テクスチャハンドル
	ID3D11ShaderResourceView* load(const std::wstring& FileName);
};