#pragma once

/// <summary>
/// �t�F�[�h�N���X
/// </summary>
class Fade:public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �t�F�[�h�C�����J�n����
	/// </summary>
	void StartFadeIn()
	{
		m_fadeState = enFadeState_FadeIn;
	}
	/// <summary>
	/// �t�F�[�h�A�E�g���J�n����
	/// </summary>
	void StartFadeOut()
	{
		m_fadeState = enFadeState_FadeOut;
	}
	/// <summary>
	/// �t�F�[�h�����H
	/// </summary>
	/// <returns>true�Ȃ�t�F�[�h��</returns>
	const bool IsFade() const
	{
		return m_fadeState != enFadeState_Idle;
	}
	/// <summary>
	/// �A���t�@�l���擾����
	/// </summary>
	/// <returns></returns>
	const float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}

private:
	/// <summary>
	/// �t�F�[�h�X�e�[�g
	/// </summary>
	enum FadeState
	{
		enFadeState_Idle,    //�ҋ@�X�e�[�g
		enFadeState_FadeIn,  //�t�F�[�h�C���X�e�[�g
		enFadeState_FadeOut  //�t�F�[�h�A�E�g�X�e�[�g
	};

	SpriteRender m_spriteRender;                   //�X�v���C�g�����_�[
	FadeState    m_fadeState = enFadeState_Idle;   //�t�F�[�h�X�e�[�g
	float        m_currentAlpha = 1.0f;            //�A���t�@�l
};

