#pragma once

//�N���X�錾
class GameCamera;

class Player :public IGameObject
{
public:
	/// <summary>
	/// �v���C���[�X�e�[�g
	/// </summary>
	enum EnPlayerState
	{
		enPlayerState_Idle,   //�ҋ@�X�e�[�g
		enPlayerState_Run,    //����X�e�[�g
		enPlayerState_Shot,
		enPlayerState_Back
	};

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
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}
	/// <summary>
	/// �E���W���擾
	/// </summary>
	/// <returns>�E���W</returns>
	const Vector3& GetRight() const
	{
		return m_right;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

private:
	void MakeBullet();

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
	/// <summary>
	/// �ˌ��X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessShotStateTransition();

	void ProcessBackStateTransition();
	//�A�j���[�V�����N���b�v�̔ԍ���\��
	enum EnAnimationClip
	{
		enAnimClip_Idle,   //�ҋ@�A�j���[�V����
		enAnimClip_Run,    //����A�j���[�V����
		enAnimClip_Shot,
		enAnimClip_Back,
		enAnimClip_Num     //�A�j���[�V�����̐�
	};

	EnPlayerState        m_playerState = enPlayerState_Idle;     //�v���C���[�X�e�[�g
	Animation            m_animation;                            //�A�j���[�V����
	AnimationClip        m_animClips[enAnimClip_Num];            //�A�j���[�V�����N���b�v
	ModelRender          m_modelRender;                          //���f�������_�[
	ModelRender          m_rightHandRender;
	ModelRender          m_leftHandRender;
	CharacterController  m_charaCon;                             //�L�����N�^�[�R���g���[���[
	Vector3              m_position;                             //���W
	Vector3              m_scale;                                //�傫��
	Vector3              m_moveSpeed;                            //�ړ����x
	Vector3              m_forward = Vector3::AxisZ;             //�v���C���[�̑O�x�N�g��
	Vector3              m_right = Vector3::AxisX;
	Quaternion           m_rotation;                             //��]
	GameCamera*          m_gameCamera;                           //�Q�[���J����
};

