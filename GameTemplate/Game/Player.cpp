#include "stdafx.h"
#include "Player.h"

namespace
{

}

bool Player::Start()
{
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/unityChan.tkm");

	return true;
}

void Player::Update()
{
	//���f���̍X�V
	m_modelRender.Update();
}

void Player::Render(RenderContext& rc)
{
	//���f���̕`��
	m_modelRender.Draw(rc);
}