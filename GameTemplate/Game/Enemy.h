#pragma once

//�o�H�T���Ŏg�����
#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

//�N���X�錾
class Bullet;   //�e
class Enemy;
class Player;

/// <summary>
/// �G�l�~�[�N���X
/// </summary>
class Enemy:public IGameObject
{
public:
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
	void SetMyNumber(const int& number)
	{
		m_myNumber = number;
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
		enAnimClip_Attack, //�U���A�j���[�V����
		enAnimClip_Down,   //�_�E���A�j���[�V����
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
	/// �o�H�T������
	/// </summary>
	void PathMove();
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
	/// ���S��̏���
	/// </summary>
	void DeathSpawn();
	/// <summary>
	/// ���S��̏���
	/// </summary>
	void Spawn();
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
	
	
	int                  m_myNumber = 0;                        //�}�C�i���o�[
	ModelRender          m_modelRender;                         //���f�������_�[
	LevelRender          m_levelRender;                         //���x�������_�[
	Vector3              m_position;                            //���W
	Vector3              m_scale;                               //�傫��
	Vector3              m_moveSpeed;                           //�ړ����x
	Vector3              m_forward = Vector3::AxisZ;            //�G�l�~�[�̑O�����̃x�N�g��
	Quaternion           m_rotation;                            //��]
	EnEnemyState         m_enemyState = enEnemyState_Idle;      //�G�l�~�[�X�e�[�g
	Animation            m_animation;                           //�A�j���[�V����
	AnimationClip        m_animClips[enAnimClip_Num];           //�A�j���[�V�����N���b�v
	CharacterController  m_charaCon;                            //�L�����R��
	int                  m_hp = 6;                              //HP
	Bullet*              m_bullet = nullptr;                    //�e
	Player*              m_player = nullptr;                    //�v���C���[
	bool                m_mitukatta = false;                    //����������
	SphereCollider	    m_sphereCollider;                       //�X�q�A�R���C�_�[
	bool                ttt = false;                            //flag
	bool                m_attacking = false;                    //�U������
	int				    m_pumchBoneId = -1;                     //�p���`�̃{�[��
	bool                m_deadFlag = true;                      //���S���Ă��邩

	//�o�H�T���Ŏg�����
	TknFile m_tknFile;
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::PathFinding m_pathFiding;
	float m_pathTimer = 200.0f;
};

