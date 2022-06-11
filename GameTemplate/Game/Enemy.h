#pragma once

class Enemy:public IGameObject
{
public:
	/// <summary>
	/// �G�l�~�[�X�e�[�g
	/// </summary>
	enum EnEnemyState
	{
		enEnemyState_Idle,
		enEnemyState_Run,
	};
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//�A�j���[�V�����N���b�v�̔ԍ���\��
	enum EnAnimationClip
	{
		enAnimClip_Idle,   //�ҋ@�A�j���[�V����
		enAnimClip_Run,    //����A�j���[�V����
		enAnimClip_Num     //�A�j���[�V�����̐�
	};

	/// <summary>
	/// �A�j���[�V�����̏�����
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
	/// �����蔻��
	/// </summary>
	void Collision();
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

	ModelRender          m_modelRender;
	Vector3              m_position;
	Vector3              m_scale;
	Vector3              m_moveSpeed;
	Vector3              m_forward = Vector3::AxisZ;
	Quaternion           m_rotation;
	EnEnemyState         m_enemyState = enEnemyState_Idle;     //�v���C���[�X�e�[�g
	Animation            m_animation;                            //�A�j���[�V����
	AnimationClip        m_animClips[enAnimClip_Num];            //�A�j���[�V�����N���b�v
	CharacterController  m_charaCon; 
	int                  m_hp = 3;
};

