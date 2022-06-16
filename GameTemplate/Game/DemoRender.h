#pragma once

class Game;

class DemoRender:public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SetMaxDemo(int maxdemo)
	{
		m_maxDemo = maxdemo;
	}

	void UpdateInstancingData(int instanceNo,
		const Vector3& pos,
		const Quaternion& rot,
		const Vector3& scale)
	{
		m_modelRender.UpdateInstancingData(instanceNo, pos, rot, scale);
	}

	void RemoveInstance(int instanceNo)
	{
		m_modelRender.RemoveInstance(instanceNo);
	}

	enum EnAnimationClip
	{
		enAnimClip_Idle,   //待機アニメーション
		enAnimClip_Run,    //走りアニメーション
		enAnimClip_Down,
		enAnimClip_Num     //アニメーションの数
	};

	AnimationClip	m_animationClip[enAnimClip_Num];
	ModelRender m_modelRender;
	int m_maxDemo = 0;
	Game* m_game = nullptr;
};

