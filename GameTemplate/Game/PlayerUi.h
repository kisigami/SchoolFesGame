#pragma once

class Player;   //プレイヤークラス

/// <summary>
/// プレイヤーUIクラス
/// </summary>
class PlayerUi:public IGameObject
{
public:
	PlayerUi();
	~PlayerUi();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// HPバーの拡大処理
	/// </summary>
	void HpBarScale();

	Player*      m_player = nullptr;    //プレイヤー
	SpriteRender m_hpBarRender;         //HPバーレンダー
	SpriteRender m_hpBarFrameRender;    //HPバーの枠レンダー
	SpriteRender m_hpBarBottomRender;   //HPバーの底レンダー
	SpriteRender m_hpFontRender;        //HPバーのフォントレンダー
	float        m_hpBarScale = 0;      //HPバーの拡大率
};

