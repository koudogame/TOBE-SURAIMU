#pragma once


//  class
class Direct3D
{
private:
	Direct3D() = default;
    //  インターフェイスの宣言
    ID3D11Device* d3d_device_;              //  D3Dデバイスインターフェイス
    ID3D11DeviceContext* device_context_;   //  デバイスコンテキストインターフェイス
    IDXGISwapChain* swap_chain_;            //  スワップチェーンインターフェイス
    D3D_FEATURE_LEVEL feature_level_;       //  機能レベル
    ID3D11RenderTargetView* render_target_view_; //  描画ターゲットビュー

public:
	static Direct3D* getInstance() { static Direct3D instance; return &instance; }
    bool init( const HWND hWnd );    //  初期化
    void destroy();                  //  破棄
    void clear();                    //  描画クリア
    void present();                  //  画面更新

    //  インターフェイスの取得
    ID3D11Device* getDevice()            { return d3d_device_; }
    ID3D11DeviceContext* getContext()    { return device_context_; }
    IDXGISwapChain* getChain()           { return swap_chain_; }
};