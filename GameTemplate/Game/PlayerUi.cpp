#include "stdafx.h"
#include "PlayerUi.h"

#include "Player.h"

namespace
{
	const float WIDTH = 1920.0f;              //�摜�̉���
	const float HEIGHT = 1080.0f;             //�摜�̏c��
	const float HPBAR_POSITION_X = -925.0f;   //HP�o�[�摜��X���W
	const float HPBAR_POSITION_Y = -540.0f;   //HP�o�[�摜��Y���W
	const float HPBAR_POSITION_Z = 0.0f;      //HP�o�[�摜��Z���W
	const float HP_FIRST_VALUE = 100.0f;      //HP�̏����l
	const float HPBAR_PIVOT_X = 0.0f;         //HP�o�[�摜�̃s�{�b�g�̑�����
	const float HPBAR_PIVOT_Y = 0.0f;         //HP�o�[�摜�̃s�{�b�g�̑�����
	const float HPBAR_SCALE_Y = 1.0f;         //HP�o�[�摜��Y���̊g�嗦
	const float HPBAR_SCALE_Z = 1.0f;         //HP�o�[�摜��Z���̊g�嗦
}

PlayerUi::PlayerUi()
{
}

PlayerUi::~PlayerUi()
{
}

bool PlayerUi::Start()
{
	//�摜�̓ǂݍ���
	m_hpBarRender.Init("Assets/sprite/player/hp/hpbar.dds", WIDTH, HEIGHT);
	m_hpBarRender.SetPosition(Vector3(HPBAR_POSITION_X, HPBAR_POSITION_Y, HPBAR_POSITION_Z));
	m_hpBarRender.SetPivot(Vector2(HPBAR_PIVOT_X, HPBAR_PIVOT_Y));
	m_hpBarFrameRender.Init("Assets/sprite/player/hp/hpbarframe.dds", WIDTH, HEIGHT);
	m_hpBarBottomRender.Init("Assets/sprite/player/hp/hpbarbottom.dds", WIDTH, HEIGHT);
	m_hpFontRender.Init("Assets/sprite/player/hp/hpfont.dds", WIDTH, HEIGHT);

	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");

	return true;
}

void PlayerUi::Update()
{
	//HP�o�[�̊g�又��
	HpBarScale();

	//�摜�̍X�V
	m_hpBarBottomRender.Update();
	m_hpBarRender.Update();
	m_hpBarFrameRender.Update();
	m_hpFontRender.Update();
}

void PlayerUi::HpBarScale()
{
	//HP�o�[�̉摜�̊g�嗦
	m_hpBarScale = m_player->GetHP() / HP_FIRST_VALUE;
	//HP�o�[�̉摜�̑傫����ݒ�
	m_hpBarRender.SetScale(Vector3(m_hpBarScale, HPBAR_SCALE_Y, HPBAR_SCALE_Z));
}

void PlayerUi::Render(RenderContext& rc)
{
	//�摜�̕`�悷��
	m_hpBarBottomRender.Draw(rc);
	m_hpBarRender.Draw(rc);
	m_hpBarFrameRender.Draw(rc);
	m_hpFontRender.Draw(rc);

}