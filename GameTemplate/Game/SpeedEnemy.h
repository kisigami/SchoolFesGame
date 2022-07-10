#pragma once

class Player;
class Bullet;

class SpeedEnemy :public IGameObject
{
public:
	SpeedEnemy();
	~SpeedEnemy();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// �G�l�~�[�X�e�[�g
	/// </summary>
	enum EnEnemyState
	{
		enEnemyState_Idle,         //�ҋ@�X�e�[�g
		enEnemyState_Run,          //����X�e�[�g
		enEnemyState_Chase,        //�ǐՃX�e�[�g
		enEnemyState_Attack,       //�U���X�e�[�g
		enEnemyState_Down,         //�_�E���X�e�[�g
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
	/// �ǐՏ���
	/// </summary>
	void Chase();
	/// <summary>
	/// �����蔻��
	/// </summary>
	void Collision();
	/// <summary>
	/// �U������
	/// </summary>
	void Attack();
	/// <summary>
	/// �U���̃R���W�������쐬����
	/// </summary>
	void MakeAttackCollision();
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
	/// �ǐՃX�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	///	�U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// �_�E���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// �A�j���[�V�����̃L�[��ݒ肷�鏈��
	/// </summary>
	/// <param name="clipName">�N���b�v��</param>
	/// <param name="eventName">�C�x���g��</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// �v���C���[����݂��Ă��Ȃ����H
	/// </summary>
	/// <returns>true�Ȃ猩����</returns>
	void CantLookMe();
	/// <summary>
	/// �U���ł��邩�H
	/// </summary>
	const bool CanAttack() const;

	/// <summary>
	//�A�j���[�V�����N���b�v�̔ԍ���\��
	/// </summary>
	enum EnAnimationClip
	{
		enAnimClip_Idle,   //�ҋ@�A�j���[�V����
		enAnimClip_Run,    //����A�j���[�V����
		enAnimClip_Attack, //�U���A�j���[�V����
		enAnimClip_Down,   //�_�E���A�j���[�V����
		enAnimClip_Num     //�A�j���[�V�����̐�
	};


	ModelRender          m_modelRender;
	Animation            m_animation;                           //�A�j���[�V����
	AnimationClip        m_animClips[enAnimClip_Num];           //�A�j���[�V�����N���b�v
	int                  m_myNumber = 0;                        //�}�C�i���o�[
	Vector3              m_position;                            //���W
	Vector3              m_scale;                               //�傫��
	Vector3              m_moveSpeed;                           //�ړ����x
	Vector3              m_forward = Vector3::AxisZ;            //�G�l�~�[�̑O�����̃x�N�g��
	Quaternion           m_rotation;                            //��]
	EnEnemyState         m_enemyState = enEnemyState_Idle;      //�G�l�~�[�X�e�[�g
	CharacterController  m_charaCon;                            //�L�����R��
	int                  m_hp = 6;                              //HP
	Bullet* m_bullet = nullptr;                    //�e
	Player* m_player = nullptr;                    //�v���C���[
	bool                 m_mitukatta = false;                    //����������
	SphereCollider	     m_sphereCollider;                       //�X�q�A�R���C�_�[
	bool                 m_attacking = false;                    //�U������
	int				     m_pumchBoneId = -1;                     //�p���`�̃{�[��
	bool                 m_deadFlag = true;                      //���S���Ă��邩
};