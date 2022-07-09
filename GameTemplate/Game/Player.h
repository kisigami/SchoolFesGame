#pragma once

//�N���X�錾
class GameCamera;  //�Q�[���J�����N���X
class Enemy;       //�G�l�~�[�N���X

/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player :public IGameObject
{
public:
	Player();
	~Player();
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
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
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
	/// ��]���擾
	/// </summary>
	/// <returns>��]</returns>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}
	/// <summary>
	/// HP���擾
	/// </summary>
	/// <returns>HP</returns>
	const int& GetHP() const
	{
		return m_hp;
	}
	const Vector3& GetForward() const
	{
		return m_forward;
	}
private:
	/// <summary>
	/// �v���C���[�X�e�[�g
	/// </summary>
	enum EnPlayerState
	{
		enPlayerState_Idle,            //�ҋ@�X�e�[�g
		enPlayerState_Run,             //����X�e�[�g
		enPlayerState_Shot,            //�ˌ��X�e�[�g
		enPlayerState_ReceiveDamage,   //��_���[�W�X�e�[�g
		enPlayerState_Down,            //�_�E���X�e�[�g
	};
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
	/// �U���̓����蔻�菈��
	/// </summary>
	void Collision();
	/// <summary>
	/// �ˌ�����
	/// </summary>
	void Shot();
	/// <summary>
	/// �e�쐬����
	/// </summary>
	void MakeBullet();
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
	/// <summary>
	/// �ˌ��X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessShotStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// �_�E���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// �G�t�F�N�g�e�X�g
	/// </summary>
	void EffectPlay();
	/// <summary>
	///�A�j���[�V�����̃L�[��ݒ�
	/// </summary>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	//�A�j���[�V�����N���b�v�̔ԍ���\��
	enum EnAnimationClip
	{
		enAnimClip_RifleIdle,   //�ҋ@�A�j���[�V����
		enAnimClip_RifleRun,    //����A�j���[�V����
		enAnimClip_RifleShot,   //�ˌ��A�j���[�V����
		enAnimClip_Num          //�A�j���[�V�����̐�
	};

	FontRender           m_fontRender;                           //�t�H���g�����_�[
	EnPlayerState        m_playerState = enPlayerState_Idle;     //�v���C���[�X�e�[�g
	Animation            m_animation;                            //�A�j���[�V����
	AnimationClip        m_animClips[enAnimClip_Num];            //�A�j���[�V�����N���b�v
	ModelRender          m_modelRender;                          //���f�������_�[
	CharacterController  m_charaCon;                             //�L�����N�^�[�R���g���[���[
	Vector3              m_position;                             //���W
	Vector3              m_scale;                                //�傫��
	Vector3              m_moveSpeed;                            //�ړ����x
	Vector3              m_forward = Vector3::AxisZ;             //�v���C���[�̑O�x�N�g��
	Quaternion           m_rotation;                             //��]
	GameCamera*          m_gameCamera = nullptr;                 //�Q�[���J����
	Enemy*               m_enemy = nullptr;                      //�G�l�~�[
	float                m_bulletNum = 30;                       //�c�e��
	int                  m_hp = 100;                             //�q�b�g�|�C���g
	float                m_receiveDamageTimer = 1.0f;            //���G�^�C�}�[
	float x = 0.0f;
	Vector3 m_toCameraPos;
};

