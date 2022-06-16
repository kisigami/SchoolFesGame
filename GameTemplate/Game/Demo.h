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
		enAnimClip_Idle,   //待機アニメーション
		enAnimClip_Down,
		enAnimClip_Num     //アニメーションの数
	};

	State state = Idle;
	int						m_instanceNo = 0;			// インスタンス番号。
	Vector3					m_position;					// 座標。
	Vector3					m_scale;					// 大きさ。
	Quaternion				m_rotation;					// 回転。
	DemoRender* m_demoRender = nullptr;	// スフィアレンダー。
	ModelRender				m_modelRender;				// モデルレンダー。
	Game* m_game = nullptr;
	CharacterController m_charaCon;
	AnimationClip	m_animationClip[enAnimClip_Num];
	int m_hp = 2;
};
