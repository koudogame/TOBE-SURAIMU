#include "textureLoder.h"
#include "texture.h"

//コンテナの作成
Container* TextureLoder::creatContainer(const std::wstring& FileName)
{
	return new Container(Texture::getInstance()->load(FileName));
}

//デストラクタ
TextureLoder::~TextureLoder()
{
	//マップを空にする
	container_.clear();
}

//テクスチャの解放
void TextureLoder::release(ID3D11ShaderResourceView* Texture)
{
	for (auto itr = container_.begin(); itr != container_.end(); itr++)
		//テクスチャハンドルが存在した場合
		if ((*itr).second.get()->getTexture() == Texture)
			//参照数の減少 ( 0以下の場合 )
			if ((*itr).second.get()->defRef())
			{
				//コンテナの削除
				container_.erase(itr);
				break;
			}
}

//テクスチャの読み込み
ID3D11ShaderResourceView * TextureLoder::load(const std::wstring& FileName)
{
	//コンテナの検索
	auto it = container_.find(FileName);

	//すでにテクスチャの読み込みがされていた場合
	if (it != container_.end())
	{
		//参照数を加算
		it->second.get()->addRef();
		return it->second.get()->getTexture();
	}

	//テクスチャ新規読み込み
	Container* p_container;

  	p_container = creatContainer(FileName);

	if (!p_container)
		return nullptr;

	//コンテナ追加
	container_.insert(std::make_pair(FileName, std::move(p_container)));
	//参照数を加算
	p_container->addRef();
	return p_container->getTexture();
}
