#pragma once

#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"
//�N���X�錾
class Player;     //�v���C���[�N���X
class Bullet;     //�e�N���X
class SpawnEnemy; //�G�l�~�[�o���N���X

/// <summary>
/// �X�s�[�h�G�l�~�[�N���X
/// </summary>
class SpeedEnemy :public IGameObject
{
public:
	SpeedEnemy();
	~SpeedEnemy();
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
	/// �}�C�i���o�[��ݒ�
	/// </summary>
	/// <param name="position">�ԍ�</param>
	void SetMyNumber(const int& position)
	{
		m_myNumber = position;
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
	/// �����t���O��ݒ�
	/// </summary>
	/// <param name="flag">�t���O</param>
	void SetActiveFlag(const bool& flag)
	{
		m_isActive = flag;
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
	/// <summary>
	/// �G�l�~�[�X�e�[�g
	/// </summary>
	enum EnEnemyState
	{
		enEnemyState_Idle,         //�ҋ@�X�e�[�g
		enEnemyState_Run,          //����X�e�[�g
		enEnemyState_Chase,        //�ǐՃX�e�[�g
		enEnemyState_PathChase,    //�p�X�ړ��X�e�[�g
		enEnemyState_Attack,       //�U���X�e�[�g
		enEnemyState_Down,         //�_�E���X�e�[�g
		enEnemyState_DeActive,     //�񊈓��X�e�[�g
	};
	
	void DeActive();

	/// <summary>
	/// �A�j���[�V�����̏���������
	/// </summary>
	void InitAnimation();
	/// <summary>
	/// �p�X�ړ�����
	/// </summary>
	void PathMove();
	/// <summary>
	/// �ǐՏ���
	/// </summary>
	void Chase();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	void Collision();
	/// <summary>
	/// �U������
	/// </summary>
	void Attack();
	/// <summary>
	/// �U���p�R���W������������
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// �A�j���[�V�����̍Đ�����
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
	/// <returns>true�Ȃ猩���Ă���</returns>
	void CantLookMe();
	/// <summary>
	/// �U���ł��邩�H
	/// </summary>
	/// <returns>true�Ȃ�ł���</returns>
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

	Bullet* m_bullet = nullptr;                    //�e
	Player* m_player = nullptr;                    //�v���C���[

	ModelRender          m_modelRender;
	Animation            m_animation;                           //�A�j���[�V����
	AnimationClip        m_animClips[enAnimClip_Num];           //�A�j���[�V�����N���b�v
	Vector3              m_position;                            //���W
	Vector3              m_scale;                               //�傫��
	Vector3              m_moveSpeed;                           //�ړ����x
	Vector3              m_forward = Vector3::AxisZ;            //�G�l�~�[�̑O�����̃x�N�g��
	Quaternion           m_rotation;                            //��]
	EnEnemyState         m_enemyState = enEnemyState_Idle;      //�G�l�~�[�X�e�[�g
	CharacterController  m_charaCon;                            //�L�����R��
	int                  m_hp = 3;                              //HP
	bool                 m_mitukatta = false;                    //����������
	SphereCollider	     m_sphereCollider;                       //�X�q�A�R���C�_�[
	bool                 m_attacking = false;                    //�U������
	int				     m_pumchBoneId = -1;                     //�p���`�̃{�[��
	int                  m_myNumber = 0;                        //�}�C�i���o�[
	bool                 m_isActive = false;
	SpawnEnemy* m_spawnEnemy = nullptr;
	SoundSource* m_se = nullptr;
	SoundSource* m_walkse;
	bool soundflag = false;
	float          m_pathTimer = 1.0f;
	TknFile m_tknFile;
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::PathFinding m_pathFiding;
};