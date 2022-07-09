#include "stdafx.h"
#include "PlayerUi.h"

#include "Player.h"

namespace
{
	const float WIDTH = 1920.0f;              //画像の横幅
	const float HEIGHT = 1080.0f;             //画像の縦幅
	const float HPBAR_POSITION_X = -925.0f;   //HPバー画像のX座標
	const float HPBAR_POSITION_Y = -540.0f;   //HPバー画像のY座標
	const float HPBAR_POSITION_Z = 0.0f;      //HPバー画像のZ座標
	const float HP_FIRST_VALUE = 100.0f;      //HPの初期値
	const float HPBAR_PIVOT_X = 0.0f;         //HPバー画像のピボットの第一引数
	const float HPBAR_PIVOT_Y = 0.0f;         //HPバー画像のピボットの第二引数
	const float HPBAR_SCALE_Y = 1.0f;         //HPバー画像のY軸の拡大率
	const float HPBAR_SCALE_Z = 1.0f;         //HPバー画像のZ軸の拡大率
}

PlayerUi::PlayerUi()
{
}

PlayerUi::~PlayerUi()
{
}

bool PlayerUi::Start()
{
	//画像の読み込み
	m_hpBarRender.Init("Assets/sprite/player/hp/hpbar.dds", WIDTH, HEIGHT);
	m_hpBarRender.SetPosition(Vector3(HPBAR_POSITION_X, HPBAR_POSITION_Y, HPBAR_POSITION_Z));
	m_hpBarRender.SetPivot(Vector2(HPBAR_PIVOT_X, HPBAR_PIVOT_Y));
	m_hpBarFrameRender.Init("Assets/sprite/player/hp/hpbarframe.dds", WIDTH, HEIGHT);
	m_hpBarBottomRender.Init("Assets/sprite/player/hp/hpbarbottom.dds", WIDTH, HEIGHT);
	m_hpFontRender.Init("Assets/sprite/player/hp/hpfont.dds", WIDTH, HEIGHT);

	//プレイヤーのインスタンスを探す
	m_player = FindGO<Player>("player");

	return true;
}

void PlayerUi::Update()
{
	//HPバーの拡大処理
	HpBarScale();

	//画像の更新
	m_hpBarBottomRender.Update();
	m_hpBarRender.Update();
	m_hpBarFrameRender.Update();
	m_hpFontRender.Update();
}

void PlayerUi::HpBarScale()
{
	//HPバーの画像の拡大率
	m_hpBarScale = m_player->GetHP() / HP_FIRST_VALUE;
	//HPバーの画像の大きさを設定
	m_hpBarRender.SetScale(Vector3(m_hpBarScale, HPBAR_SCALE_Y, HPBAR_SCALE_Z));
}

void PlayerUi::Render(RenderContext& rc)
{
	//画像の描画する
	m_hpBarBottomRender.Draw(rc);
	m_hpBarRender.Draw(rc);
	m_hpBarFrameRender.Draw(rc);
	m_hpFontRender.Draw(rc);

}