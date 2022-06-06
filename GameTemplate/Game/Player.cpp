#include "stdafx.h"
#include "Player.h"

namespace
{

}

bool Player::Start()
{
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/unityChan.tkm");

	return true;
}

void Player::Update()
{
	//モデルの更新
	m_modelRender.Update();
}

void Player::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender.Draw(rc);
}