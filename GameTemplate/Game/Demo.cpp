#include "stdafx.h"
#include "Demo.h"
#include "DemoRender.h"
#include "Game.h"

Demo::Demo()
{

}

Demo::~Demo()
{
	m_demoRender->RemoveInstance(m_instanceNo);
}

bool Demo::Start()
{
	m_demoRender = FindGO<DemoRender>("demorender");
	m_game = FindGO<Game>("game");

	m_animationClip[enAnimClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Down].Load("Assets/animData/enemy/down.tka");
	m_animationClip[enAnimClip_Down].SetLoopFlag(false);

	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisY);

	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	m_charaCon.Init(12.0f, 50.0f, m_position);

	m_modelRender.Update();
	return true;
}

void Demo::Update()
{
	PlayAnimation();
	StateManage();

	{
		//敵の攻撃用のコリジョンの配列を取得する。
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("bullet");
		//配列をfor文で回す。
		for (auto collision : collisions)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				//HPを減らす。
				m_hp -= 1;
				//コリジョンを非アクティブにする
				collision->Dead();

				//HPが0になったら。
				if (m_hp <= 0)
				{
					state = Down;
					m_charaCon.RemoveRigidBoby();
				}
				return;
			}
		}
	}

	m_demoRender->UpdateInstancingData(
		m_instanceNo,
		m_position,
		m_rotation,
		m_scale
	);
}

void Demo::PlayAnimation()
{
	switch (state)
	{
	case Demo::Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.1f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
	case Demo::Down:
		m_modelRender.PlayAnimation(enAnimClip_Down, 0.1f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
	}
}

void Demo::ProcessCommonStateTransition()
{
	state = Idle;
}

void Demo::ProcessDownStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		DeleteGO(this);
	}
}

void Demo::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void Demo::StateManage()
{
	switch (state)
	{
	case Demo::Idle:
		ProcessIdleStateTransition();
		break;
	case Demo::Down:
		ProcessDownStateTransition();
		break;
	}
}

void Demo::Render(RenderContext& rc)
{
}