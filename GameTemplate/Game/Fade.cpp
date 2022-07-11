#include "stdafx.h"
#include "Fade.h"

namespace
{
	const float SPRITE_WIDTH = 1920.0f;            //�摜�̉���
	const float SPRITE_HEIGHT = 1080.0f;           //�摜�̏c��
	const float ALPHA_CHANGE_VALUE = 2.0f;         //�A���t�@�l�̕ω��l
	const float ALPHA_MAX_VALUE = 1.0f;            //�A���t�@�l�̍ő�l
	const float ALPHA_MIN_VALUE = 0.0f;            //�A���t�@�l�̍Œ�l
	const float FADE_SPRITE_COLOR_R_VALUE = 1.0f;  //�t�F�[�h�摜��R�����̒l
	const float FADE_SPRITE_COLOR_G_VALUE = 1.0f;  //�t�F�[�h�摜��G�����̒l
	const float FADE_SPRITE_COLOR_B_VALUE = 1.0f;  //�t�F�[�h�摜��B�����̒l
}
Fade::Fade()
{
}

Fade::~Fade()
{
}

bool Fade::Start()
{
	//�t�F�[�h���ɕ\������摜��ǂݍ���
	m_spriteRender.Init("Assets/sprite/fade/fade.dds", SPRITE_WIDTH, SPRITE_HEIGHT);
	return true;
}

void Fade::Update()
{
	switch (m_fadeState)
	{
		//�ҋ@�X�e�[�g��������
	case Fade::enFadeState_Idle:
		break;
		//�t�F�[�h�C���X�e�[�g��������
	case Fade::enFadeState_FadeIn:
		//�摜�̃A���t�@�l��������
		m_currentAlpha -= ALPHA_CHANGE_VALUE * g_gameTime->GetFrameDeltaTime();
		//�摜�̃A���t�@�l���A���t�@�l�̍Œ�l��菬����������
		if (m_currentAlpha <= ALPHA_MIN_VALUE)
		{
			//�摜�̃A���t�@���Œ�l�ɂ���
			m_currentAlpha = ALPHA_MIN_VALUE;
			//�ҋ@�X�e�[�g�Ɉڍs����
			m_fadeState = enFadeState_Idle;
		}
		break;
		//�t�F�[�h�A�E�g�X�e�[�g��������
	case Fade::enFadeState_FadeOut:
		//�摜�̃A���t�@�l��������
		m_currentAlpha += ALPHA_CHANGE_VALUE * g_gameTime->GetFrameDeltaTime();
		//�摜�̃A���t�@�l���A���t�@�l�̍ő�l���傫��������
		if (m_currentAlpha >= ALPHA_MAX_VALUE)
		{
			//�摜�̃A���t�@�l���ő�l�ɂ���
			m_currentAlpha = ALPHA_MAX_VALUE;
			//�ҋ@�X�e�[�g�Ɉڍs����
			m_fadeState = enFadeState_Idle;
		}
		break;
	}
}

void Fade::Render(RenderContext& rc)
{
	//�摜�̃A���t�@�l���A���t�@�l�̍Œ�l���傫��������
	if (m_currentAlpha > ALPHA_MIN_VALUE)
	{
		//�摜�̏�Z�J���[��ݒ肷��
		m_spriteRender.SetMulColor({ 
			FADE_SPRITE_COLOR_R_VALUE,
			FADE_SPRITE_COLOR_G_VALUE,
			FADE_SPRITE_COLOR_B_VALUE,
			m_currentAlpha });
		//�摜��`�悷��
		m_spriteRender.Draw(rc);
	}
}