#pragma once

class DemoRender;
class Game;

class Demo : public IGameObject
{
	enum State
	{
		Idle,
		Down,
	};
public:
	Demo();
	~Demo();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void PlayAnimation();
	void StateManage();
	void ProcessCommonStateTransition();
	void ProcessIdleStateTransition();
	void ProcessDownStateTransition();

	enum EnAnimationClip
	{
		enAnimClip_Idle,   //�ҋ@�A�j���[�V����
		enAnimClip_Down,
		enAnimClip_Num     //�A�j���[�V�����̐�
	};

	State state = Idle;
	int						m_instanceNo = 0;			// �C���X�^���X�ԍ��B
	Vector3					m_position;					// ���W�B
	Vector3					m_scale;					// �傫���B
	Quaternion				m_rotation;					// ��]�B
	DemoRender* m_demoRender = nullptr;	// �X�t�B�A�����_�[�B
	ModelRender				m_modelRender;				// ���f�������_�[�B
	Game* m_game = nullptr;
	CharacterController m_charaCon;
	AnimationClip	m_animationClip[enAnimClip_Num];
	int m_hp = 2;
};
