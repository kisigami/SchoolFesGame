#pragma once

#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

//�N���X�錾
class Bullet;        //�e�N���X
class Enemy;         //�G�l�~�[�N���X
class Player;        //�v���C���[�N���X
class SpawnEnemy;    //�G�l�~�[�o���N���X

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
	/// <summary>
	/// �傫����ݒ�
	/// </summary>
	/// <param name="scale">�傫��</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
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
	/// �}�C�i���o�[��ݒ�
	/// </summary>
	/// <param name="number">�ԍ�</param>
	void SetMyNumber(const int& number)
	{
		m_myNumber = number;
	}
	/// <summary>
	/// �����t���O��ݒ�
	/// </summary>
	/// <param name="flag">�t���O</param>
	void SetActiveFlag(const bool& flag)
	{
		m_isActive = flag;
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
	/// �傫�����擾
	/// </summary>
	/// <returns>�傫��</returns>
	const Vector3& GetScale() const
	{
		return m_scale;
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
	/// �}�C�i���o�[���擾
	/// </summary>
	/// <returns>�ԍ�</returns>
	const int& GetMyNumber() const
	{
		return m_myNumber;
	}
	/// <summary>
	/// �����t���O���擾
	/// </summary>
	/// <returns>�t���O</returns>
	const bool& GetActiveFlag() const
	{
		return m_isActive;
	}

private:
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
	/// �v���C���[���猩���Ă��邩�H
	/// </summary>
	/// <returns>true�Ȃ猩���Ă���</returns>
	void CantLookMe();
	/// <summary>
	/// �U���ł��邩�H
	/// </summary>
	/// <returns>true�Ȃ�ł���</returns>
	const bool CanAttack() const;
	
	//�N���X��`
	SpawnEnemy*         m_spawnEnemy = nullptr;            //�G�l�~�[�̏o��
	Bullet*             m_bullet = nullptr;                //�e
	Player*             m_player = nullptr;                //�v���C���[
	//�X�e�[�^�X
	EnEnemyState        m_enemyState = enEnemyState_Idle;  //�G�l�~�[�X�e�[�g
	int                 m_myNumber = 0;                    //�}�C�i���o�[
	int                 m_hp = 6;                          //HP
	//�t���O
	bool                m_mitukatta = false;               //�������Ă��邩�H
	bool                m_attacking = false;               //�U������
	bool                m_deadFlag = true;                 //���S���Ă��邩
	//�o�H�T��
	TknFile             m_tknFile;                         //tkm�t�@�C��
	nsAI::NaviMesh      m_nvmMesh;                         //�i�r���b�V��
	nsAI::Path          m_path;                            //�p�X
 	nsAI::PathFinding   m_pathFiding;                      //�p�X����
	float               m_pathTimer = 200.0f;              //�p�X�����^�C�}�[

	ModelRender         m_modelRender;                     //���f�������_�[
	Vector3             m_position;                        //���W
	Vector3             m_scale;                           //�傫��
	Vector3             m_moveSpeed;                       //�ړ����x
	Vector3             m_forward = Vector3::AxisZ;        //�G�l�~�[�̑O�����̃x�N�g��
	Quaternion          m_rotation;                        //��]
	Animation           m_animation;                       //�A�j���[�V����
	AnimationClip       m_animClips[enAnimClip_Num];       //�A�j���[�V�����N���b�v
	CharacterController m_charaCon;                        //�L�����R��
	SphereCollider	    m_sphereCollider;                  //�X�q�A�R���C�_�[
	int				    m_pumchBoneId = -1;                //�p���`�̃{�[��
};

