#pragma once

class Player;   //�v���C���[�N���X

/// <summary>
/// �v���C���[UI�N���X
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
	/// HP�o�[�̊g�又��
	/// </summary>
	void HpBarScale();

	//�N���X��`
	Player*      m_player = nullptr;    //�v���C���[

	SpriteRender m_hpBarRender;         //HP�o�[�����_�[
	SpriteRender m_hpBarFrameRender;    //HP�o�[�̘g�����_�[
	SpriteRender m_hpBarBottomRender;   //HP�o�[�̒ꃌ���_�[
	SpriteRender m_hpFontRender;        //HP�o�[�̃t�H���g�����_�[
	SpriteRender m_targetRender;        //�e�����ł����ꏊ�̖ڈ󃌃��_�[
	float        m_hpBarScale = 0;      //HP�o�[�̊g�嗦
};

