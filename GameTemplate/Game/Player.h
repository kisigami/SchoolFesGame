#pragma once

class Player:public IGameObject
{
public:
	/// <summary>
	/// �v���C���[�X�e�[�g
	/// </summary>
	enum EnPlayerState
	{
		enPlayerState_Idle,   //�ҋ@�X�e�[�g
		enPlayerState_Run,    //����X�e�[�g
	};

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// �A�j���[�V����������
	/// </summary>
	void InitAnimation();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ManageState();
	/// <summary>
	/// ���ʂ̃X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessRunStateTransition();

	//�A�j���[�V�����N���b�v�̔ԍ���\��
	enum EnAnimationClip
	{
		enAnimClip_Idle,   //�ҋ@�A�j���[�V����
		enAnimClip_Run,    //����A�j���[�V����
		enAnimClip_Num     //�A�j���[�V�����̐�
	};

	EnPlayerState        m_playerState = enPlayerState_Idle;     //�v���C���[�X�e�[�g
	Animation            m_animation;                            //�A�j���[�V����
	AnimationClip        m_animClips[enAnimClip_Num];           //�A�j���[�V�����N���b�v
	ModelRender          m_modelRender;                          //���f�������_�[
	CharacterController  m_charaCon;                             //�L�����N�^�[�R���g���[���[
	Vector3              m_position;                             //���W
	Vector3              m_scale;                                //�傫��
	Vector3              m_moveSpeed;                            //�ړ����x
	Vector3              m_forward = Vector3::AxisZ;             //�v���C���[�̑O�x�N�g��
	Quaternion           m_rotation;                             //��]
};

